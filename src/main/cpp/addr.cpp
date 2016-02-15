#include <addr.h>
#include <cstdlib>
#include <TObject.h>
#include <iostream>
  
addr_t Pointer_getReference( addr_t addr ) {
  void* ptr = reinterpret_cast<void*>(addr);
  void* ref = &ptr;
  return reinterpret_cast<addr_t>(ref);
};

addr_t Pointer_allocate( long size ) {
  return reinterpret_cast<addr_t>(malloc(size));
};

void Pointer_free( addr_t addr ) {
  free(reinterpret_cast<void*>(addr));
};
addr_t Pointer_getPosition( addr_t addr, long long pos ) {
  return  addr + pos;
};

int Pointer_getIntValue( addr_t addr ) {
  return *reinterpret_cast<int*>(addr);
};

long Pointer_getLongValue( addr_t addr ) {
  return *reinterpret_cast<long*>(addr);
};
float Pointer_getFloatValue( addr_t addr ) {
  return *reinterpret_cast<float*>(addr);
};
double Pointer_getDoubleValue( addr_t addr ) {
  return *reinterpret_cast<double*>(addr);
};

void Pointer_setIntValue( addr_t addr, int value ) {
  (*reinterpret_cast<int*>(addr)) = value;
};
void Pointer_setLongValue( addr_t addr, long long value ) {
  (*reinterpret_cast<long*>(addr)) = value;
};
void Pointer_setFloatValue( addr_t addr, float value ) {
  (*reinterpret_cast<float*>(addr)) = value;
};
void Pointer_setDoubleValue( addr_t addr, double value ) {
  (*reinterpret_cast<double*>(addr)) = value;
};
