#ifndef TYPEACCEPTOR_HH
#define TYPEACCEPTOR_HH

#include "enumType.hh" // only for _ByteSwap_

class typeAcceptor {

public:

  virtual ~typeAcceptor(){};
  virtual void pass(char* name, int& i, unsigned int len) = 0;  
  virtual void pass(char* name, long& i, unsigned int len) = 0;  
  virtual void pass(char* name, float& i, unsigned int len) = 0;
  virtual void pass(char* name, double& i, unsigned int len) = 0;
  virtual void pass(char* name, char* i, unsigned int len) = 0;
  virtual void pass(char* name, int* i, unsigned int len) = 0;  
  virtual void pass(char* name, long* i, unsigned int len) = 0;  
  virtual void pass(char* name, float* i, unsigned int len) = 0;
  virtual void pass(char* name, double* i, unsigned int len) = 0;

};

#endif







