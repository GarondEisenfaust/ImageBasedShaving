#include "ibspch.h"
#include "Exports/Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>

bool initialized = false;
std::shared_ptr<spdlog::logger> Log::logger;

void Log::Init()
{
  if (!initialized)
  {
    spdlog::set_pattern("%^[%T][%l] %v%$");
    logger = spdlog::stdout_color_mt("FACEEXTRACTION");
    logger->set_level(spdlog::level::trace);
  }
  initialized = true;
}

std::shared_ptr<spdlog::logger> Log::GetLogger()
{
  Init();
  return logger;
}
