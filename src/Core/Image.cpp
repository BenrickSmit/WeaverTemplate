/**
 * @file Image.cpp
 * @author B.G. Smit
 * @brief Implements image loading, manipulation, and rendering functionalities.
 *
 * This file provides the implementation for the `Image` class, which handles
 * loading image data from various formats, managing image memory on the GPU,
 * and preparing images for display within the application's user interface.
 * It integrates with Vulkan for efficient texture management.
 * @copyright Copyright (c) 2025
 */
#include "Image.h"

#include "Canvas.h"
#include "Log.h"
#include "Windows.h"
#include "backends/imgui_impl_vulkan.h"
#include "imgui.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stdexcept>

#include "stb_image/stb_image.h"

namespace Weaver {

/**
 * @namespace Utils
 * @brief Contains utility functions for the Image class.
 */
namespace Utils {

/**
 * @brief Gets the Vulkan memory type index for a given memory type and properties.
 * @param properties The memory properties.
 * @param type_bits The memory type bits.
 * @return The memory type index.
 */
static uint32_t GetVulkanMemoryType(VkMemoryPropertyFlags properties, uint32_t type_bits) {
  VkPhysicalDeviceMemoryProperties prop;
  vkGetPhysicalDeviceMemoryProperties(Canvas::GetPhysicalDevice(), &prop);
  for (uint32_t i = 0; i < prop.memoryTypeCount; i++) {
    if ((prop.memoryTypes[i].propertyFlags & properties) == properties && type_bits & (1 << i))
      return i;
  }

  return 0xffffffff;
}

/**
 * @brief Gets the number of bytes per pixel for a given image format.
 * @param format The image format.
 * @return The number of bytes per pixel.
 */
static uint32_t BytesPerPixel(ImageFormat format) {
  switch (format) {
    case ImageFormat::RGBA:
      return 4;
    case ImageFormat::RGBA32F:
      return 16;
  }
  return 0;
}

/**
 * @brief Converts a Weaver image format to a Vulkan image format.
 * @param format The Weaver image format.
 * @return The Vulkan image format.
 */
static VkFormat WeaverFormatToVulkanFormat(ImageFormat format) {
  switch (format) {
    case ImageFormat::RGBA:
      return VK_FORMAT_R8G8B8A8_UNORM;
    case ImageFormat::RGBA32F:
      return VK_FORMAT_R32G32B32A32_SFLOAT;
  }
  return (VkFormat)0;
}

}  // namespace Utils

/**
 * @brief Constructs an Image object from a file path.
 * @param path The path to the image file.
 */
Image::Image(std::string_view path) : m_Filepath(path) {
  int width, height, channels;
  uint8_t* data = nullptr;

  if (stbi_is_hdr(m_Filepath.c_str())) {
    data = (uint8_t*)stbi_loadf(m_Filepath.c_str(), &width, &height, &channels, 4);
    m_Format = ImageFormat::RGBA32F;
  } else {
    data = stbi_load(m_Filepath.c_str(), &width, &height, &channels, 4);
    m_Format = ImageFormat::RGBA;
  }
  if (!data) {
    printf("Failed to load image: %s\n", stbi_failure_reason());
    return;
  }

  m_Width = width;
  m_Height = height;

  AllocateMemory(m_Width * m_Height * Utils::BytesPerPixel(m_Format));
  SetData(data);
  stbi_image_free(data);
}

/**
 * @brief Constructs an Image object with a specified width, height, and format.
 * @param width The width of the image.
 * @param height The height of the image.
 * @param format The format of the image.
 * @param data Optional initial data for the image.
 */
Image::Image(uint32_t width, uint32_t height, ImageFormat format, const void* data)
    : m_Width(width),
      m_Height(height),
      m_Format(format) {
  AllocateMemory(m_Width * m_Height * Utils::BytesPerPixel(m_Format));
  if (data)
    SetData(data);
}

/**
 * @brief Destroys the Image object and releases its resources.
 */
Image::~Image() {
  Release();
}

/**
 * @brief Allocates memory for the image.
 * @param size The size of the memory to allocate.
 */
void Image::AllocateMemory(uint64_t size) {
  VkDevice device = Canvas::GetDevice();

  VkResult err;

  VkFormat vulkanFormat = Utils::WeaverFormatToVulkanFormat(m_Format);

  // Create the Image
  {
    VkImageCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    info.imageType = VK_IMAGE_TYPE_2D;
    info.format = vulkanFormat;
    info.extent.width = m_Width;
    info.extent.height = m_Height;
    info.extent.depth = 1;
    info.mipLevels = 1;
    info.arrayLayers = 1;
    info.samples = VK_SAMPLE_COUNT_1_BIT;
    info.tiling = VK_IMAGE_TILING_OPTIMAL;
    info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    err = vkCreateImage(device, &info, nullptr, &m_Image);
    check_vk_result(err);
    VkMemoryRequirements req;
    vkGetImageMemoryRequirements(device, m_Image, &req);
    VkMemoryAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = req.size;
    alloc_info.memoryTypeIndex =
        Utils::GetVulkanMemoryType(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, req.memoryTypeBits);
    if (alloc_info.memoryTypeIndex == 0xffffffff) {
      throw std::runtime_error("Failed to find a suitable memory type!");
    }

    err = vkAllocateMemory(device, &alloc_info, nullptr, &m_Memory);
    check_vk_result(err);
    err = vkBindImageMemory(device, m_Image, m_Memory, 0);
    check_vk_result(err);
  }

  // Create the Image View:
  {
    VkImageViewCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    info.image = m_Image;
    info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    info.format = vulkanFormat;
    info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    info.subresourceRange.levelCount = 1;
    info.subresourceRange.layerCount = 1;
    err = vkCreateImageView(device, &info, nullptr, &m_ImageView);
    check_vk_result(err);
  }

  // Create sampler:
  {
    VkSamplerCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    info.magFilter = VK_FILTER_LINEAR;
    info.minFilter = VK_FILTER_LINEAR;
    info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    info.minLod = -1000;
    info.maxLod = 1000;
    info.maxAnisotropy = 1.0f;
    VkResult err = vkCreateSampler(device, &info, nullptr, &m_Sampler);
    check_vk_result(err);
  }

  // Verification
  if (m_Sampler == VK_NULL_HANDLE || m_ImageView == VK_NULL_HANDLE) {
    throw std::runtime_error("Sampler or ImageView is invalid");
  }

  // Create the Descriptor Set:
  m_DescriptorSet =
      ImGui_ImplVulkan_AddTexture(m_Sampler, m_ImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

  if (m_DescriptorSet == VK_NULL_HANDLE) {
    throw std::runtime_error("Failed to create descriptor set with ImGui_ImplVulkan_AddTexture");
  }
}

/**
 * @brief Releases all resources used by the image.
 */
void Image::Release() {
  if (m_DescriptorSet != VK_NULL_HANDLE) {
    ImGui_ImplVulkan_RemoveTexture(m_DescriptorSet);
    m_DescriptorSet = VK_NULL_HANDLE;
  }

  Canvas::SubmitResourceFree([sampler = m_Sampler,
                                 imageView = m_ImageView,
                                 image = m_Image,
                                 memory = m_Memory,
                                 stagingBuffer = m_StagingBuffer,
                                 stagingBufferMemory = m_StagingBufferMemory]() {
    VkDevice device = Canvas::GetDevice();

    vkDestroySampler(device, sampler, nullptr);
    vkDestroyImageView(device, imageView, nullptr);
    vkDestroyImage(device, image, nullptr);
    vkFreeMemory(device, memory, nullptr);
    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
  });

  m_Sampler = VK_NULL_HANDLE;
  m_ImageView = VK_NULL_HANDLE;
  m_Image = VK_NULL_HANDLE;
  m_Memory = VK_NULL_HANDLE;
  m_StagingBuffer = VK_NULL_HANDLE;
  m_StagingBufferMemory = VK_NULL_HANDLE;
}

/**
 * @brief Sets the image data.
 * @param data A pointer to the image data.
 */
void Image::SetData(const void* data) {
  VkDevice device = Canvas::GetDevice();
  if (m_Width == 0 || m_Height == 0) {
    m_Width = 200;
    m_Height = 200;
  }

  size_t upload_size = m_Width * m_Height * Utils::BytesPerPixel(m_Format);

  VkResult err;

  if (m_StagingBuffer == VK_NULL_HANDLE) {
    // Create the Upload Buffer
    {
      VkBufferCreateInfo buffer_info = {};
      buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
      buffer_info.size = upload_size;
      buffer_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
      buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
      err = vkCreateBuffer(device, &buffer_info, nullptr, &m_StagingBuffer);
      check_vk_result(err);
      VkMemoryRequirements req;
      vkGetBufferMemoryRequirements(device, m_StagingBuffer, &req);
      m_AlignedSize = req.size;
      VkMemoryAllocateInfo alloc_info = {};
      alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      // alloc_info.allocationSize = req.size;
      alloc_info.allocationSize = (req.size + req.alignment - 1) & ~(req.alignment - 1);
      alloc_info.memoryTypeIndex =
          Utils::GetVulkanMemoryType(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, req.memoryTypeBits);
      err = vkAllocateMemory(device, &alloc_info, nullptr, &m_StagingBufferMemory);
      check_vk_result(err);
      err = vkBindBufferMemory(device, m_StagingBuffer, m_StagingBufferMemory, 0);
      check_vk_result(err);
    }
  }

  // Upload to Buffer
  {
    char* map = NULL;
    err = vkMapMemory(device, m_StagingBufferMemory, 0, m_AlignedSize, 0, (void**)(&map));
    check_vk_result(err);
    memcpy(map, data, upload_size);
    VkMappedMemoryRange range[1] = {};
    range[0].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    range[0].memory = m_StagingBufferMemory;
    range[0].size = m_AlignedSize;
    err = vkFlushMappedMemoryRanges(device, 1, range);
    check_vk_result(err);
    vkUnmapMemory(device, m_StagingBufferMemory);
  }

  // Copy to Image
  {
    VkCommandBuffer command_buffer = Canvas::GetCommandBuffer(true);

    if (command_buffer == VK_NULL_HANDLE) {
      throw std::runtime_error("Failed to allocate command buffer!");
      // MessageBox(nullptr, TEXT("Failed to allocate command buffer!!!"), TEXT("Message"), MB_OK);
    }

    VkImageMemoryBarrier copy_barrier = {};
    copy_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    copy_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    copy_barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    copy_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    copy_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    copy_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    copy_barrier.image = m_Image;
    copy_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copy_barrier.subresourceRange.levelCount = 1;
    copy_barrier.subresourceRange.layerCount = 1;
    vkCmdPipelineBarrier(command_buffer,
        VK_PIPELINE_STAGE_HOST_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        0,
        0,
        NULL,
        0,
        NULL,
        1,
        &copy_barrier);

    VkBufferImageCopy region = {};
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.layerCount = 1;
    region.imageExtent.width = m_Width;
    region.imageExtent.height = m_Height;
    region.imageExtent.depth = 1;
    vkCmdCopyBufferToImage(
        command_buffer, m_StagingBuffer, m_Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    VkImageMemoryBarrier use_barrier = {};
    use_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    use_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    use_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    use_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    use_barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    use_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    use_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    use_barrier.image = m_Image;
    use_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    use_barrier.subresourceRange.levelCount = 1;
    use_barrier.subresourceRange.layerCount = 1;
    vkCmdPipelineBarrier(command_buffer,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        0,
        0,
        NULL,
        0,
        NULL,
        1,
        &use_barrier);

    Canvas::FlushCommandBuffer(command_buffer);
  }
}

/**
 * @brief Resizes the image.
 * @param width The new width of the image.
 * @param height The new height of the image.
 */
void Image::Resize(uint32_t width, uint32_t height) {
  if (m_Image && m_Width == width && m_Height == height)
    return;

  if (m_Width == 0 || m_Height == 0) {
    m_Width = 200;
    m_Height = 200;
  }
  // TODO: max size?

  m_Width = width;
  m_Height = height;

  Release();
  AllocateMemory(m_Width * m_Height * Utils::BytesPerPixel(m_Format));
}

}  // namespace Weaver
