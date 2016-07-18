#ifndef __JROOT_ADDR_H__
#define __JROOT_ADDR_H__

extern "C" {

  typedef long int addr_t;

  addr_t Pointer_getReference( addr_t addr );

  addr_t Pointer_allocate( long size );

  void Pointer_free( addr_t addr );
  addr_t Pointer_getPosition( addr_t addr, long long pos );

  int Pointer_getInt8Value( addr_t addr );
  int Pointer_getUInt8Value( addr_t addr );
  int Pointer_getInt16Value( addr_t addr );
  int Pointer_getUInt16Value( addr_t addr );
  int Pointer_getInt32Value( addr_t addr );
  long Pointer_getUInt32Value( addr_t addr );
  int Pointer_getIntValue( addr_t addr );
  long Pointer_getLongValue( addr_t addr );
  float Pointer_getFloatValue( addr_t addr );
  double Pointer_getDoubleValue( addr_t addr );

  void Pointer_setInt8Value( addr_t addr, int value );
  void Pointer_setUInt8Value( addr_t addr, int value );
  void Pointer_setInt16Value( addr_t addr, int value );
  void Pointer_setUInt16Value( addr_t addr, int value );
  void Pointer_setInt32Value( addr_t addr, int value );
  void Pointer_setUInt32Value( addr_t addr, long value );
  void Pointer_setIntValue( addr_t addr, int value );
  void Pointer_setLongValue( addr_t addr, long long value );
  void Pointer_setFloatValue( addr_t addr, float value );
  void Pointer_setDoubleValue( addr_t addr, double value );
  
}

#endif
