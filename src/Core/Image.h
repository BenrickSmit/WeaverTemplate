/**
 * @file Image.h
 * @author B.G. Smit
 * @brief Declares the Image class for handling image data and textures.
 *
 * This file defines the `Image` class, which encapsulates image loading,
 * storage, and management, particularly for use as textures in a Vulkan
 * rendering context.
 * It provides functionalities for setting image data, resizing, and retrieving
 * a Vulkan descriptor set for rendering.
 * @copyright Copyright (c) 2025
 */
#ifndef IMAGE_H
#define IMAGE_H

#pragma once

#include <vulkan/vulkan.h>

#include <string>

namespace Weaver {

/**
 * @enum ImageFormat
 * @brief Specifies the format of the image data.
 */
enum class ImageFormat { None = 0, /**< No format specified. */ RGBA, /**< 8-bit RGBA format. */ RGBA32F /**< 32-bit floating point RGBA format. */ };

/**
 * @class Image
 * @brief Represents an image that can be used as a texture in the rendering engine.
 */
class Image {
 public:
  /**
   * @brief Constructs an Image object from a file path.
   * @param path The path to the image file.
   */
  Image(std::string_view path);
  /**
   * @brief Constructs an Image object with a specified width, height, and format.
   * @param width The width of the image.
   * @param height The height of the image.
   * @param format The format of the image.
   * @param data Optional initial data for the image.
   */
  Image(uint32_t width, uint32_t height, ImageFormat format, const void* data = nullptr);
  /**
   * @brief Destroys the Image object and releases its resources.
   */
  ~Image();

  /**
   * @brief Sets the image data.
   * @param data A pointer to the image data.
   */
  void SetData(const void* data);

  /**
   * @brief Gets the Vulkan descriptor set for the image.
   * @return The Vulkan descriptor set.
   */
  VkDescriptorSet GetDescriptorSet() const {
    return m_DescriptorSet;
  }

  /**
   * @brief Resizes the image.
   * @param width The new width of the image.
   * @param height The new height of the image.
   */
  void Resize(uint32_t width, uint32_t height);

  /**
   * @brief Gets the width of the image.
   * @return The width of the image.
   */
  uint32_t GetWidth() const {
    return m_Width;
  }
  /**
   * @brief Gets the height of the image.
   * @return The height of the image.
   */
  uint32_t GetHeight() const {
    return m_Height;
  }

 private:
  /**
   * @brief Allocates memory for the image.
   * @param size The size of the memory to allocate.
   */
  void AllocateMemory(uint64_t size);
  /**
   * @brief Releases all resources used by the image.
   */
  void Release();

 private:
  uint32_t m_Width = 0, m_Height = 0;

  //   VkImage m_Image = nullptr;
  //   VkImageView m_ImageView = nullptr;
  //   VkDeviceMemory m_Memory = nullptr;
  //   VkSampler m_Sampler = nullptr;
  //   VkBuffer m_StagingBuffer = nullptr;
  //   VkDeviceMemory m_StagingBufferMemory = nullptr;
  VkImage m_Image = VK_NULL_HANDLE;
  VkImageView m_ImageView = VK_NULL_HANDLE;
  VkDeviceMemory m_Memory = VK_NULL_HANDLE;
  VkSampler m_Sampler = VK_NULL_HANDLE;
  VkBuffer m_StagingBuffer = VK_NULL_HANDLE;
  VkDeviceMemory m_StagingBufferMemory = VK_NULL_HANDLE;

  ImageFormat m_Format = ImageFormat::None;

  size_t m_AlignedSize = 0;

  VkDescriptorSet m_DescriptorSet = VK_NULL_HANDLE;

  std::string m_Filepath;
};

}  // namespace Weaver

#endif