#ifndef YUGGOTH_EDITOR_TOOLS_H
#define YUGGOTH_EDITOR_TOOLS_H

#include <filesystem>
#include <optional>

namespace Yuggoth {

std::optional<std::filesystem::path> DrawFileDialog(std::string_view name);

} // namespace Yuggoth

#endif // YUGGOTH_EDITOR_TOOLS_H