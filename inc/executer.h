#pragma once

#include "observer.h"
#include "reader.h"

#include <memory>

namespace Otus {

class Excuter : public IObserver, public std::enable_shared_from_this<Excuter>
{
  static std::shared_ptr<Excuter> create(Reader* a_reader)
  {
    auto ptr = std::shared_ptr<Excuter>{new Excuter{}};
    ptr->SetReader(a_reader);
    return ptr;
  }

  void Update() override 
  {

  }

private:
  Excuter() {}

  void SetReader(Reader* a_reader)
  {
    m_reader = a_reader;
    m_reader->Subscribe(shared_from_this());
  }

private:
  Reader* m_reader;
};

} // Otus::