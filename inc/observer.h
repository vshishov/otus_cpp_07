#pragma once

#include <iostream>
#include <memory>
#include <list>

namespace Otus {

class IObserver 
{
public:
  virtual ~IObserver() = default;

  virtual void Update() = 0;
};

class IObservable 
{
public:
  virtual ~IObservable() = default;

  virtual void Subscribe(const std::shared_ptr<IObserver>& a_ptrObserver) = 0;
};

class BaseObservable : public IObservable 
{
public:
  void Subscribe(const std::shared_ptr<IObserver>& a_ptrObserver) override
  {
    m_subscribers.emplace_back(a_ptrObserver);
  }

  void Notify() {
    auto iter = m_subscribers.begin();
    while (iter != m_subscribers.end()) {
      auto ptr = iter->lock();
      if (ptr) {
          ptr->Update();
          ++iter;
      } else {
          m_subscribers.erase(iter++);
      }
    }
  }

private:
  std::list< std::weak_ptr<IObserver> > m_subscribers;
};

} // Otus::