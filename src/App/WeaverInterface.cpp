/**
 * @file WeaverInterface.cpp
 * @author B.G. Smit
 * @brief This file implements the main application interface for the Weaver GUI Template.
 *
 * This template provides a foundational structure for building C++ GUI applications
 * using SDL2 and Dear ImGui. It demonstrates a clean, layer-based architecture
 * and integrates essential features like logging, input handling, and UI theming.
 * The application's core logic resides in the 'Core' module, which can be
 * easily extended and integrated into the 'App' module's user interface.
 *
 * @copyright Copyright (c) 2025
 */

#include <imgui.h>

#include <cmath>    // For sin
#include <cstdlib>  // For rand
#include <glm/glm.hpp>
#include <string>

#include "../Core/Canvas.h"
#include "../Core/EntryPoint.h"
#include "../Core/IconsMaterialDesign.h"
#include "../Core/Image.h"
#include "../Core/Log.h"
#include "../Core/Timer.h"
#include "../Core/Common/Settings.h"
#include "glm/gtc/type_ptr.hpp"

// Temporary, for demo purposes
static bool s_show_demo_window = false;

/**
 * @class ExampleLayer
 * @brief An example application layer demonstrating various UI elements and features.
 */
class ExampleLayer : public Weaver::Layer {
 public:
  /**
   * @brief Constructs a new ExampleLayer object.
   */
  ExampleLayer() {
    WEAVER_LOG_INFO("ExampleLayer created!");
  }

  /**
   * @brief Called every frame to update the layer's state.
   * @param delta_time The time step since the last frame.
   */
  virtual void OnUpdate(float delta_time) override {}

  /**
   * @brief Called every frame to render the layer's UI.
   */
  virtual void OnUIRender() override;

  /**
   * @brief Called when the window is resized.
   * @param width The new width of the window.
   * @param height The new height of the window.
   */
  virtual void OnResize(uint32_t width, uint32_t height) override {}

  /**
   * @brief Called when the window is minimized.
   */
  virtual void OnMinimize() override {
    m_continuous_rendering_before_state_change = m_continuous_rendering;
    m_continuous_rendering = false;
  }

  /**
   * @brief Called when the window is maximized.
   */
  virtual void OnMaximize() override {
    m_continuous_rendering_before_state_change = m_continuous_rendering;
    m_continuous_rendering = false;
  }

  /**
   * @brief Called when the window is restored.
   */
  virtual void OnRestored() override {
    m_continuous_rendering = m_continuous_rendering_before_state_change;
  }

 private:
  bool m_continuous_rendering = false;
  bool m_continuous_rendering_before_state_change = false;

  uint32_t m_viewport_width = Weaver::Settings::Window::DEFAULT_WIDTH, m_viewport_height = Weaver::Settings::Window::DEFAULT_HEIGHT;

  float m_last_render_time = 0.0f;
};

/**
 * @brief Renders the UI for the ExampleLayer.
 */
void ExampleLayer::OnUIRender() {
  WEAVER_LOG_INFO("Started: OnUIRender");

  // Main control panel for generic application settings and demonstrations.
  ImGui::SetNextWindowSizeConstraints(ImVec2(Weaver::Settings::UI::CONTROL_PANEL_MIN_WIDTH, Weaver::Settings::UI::CONTROL_PANEL_MIN_HEIGHT), ImVec2(FLT_MAX, FLT_MAX));
  ImGui::Begin("Control Panel");

  ImGui::Text("Application Statistics");
  ImGui::Separator();
  ImGui::Text("Frame Rate: %.1f FPS", ImGui::GetIO().Framerate);
  ImGui::Text("Viewport Size: %d x %d", m_viewport_width, m_viewport_height);

  ImGui::Spacing();
  ImGui::Text("Generic Controls");
  ImGui::Separator();

  // Example of a simple button
  if (ImGui::Button("Perform Action")) {
    WEAVER_LOG_INFO("Generic action performed!");
    // TODO: Implement a generic action here, e.g., trigger a Core function.
  }

  // Example of a checkbox
  static bool s_toggle_feature = false;
  if (ImGui::Checkbox("Toggle Feature", &s_toggle_feature)) {
    WEAVER_LOG_INFO("Feature toggled: %s", s_toggle_feature ? "On" : "Off");
    // TODO: Implement feature toggle logic.
  }

  // Example of a slider
  static float s_value_slider = 0.0f;
  if (ImGui::SliderFloat("Value Slider", &s_value_slider, 0.0f, 100.0f)) {
    WEAVER_LOG_INFO("Slider value changed: %.2f", s_value_slider);
    // TODO: Use slider value to control some Core parameter.
  }

  // Example of a text input
  static char s_text_input_buffer[Weaver::Settings::UI::TEXT_INPUT_BUFFER_SIZE] = "Default Text";
  if (ImGui::InputText("Text Input", s_text_input_buffer, sizeof(s_text_input_buffer))) {
    WEAVER_LOG_INFO("Text input changed: %s", s_text_input_buffer);
    // TODO: Process text input.
  }

  ImGui::Spacing();
  ImGui::Text("Data Visualization");
  ImGui::Separator();

  // Example of a simple plot (e.g., frame times, or some simulated data)
  static float s_history[Weaver::Settings::UI::FRAME_RATE_HISTORY_SIZE] = {0};
  static int s_history_idx = 0;
  s_history[s_history_idx] = ImGui::GetIO().Framerate;  // Example: plot framerate
  s_history_idx = (s_history_idx + 1) % IM_ARRAYSIZE(s_history);

  ImGui::PlotLines("Framerate History",
      s_history,
      IM_ARRAYSIZE(s_history),
      s_history_idx,
      "FPS",
      0.0f,
      ImGui::GetIO().Framerate * Weaver::Settings::UI::FRAME_RATE_PLOT_MULTIPLIER,
      ImVec2(0, Weaver::Settings::UI::PLOT_HEIGHT));

  ImGui::End();  // End Control Panel

  // New window for demonstrating tables and calculations
  ImGui::Begin("Data and Calculations");

  ImGui::Text("Table Demonstration");
  ImGui::Separator();

  // Example table
  if (ImGui::BeginTable("MyTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
    ImGui::TableSetupColumn("Name");
    ImGui::TableSetupColumn("Value");
    ImGui::TableSetupColumn("Status");
    ImGui::TableHeadersRow();

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("Item A");
    ImGui::TableSetColumnIndex(1);
    ImGui::Text("%.2f", s_value_slider * 0.5f);
    ImGui::TableSetColumnIndex(2);
    ImGui::Text("Active");

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("Item B");
    ImGui::TableSetColumnIndex(1);
    ImGui::Text("%.2f", s_value_slider * 1.2f);
    ImGui::TableSetColumnIndex(2);
    ImGui::Text("Inactive");

    ImGui::EndTable();
  }

  ImGui::Spacing();
  ImGui::Text("Calculation Examples");
  ImGui::Separator();

  static float s_input_a = 10.0f;
  static float s_input_b = 5.0f;
  ImGui::InputFloat("Input A", &s_input_a);
  ImGui::InputFloat("Input B", &s_input_b);

  float sum = s_input_a + s_input_b;
  float product = s_input_a * s_input_b;

  ImGui::Text("Sum: %.2f", sum);
  ImGui::Text("Product: %.2f", product);

  ImGui::End();  // End Data and Calculations

  // New window for dynamic graphs
  ImGui::Begin("Dynamic Graphs");

  // Line graph: Sine wave
  ImGui::Text("Sine Wave (Line Graph)");
  ImGui::Separator();
  static float s_sine_values[Weaver::Settings::UI::FRAME_RATE_HISTORY_SIZE];
  static int s_sine_offset = 0;
  static double s_time = 0.0;
  s_time += ImGui::GetIO().DeltaTime;
  for (int i = 0; i < IM_ARRAYSIZE(s_sine_values); i++) {
    s_sine_values[i] = (float)(sin(s_time + i * 0.1f) * 0.5f + 0.5f);
  }
  ImGui::PlotLines("##SinePlot",
      s_sine_values,
      IM_ARRAYSIZE(s_sine_values),
      s_sine_offset,
      "",
      0.0f,
      1.0f,
      ImVec2(0, Weaver::Settings::UI::PLOT_HEIGHT));

  ImGui::Spacing();

  // Bar chart: Random values with fixed window
  ImGui::Text("Random Values (Bar Chart)");
  ImGui::Separator();
  static float s_bar_values[Weaver::Settings::UI::BAR_CHART_ITEM_COUNT] = {0};  // 10 items
  static int s_bar_offset = 0;
  static float s_bar_time_accumulator = 0.0f;
  s_bar_time_accumulator += ImGui::GetIO().DeltaTime;
  if (s_bar_time_accumulator > Weaver::Settings::UI::BAR_CHART_UPDATE_INTERVAL) {  // Update every 0.5 seconds
    s_bar_time_accumulator = 0.0f;
    // Shift values to the left
    for (int i = 0; i < IM_ARRAYSIZE(s_bar_values) - 1; i++) {
      s_bar_values[i] = s_bar_values[i + 1];
    }
    // Add new random value at the end
    s_bar_values[IM_ARRAYSIZE(s_bar_values) - 1] = (float)rand() / (float)RAND_MAX;
  }
  ImGui::PlotHistogram("##BarPlot",
      s_bar_values,
      IM_ARRAYSIZE(s_bar_values),
      s_bar_offset,
      "",
      0.0f,
      1.0f,
      ImVec2(0, Weaver::Settings::UI::PLOT_HEIGHT));

  ImGui::End();  // End Dynamic Graphs

  if (s_show_demo_window) {
    ImGui::ShowDemoWindow(&s_show_demo_window);
  }

  WEAVER_LOG_INFO("Finished: OnUIRender");
}

/**
 * @brief Creates and configures the main application canvas.
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments.
 * @return A pointer to the created Canvas object.
 */
Weaver::Canvas* Weaver::CreateCanvas(int argc, char** argv) {
  // TODO: Add OpenTelemetry span for application creation here

  Weaver::CanvasSpecification spec;
  spec.Name = "Weaver Example";

  Weaver::Canvas* app = new Weaver::Canvas(spec);
  app->PushLayer<ExampleLayer>();
  app->SetMenubarCallback([app]() {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Exit")) {
        app->Close();
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("View")) {
      if (ImGui::MenuItem("Show Demo Window")) {
        s_show_demo_window = true;
      }
      ImGui::EndMenu();
    }

    // Get window width
    float width = ImGui::GetWindowWidth();
    // Spacing to push buttons to the right
    ImGui::SetCursorPosX(width - Weaver::Settings::UI::MENUBAR_BUTTON_OFFSET);

    // Add a separator
    ImGui::Separator();
    {
      ImGui::PushStyleColor(
          ImGuiCol_HeaderHovered, Weaver::Settings::UI::MINIMIZE_BUTTON_HOVER_COLOR);  // Greenish on hover
      ImGui::PushStyleColor(
          ImGuiCol_HeaderActive, Weaver::Settings::UI::MINIMIZE_BUTTON_ACTIVE_COLOR);  // Darker greenish on click
      if (ImGui::MenuItem(ICON_MD_MINIMIZE)) {
        app->Minimize();
      }
      ImGui::PopStyleColor(2);
    }
    {
      ImGui::PushStyleColor(
          ImGuiCol_HeaderHovered, Weaver::Settings::UI::MAXIMIZE_BUTTON_HOVER_COLOR);  // Blueish on hover
      ImGui::PushStyleColor(
          ImGuiCol_HeaderActive, Weaver::Settings::UI::MAXIMIZE_BUTTON_ACTIVE_COLOR);  // Darker blueish on click
      if (ImGui::MenuItem(ICON_MD_FULLSCREEN)) {
        app->ToggleMaximize();
      }
      ImGui::PopStyleColor(2);
    }
    {
      ImGui::PushStyleColor(
          ImGuiCol_HeaderHovered, Weaver::Settings::UI::CLOSE_BUTTON_HOVER_COLOR);  // Reddish on hover
      ImGui::PushStyleColor(
          ImGuiCol_HeaderActive, Weaver::Settings::UI::CLOSE_BUTTON_ACTIVE_COLOR);  // Darker reddish on click
      if (ImGui::MenuItem(ICON_MD_CLOSE)) {
        app->Close();
      }
      ImGui::PopStyleColor(2);
    }
  });
  return app;
}