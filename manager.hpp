#ifndef MANAGER_HPP
#define MANAGER_HPP

#include "object.hpp"

class Manager {
public:
  Manager() {}
  virtual void register_object(Object *object) = 0;
  virtual void deregister_object(Object * object) = 0;
  virtual void manage() = 0;
  virtual ~Manager() {}
protected:
  
};

#endif
