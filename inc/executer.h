#pragma once

#include "command.h"
#include "observer.h"
#include "reader.h"

#include <memory>

namespace Otus {

class Excuter : public IObserver<Commands_t>, public std::enable_shared_from_this<Excuter>
{
public:
  static std::shared_ptr<Excuter> Create(std::shared_ptr<Reader>& a_pReader)
  {
    auto ptr = std::shared_ptr<Excuter>{new Excuter{}};
    ptr->SetReader(a_pReader);
    return ptr;
  }

  void Update(const Commands_t& a_Commands) override 
  {
    std::cout << "bulk: ";
    for (auto command_it = a_Commands.begin(); command_it != a_Commands.cend(); ++command_it) {
      if (command_it != a_Commands.begin()) {
        std::cout << ", ";
      }
      std::cout << command_it->GetName();
      
    }
    std::cout << std::endl;
  }

private:
  Excuter() {}

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
