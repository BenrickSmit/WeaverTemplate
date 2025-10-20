/**
 * @file Themes.cpp
 * @author B.G. Smit
 * @brief Implements various ImGui themes for the Weaver GUI Template.
 *
 * This file provides functions to apply different visual themes to the Dear ImGui
 * interface, allowing for customization of colors, styles, and layout.
 * @copyright Copyright (c) 2025
 */
#include "Themes.h"

#include <imgui.h>

/**
 * @brief Applies a Visual Studio-like dark theme with Gnome-style rounding.
 */
void VisualStudioGnomeTheme() {
  ImGuiStyle& style = ImGui::GetStyle();
  auto& colors = style.Colors;

  // Padding
  style.FramePadding = ImVec2(12, 8);
  style.ItemSpacing = ImVec2(12, 8);

  // Sizing
  style.GrabMinSize = 20.0f;

  // Rounding
  style.WindowRounding = 12.0f;
  style.ChildRounding = 12.0f;
  style.FrameRounding = 12.0f;
  style.GrabRounding = 12.0f;
  style.PopupRounding = 12.0f;
  style.ScrollbarRounding = 12.0f;
  style.TabRounding = 12.0f;
  style.AntiAliasedFill = true;

  // Visual Studio Style Colors
  colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
  colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
  colors[ImGuiCol_WindowBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
  colors[ImGuiCol_ChildBg] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
  colors[ImGuiCol_PopupBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
  colors[ImGuiCol_Border] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
  colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
  colors[ImGuiCol_TitleBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
  colors[ImGuiCol_MenuBarBg] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
  colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.5f, 1.0f, 1.00f);
  colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 0.5f, 1.0f, 1.00f);
  colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 0.5f, 1.0f, 1.00f);
  colors[ImGuiCol_Button] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
  colors[ImGuiCol_Header] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
  colors[ImGuiCol_Separator] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
  colors[ImGuiCol_ResizeGrip] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
  colors[ImGuiCol_Tab] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
  colors[ImGuiCol_TabUnfocused] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
  colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.5f, 1.0f, 0.50f);
  colors[ImGuiCol_NavHighlight] = ImVec4(0.00f, 0.5f, 1.0f, 1.00f);
  colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
  colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
  colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

  // Lighter blue accent for hovering
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.2f, 0.35f, 0.6f, 1.0f);
  colors[ImGuiCol_ButtonHovered] = ImVec4(0.2f, 0.35f, 0.6f, 1.0f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(0.2f, 0.35f, 0.6f, 1.0f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.2f, 0.35f, 0.6f, 1.0f);
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.2f, 0.35f, 0.6f, 1.0f);
  colors[ImGuiCol_TabHovered] = ImVec4(0.2f, 0.35f, 0.6f, 1.0f);
  colors[ImGuiCol_SeparatorHovered] = ImVec4(0.2f, 0.35f, 0.6f, 1.0f);

  // Lighter blue accent for active state
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.15f, 0.3f, 0.55f, 1.0f);
  colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.3f, 0.55f, 1.0f);
  colors[ImGuiCol_HeaderActive] = ImVec4(0.15f, 0.3f, 0.55f, 1.0f);
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.15f, 0.3f, 0.55f, 1.0f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(0.15f, 0.3f, 0.55f, 1.0f);
  colors[ImGuiCol_TabActive] = ImVec4(0.15f, 0.3f, 0.55f, 1.0f);
  colors[ImGuiCol_SeparatorActive] = ImVec4(0.15f, 0.3f, 0.55f, 1.0f);

  // Plot and graph colors
  colors[ImGuiCol_PlotLines] = ImVec4(0.25f, 0.55f, 0.90f, 1.0f);
  colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.35f, 0.65f, 0.95f, 1.0f);
  colors[ImGuiCol_PlotHistogram] = ImVec4(0.25f, 0.55f, 0.90f, 1.0f);
  colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.35f, 0.65f, 0.95f, 1.0f);

  // Table colors
  colors[ImGuiCol_TableHeaderBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
  colors[ImGuiCol_TableBorderStrong] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
  colors[ImGuiCol_TableBorderLight] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
  colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);

  // Drag and drop
  colors[ImGuiCol_DragDropTarget] = ImVec4(0.00f, 0.5f, 1.0f, 1.00f);
}

/**
 * @brief Applies a dark theme with Material Design influences.
 */
void DarkMaterialTheme() {
  ImGuiStyle& style = ImGui::GetStyle();

  style.Alpha = 1.0f;
  style.DisabledAlpha = 0.4000000059604645f;
  style.WindowPadding = ImVec2(12.0f, 12.0f);
  style.WindowRounding = 9.0f;
  style.WindowBorderSize = 1.0f;
  style.WindowMinSize = ImVec2(32.0f, 32.0f);
  style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
  style.WindowMenuButtonPosition = ImGuiDir_Left;
  style.ChildRounding = 9.0f;
  style.ChildBorderSize = 1.0f;
  style.PopupRounding = 12.0f;
  style.PopupBorderSize = 1.0f;
  style.FramePadding = ImVec2(10.0f, 7.0f);
  style.FrameRounding = 9.0f;
  style.FrameBorderSize = 1.0f;
  style.ItemSpacing = ImVec2(12.0f, 10.0f);
  style.ItemInnerSpacing = ImVec2(12.0f, 10.0f);
  style.CellPadding = ImVec2(9.199999809265137f, 10.0f);
  style.IndentSpacing = 21.0f;
  style.ColumnsMinSpacing = 7.0f;
  style.ScrollbarSize = 20.0f;
  style.ScrollbarRounding = 7.0f;
  style.GrabMinSize = 20.0f;
  style.GrabRounding = 20.0f;
  style.TabRounding = 7.0f;
  style.TabBorderSize = 0.0f;
  style.TabMinWidthForCloseButton = 0.0f;
  style.ColorButtonPosition = ImGuiDir_Left;
  style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
  style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

  style.Colors[ImGuiCol_Text] =
      ImVec4(0.8313725590705872f, 0.8470588326454163f, 0.8784313797950745f, 1.0f);
  style.Colors[ImGuiCol_TextDisabled] =
      ImVec4(0.8313725590705872f, 0.8470588326454163f, 0.8784313797950745f, 0.501960813999176f);
  style.Colors[ImGuiCol_WindowBg] =
      ImVec4(0.1725490242242813f, 0.1921568661928177f, 0.2352941185235977f, 1.0f);
  style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.1587982773780823f);
  style.Colors[ImGuiCol_PopupBg] =
      ImVec4(0.1725490242242813f, 0.1921568661928177f, 0.2352941185235977f, 1.0f);
  style.Colors[ImGuiCol_Border] =
      ImVec4(0.2039215713739395f, 0.2313725501298904f, 0.2823529541492462f, 1.0f);
  style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
  style.Colors[ImGuiCol_FrameBg] =
      ImVec4(0.105882354080677f, 0.1137254908680916f, 0.1372549086809158f, 0.501960813999176f);
  style.Colors[ImGuiCol_FrameBgHovered] =
      ImVec4(0.3098039329051971f, 0.6235294342041016f, 0.9333333373069763f, 0.250980406999588f);
  style.Colors[ImGuiCol_FrameBgActive] =
      ImVec4(0.2794120311737061f, 0.5642462968826294f, 0.8454935550689697f, 0.8412017226219177f);
  style.Colors[ImGuiCol_TitleBg] =
      ImVec4(0.105882354080677f, 0.1137254908680916f, 0.1372549086809158f, 1.0f);
  style.Colors[ImGuiCol_TitleBgActive] =
      ImVec4(0.105882354080677f, 0.1137254908680916f, 0.1372549086809158f, 1.0f);
  style.Colors[ImGuiCol_TitleBgCollapsed] =
      ImVec4(0.105882354080677f, 0.1137254908680916f, 0.1372549086809158f, 1.0f);
  style.Colors[ImGuiCol_MenuBarBg] =
      ImVec4(0.105882354080677f, 0.1137254908680916f, 0.1372549086809158f, 1.0f);
  style.Colors[ImGuiCol_ScrollbarBg] =
      ImVec4(0.01960784383118153f, 0.01960784383118153f, 0.01960784383118153f, 0.0f);
  style.Colors[ImGuiCol_ScrollbarGrab] =
      ImVec4(0.5333333611488342f, 0.5333333611488342f, 0.5333333611488342f, 1.0f);
  style.Colors[ImGuiCol_ScrollbarGrabHovered] =
      ImVec4(0.3333333432674408f, 0.3333333432674408f, 0.3333333432674408f, 1.0f);
  style.Colors[ImGuiCol_ScrollbarGrabActive] =
      ImVec4(0.6000000238418579f, 0.6000000238418579f, 0.6000000238418579f, 1.0f);
  style.Colors[ImGuiCol_CheckMark] =
      ImVec4(0.3098039329051971f, 0.6235294342041016f, 0.9333333373069763f, 1.0f);
  style.Colors[ImGuiCol_SliderGrab] =
      ImVec4(0.239215686917305f, 0.5215686559677124f, 0.8784313797950745f, 1.0f);
  style.Colors[ImGuiCol_SliderGrabActive] =
      ImVec4(0.3098039329051971f, 0.6235294342041016f, 0.9333333373069763f, 1.0f);
  style.Colors[ImGuiCol_Button] =
      ImVec4(0.1529411822557449f, 0.1725490242242813f, 0.2117647081613541f, 0.501960813999176f);
  style.Colors[ImGuiCol_ButtonHovered] =
      ImVec4(0.1529411822557449f, 0.1725490242242813f, 0.2117647081613541f, 1.0f);
  style.Colors[ImGuiCol_ButtonActive] =
      ImVec4(0.3098039329051971f, 0.6235294342041016f, 0.9333333373069763f, 1.0f);
  style.Colors[ImGuiCol_Header] =
      ImVec4(0.1529411822557449f, 0.1725490242242813f, 0.2117647081613541f, 1.0f);
  style.Colors[ImGuiCol_HeaderHovered] =
      ImVec4(0.3098039329051971f, 0.6235294342041016f, 0.9333333373069763f, 0.250980406999588f);
  style.Colors[ImGuiCol_HeaderActive] =
      ImVec4(0.3098039329051971f, 0.6235294342041016f, 0.9333333373069763f, 1.0f);
  style.Colors[ImGuiCol_Separator] =
      ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 0.5f);
  style.Colors[ImGuiCol_SeparatorHovered] =
      ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 0.7799999713897705f);
  style.Colors[ImGuiCol_SeparatorActive] =
      ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 1.0f);
  style.Colors[ImGuiCol_ResizeGrip] =
      ImVec4(0.105882354080677f, 0.1137254908680916f, 0.1372549086809158f, 1.0f);
  style.Colors[ImGuiCol_ResizeGripHovered] =
      ImVec4(0.3098039329051971f, 0.6235294342041016f, 0.9333333373069763f, 0.250980406999588f);
  style.Colors[ImGuiCol_ResizeGripActive] =
      ImVec4(0.3098039329051971f, 0.6235294342041016f, 0.9333333373069763f, 1.0f);
  style.Colors[ImGuiCol_Tab] =
      ImVec4(0.1529411822557449f, 0.1725490242242813f, 0.2117647081613541f, 1.0f);
  style.Colors[ImGuiCol_TabHovered] =
      ImVec4(0.3098039329051971f, 0.6235294342041016f, 0.9333333373069763f, 0.250980406999588f);
  style.Colors[ImGuiCol_TabActive] =
      ImVec4(0.3098039329051971f, 0.6235294342041016f, 0.9333333373069763f, 1.0f);
  style.Colors[ImGuiCol_TabUnfocused] =
      ImVec4(0.1529411822557449f, 0.1725490242242813f, 0.2117647081613541f, 1.0f);
  style.Colors[ImGuiCol_TabUnfocusedActive] =
      ImVec4(0.3098039329051971f, 0.6235294342041016f, 0.9333333373069763f, 1.0f);
  style.Colors[ImGuiCol_PlotLines] =
      ImVec4(0.6078431606292725f, 0.6078431606292725f, 0.6078431606292725f, 1.0f);
  style.Colors[ImGuiCol_PlotLinesHovered] =
      ImVec4(1.0f, 0.4274509847164154f, 0.3490196168422699f, 1.0f);
  style.Colors[ImGuiCol_PlotHistogram] =
      ImVec4(0.8980392217636108f, 0.6980392336845398f, 0.0f, 1.0f);
  style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6000000238418579f, 0.0f, 1.0f);
  style.Colors[ImGuiCol_TableHeaderBg] =
      ImVec4(0.105882354080677f, 0.1137254908680916f, 0.1372549086809158f, 1.0f);
  style.Colors[ImGuiCol_TableBorderStrong] =
      ImVec4(0.2039215713739395f, 0.2313725501298904f, 0.2823529541492462f, 1.0f);
  style.Colors[ImGuiCol_TableBorderLight] =
      ImVec4(0.2039215713739395f, 0.2313725501298904f, 0.2823529541492462f, 0.5021458864212036f);
  style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
  style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.03862661123275757f);
  style.Colors[ImGuiCol_TextSelectedBg] =
      ImVec4(0.2039215713739395f, 0.2313725501298904f, 0.2823529541492462f, 1.0f);
  style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
  style.Colors[ImGuiCol_NavHighlight] =
      ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
  style.Colors[ImGuiCol_NavWindowingHighlight] =
      ImVec4(0.2039215713739395f, 0.2313725501298904f, 0.2823529541492462f, 0.7529411911964417f);
  style.Colors[ImGuiCol_NavWindowingDimBg] =
      ImVec4(0.105882354080677f, 0.1137254908680916f, 0.1372549086809158f, 0.7529411911964417f);
  style.Colors[ImGuiCol_ModalWindowDimBg] =
      ImVec4(0.105882354080677f, 0.1137254908680916f, 0.1372549086809158f, 0.7529411911964417f);
}