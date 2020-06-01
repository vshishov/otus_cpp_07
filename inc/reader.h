#pragma once

#include "observer.h"

namespace Otus {

class Reader : public BaseObservable
{
 public: 
  void Set()
  {
    Notify();
  }
};

} // Otus::