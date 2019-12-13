#pragma once

namespace Util
{
  bool IsImage(const std::filesystem::path& path);
  bool IsYml(const std::filesystem::path& path);
  void CreateDirectoryIfAbsent(const std::filesystem::path& path);
}
