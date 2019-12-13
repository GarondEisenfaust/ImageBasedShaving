#include "ibspch.h"
#include "Exports/General.h"

namespace Util
{
  bool IsImage(const fs::path& path)
  {
    const fs::path extension = path.extension();
    return (extension == ".jpg") || (extension == ".jpeg") || (extension == ".png") || (extension == ".ppm");
  }

  bool IsYml(const fs::path& path)
  {
    const fs::path extension = path.extension();
    return extension == ".yml";
  }

  void CreateDirectoryIfAbsent(const fs::path& path)
  {
    fs::path directoryPath = path;
    if (directoryPath.has_filename())
    {
      directoryPath = fs::path(path).remove_filename();
    }

    if (!fs::is_directory(directoryPath))
    {
      fs::create_directories(directoryPath);
    }
  }
}
