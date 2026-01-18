#include "console_window.h"
#include "console_window_sink.h"
#include "yuggoth/core/logger/logger.h"
#include "imgui.h"

namespace Yuggoth {

ConsoleWindow::ConsoleWindow() {
  std::shared_ptr<ConsoleWindowSink> console_window_sink = std::make_shared<ConsoleWindowSink>(this);
  std::shared_ptr<spdlog::logger> editor_logger = std::make_shared<spdlog::logger>("EditorConsole", console_window_sink);

  Logger::RegisterLogger(editor_logger);
}

void ConsoleWindow::PushMessage(const ConsoleWindowMessage &message) {
  messages_.push_back(message);
  if (messages_.size() > 256) {
    messages_.erase(messages_.begin());
  }
}

void ConsoleWindow::OnImGui() {
  ImGui::Begin("Console");

  for (auto &message : messages_) {
    ImGui::Text("%s", message.message_.c_str());
  }

  ImGui::End();
}

} // namespace Yuggoth