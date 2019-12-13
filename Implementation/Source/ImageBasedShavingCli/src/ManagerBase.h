#pragma once
#define REGISTERMANAGERIMPL(classname,name) const ManagerCreatorImpl<classname> name(#name);

class ManagerBase
{
public:
  struct Props
  {
    Props(const std::string& mode, const std::string& source, const std::string& destination,
          const std::string& beardedFace,
          const std::string& algorithm, bool extract, bool extractBeard, unsigned int index);
    const std::string mode;
    const std::string source;
    const std::string destination;
    const std::string algorithm;
    const std::string beardedFace;
    const bool extract;
    const bool extractBeard;
    const unsigned int index;
  };

  ManagerBase() = default;
  virtual ~ManagerBase() = default;
  ManagerBase(ManagerBase&& other) noexcept = default;
  ManagerBase(const ManagerBase& other) = default;
  ManagerBase& operator=(ManagerBase const& other) = default;
  ManagerBase& operator=(ManagerBase&& other) = default;
  virtual int Manage(const Props& props) const;
};
