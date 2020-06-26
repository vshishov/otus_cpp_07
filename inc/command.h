#pragma once

#include <chrono>
#include <string>
#include <vector>

namespace Otus {

class Command
{
public:
  Command(const std::string& a_strName)
    : m_strName{a_strName}
    , m_TimeStamp(std::chrono::system_clock::now())
  { }

  std::string GetName() const
  {
    return m_strName;
  }

  std::string GetTimeStamp() const 
  {
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(
      m_TimeStamp.time_since_epoch()).count();
    
    return  std::to_string(seconds);
  }

private:
  std::string m_strName;
  std::chrono::system_clock::time_point m_TimeStamp;
};

using Commands_t = std::vector<Command>;

} // Otus::
