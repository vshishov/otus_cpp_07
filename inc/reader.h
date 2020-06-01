#pragma once

#include "observer.h"
#include "command.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <string>

namespace Otus {

class Reader : public BaseObservable<Commands_t>
{
public: 
  Reader(std::size_t a_szBlockSize) 
    : m_szBlockSize(a_szBlockSize)
    , m_nBlockDepth(0)
  { }

  void Exec()
  {
    m_vCommands.reserve(m_szBlockSize);

    std::string strLine;
    while ( std::getline(std::cin, strLine) ) {
      if ( strLine == "{" ) {
        m_nBlockDepth++;
      } 
      else if ( strLine == "}" ) {
        m_nBlockDepth--;
      }
      else {
        m_vCommands.emplace_back(Command(strLine));
      }

      if (!m_nBlockDepth && m_vCommands.size() >= m_szBlockSize) {
        Notify(m_vCommands);
        m_vCommands.clear();
      }
    }
  }

private:
  std::size_t m_szBlockSize;
  std::size_t m_nBlockDepth;
  Commands_t m_vCommands;  
};

} // Otus::
