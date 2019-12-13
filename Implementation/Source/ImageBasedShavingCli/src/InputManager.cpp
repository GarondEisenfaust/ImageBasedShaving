#include "ibspch.h"
#include "InputManager.h"
#include "FaceExtractionManager.h"
#include "ManagerFactory.h"
#include "ManagerBase.h"
#include <opencv2/core/utility.hpp>

InputManager& InputManager::Instance()
{
  static InputManager instance;
  return instance;
}

int InputManager::Manage(int argc, const char* const argv[]) const
{
  const cv::CommandLineParser commandLineParser(argc, argv, keys);
  if (commandLineParser.has("help"))
  {
    commandLineParser.printMessage();
    return 0;
  }
  const auto props = CreateProps(commandLineParser);
  const auto manager = ManagerFactory::Instance().Create(props.mode);
  return manager->Manage(props);
}

ManagerBase::Props InputManager::CreateProps(const cv::CommandLineParser& commandLineParser) const
{
  std::string mode;
  if (commandLineParser.has("mode"))
  {
    mode = commandLineParser.get<std::string>("mode");
  }

  std::string source;
  if (commandLineParser.has("source"))
  {
    source = commandLineParser.get<std::string>("source");
  }

  std::string destination = "./output.png";
  if (commandLineParser.has("destination"))
  {
    destination = commandLineParser.get<std::string>("destination");
  }

  std::string beardedFace;
  if (commandLineParser.has("beardedFace"))
  {
    beardedFace = commandLineParser.get<std::string>("beardedFace");
  }

  std::string algorithm;
  if (commandLineParser.has("algorithm"))
  {
    algorithm = commandLineParser.get<std::string>("algorithm");
  }

  bool extract = false;
  if (commandLineParser.has("extract"))
  {
    extract = true;
  }

  bool extractBeard = false;
  if (commandLineParser.has("extractBeard"))
  {
    extractBeard = true;
  }

  unsigned int index = 0;
  if (commandLineParser.has("index"))
  {
    index = commandLineParser.get<unsigned int>("index");
  }

  return ManagerBase::Props(mode, source, destination, beardedFace, algorithm, extract, extractBeard, index);
}

InputManager::InputManager()
{
  keys = "{help h usage ?     |          |         }"
    "{mode m             |          | specifies which mode the program runs: shaving, adding, pc}"
    "{algorithm a        |naive     | reconstruction algorithm that is used}"
    "{source s           |          | path to the image}"
    "{destination dest d |output.png| output destination}"
    "{extract e          |          | extract face  from image}"
    "{extractBeard eb    |          | extract the beard}"
    "{add                |          | if set a beard from the beard mat is added to the face}"
    "{beardedFace bf     |          | specifies from which face to extract the beard}"
    "{index              |          | specifies which principal component to add}";
}
