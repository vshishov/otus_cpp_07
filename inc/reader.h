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
  static std::shared_ptr<Reader> Create(std::size_t a_szBlockSize, std::istream& a_isIn = std::cin)
  {
    auto ptr = std::shared_ptr<Reader>{ new Reader{a_szBlockSize, a_isIn} };
    return ptr;
  }

  void Exec()
  {
    m_vCommands.reserve(m_szBlockSize);

    std::string strLine;
    while ( std::getline(m_isIn, strLine) ) {
      if ( strLine == "{" ) {
        if (m_nBlockDepth == 0) {
          Flush();
        }
        m_nBlockDepth++;
      } 
      else if ( strLine == "}" ) {
        if (m_nBlockDepth == 0) {
          throw std::logic_error("unexpected symbol '}'");
        }
        m_nBlockDepth--;
      }
      else {
        m_vCommands.emplace_back(Command(strLine));
      }

      if (!m_nBlockDepth && m_vCommands.size() >= m_szBlockSize) {
        Flush();
      }
    }
  }

private:
  Reader(std::size_t a_szBlockSize, std::istream& a_isIn = std::cin) 
    : m_isIn(a_isIn)
    , m_szBlockSize(a_szBlockSize)
    , m_nBlockDepth(0)
  { }

  void Flush() 
  {
    if (m_vCommands.size() > 0) {
      Notify(m_vCommands);
      m_vCommands.clear();
    }
  }


private:
  std::istream& m_isIn;
  std::size_t m_szBlockSize;
  std::size_t m_nBlockDepth;
  Commands_t m_vCommands;  
};

} // Otus::
