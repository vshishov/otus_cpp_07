#pragma once

#include "command.h"
#include "observer.h"
#include "reader.h"

#include <memory>
#include <fstream>

namespace Otus {

class Logger : public IObserver<Commands_t>, public std::enable_shared_from_this<Logger>
{
public:
  static std::shared_ptr<Logger> Create(std::shared_ptr<Reader>& a_pReader)
  {
    auto ptr = std::shared_ptr<Logger>{new Logger{}};
    ptr->SetReader(a_pReader);
    return ptr;
  }

  void Update(const Commands_t& a_Commands) override 
  {
    std::string strLogName{"bulk" + a_Commands.begin()->GetTimeStamp() + ".log"};
    std::ofstream log(strLogName, std::ios::out);

    for (auto command_it = a_Commands.begin(); command_it != a_Commands.end(); ++command_it) {
      if (command_it != a_Commands.begin()) {
        log << ", ";
      }
      log << command_it->GetName();
      
    }
    log << std::endl;
    log.close();
  }

private:
  Logger() {}

  void SetReader(std::shared_ptr<Reader>& a_pReader)
  {
    m_pReader = a_pReader;
    auto ptrReader = m_pReader.lock();
    if (ptrReader) {
      ptrReader->Subscribe(shared_from_this());
    }
  }

private:
  std::weak_ptr<Reader> m_pReader;
};

} // Otus::
