#pragma once
#include <memory>
#include <spdlog/spdlog.h>

class Log
{
private:
  static std::shared_ptr<spdlog::logger> logger;
  static void Init();
public:
  static std::shared_ptr<spdlog::logger> GetLogger();
};
