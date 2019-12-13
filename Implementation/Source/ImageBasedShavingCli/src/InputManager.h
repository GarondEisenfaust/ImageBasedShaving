#pragma once
#include "ManagerBase.h"

class InputManager
{
public:
  InputManager(InputManager const&) = delete;
  void operator=(InputManager const&) = delete;
  InputManager(InputManager&&) = delete;
  InputManager& operator =(InputManager&&) = delete;
  ~InputManager() = default;
  static InputManager& Instance();
  int Manage(int argc, const char* const argv[]) const;
private:
  InputManager();
  ManagerBase::Props CreateProps(const cv::CommandLineParser& commandLineParser) const;
  std::string keys;
};
