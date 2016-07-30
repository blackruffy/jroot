
package net.blackruffy.root;

class PointerNative {
  static {
    System.loadLibrary("JRoot");
  }

  public static native long Pointer_getReference( long addr );
  public static native long Pointer_allocate( long size );
  public static native void Pointer_free( long addr );
  public static native long Pointer_getPosition( long addr, long pos );

  public static native int Pointer_getInt8Value( long addr );
  public static native int Pointer_getUInt8Value( long addr );
  public static native int Pointer_getInt16Value( long addr );
  public static native int Pointer_getUInt16Value( long addr );
  public static native int Pointer_getInt32Value( long addr );
  public static native long Pointer_getUInt32Value( long addr );
  public static native int Pointer_getIntValue( long addr );
  public static native long Pointer_getLongValue( long addr );
  public static native float Pointer_getFloatValue( long addr );
  public static native double Pointer_getDoubleValue( long addr );

  public static native void Pointer_setInt8Value( long addr, int value );
  public static native void Pointer_setUInt8Value( long addr, int value );
  public static native void Pointer_setInt16Value( long addr, int value );
  public static native void Pointer_setUInt16Value( long addr, int value );
  public static native void Pointer_setInt32Value( long addr, int value );
  public static native void Pointer_setUInt32Value( long addr, long value );
  public static native void Pointer_setIntValue( long addr, int value );
  public static native void Pointer_setLongValue( long addr, long value );
  public static native void Pointer_setFloatValue( long addr, float value );
  public static native void Pointer_setDoubleValue( long addr, double value );

  public static native byte[] Pointer_getByteArray( long addr, int size );
  public static native int[] Pointer_getIntArray( long addr, int size );
  public static native long[] Pointer_getLongArray( long addr, int size );
  public static native float[] Pointer_getFloatArray( long addr, int size );
  public static native double[] Pointer_getDoubleArray( long addr, int size );
  public static native String[] Pointer_getStringArray( long addr, int size );

  public static native void Pointer_setByteArray( long addr, byte[] xs );
  public static native void Pointer_setIntArray( long addr, int[] xs );
  public static native void Pointer_setLongArray( long addr, long[] xs );
  public static native void Pointer_setFloatArray( long addr, float[] xs );
  public static native void Pointer_setDoubleArray( long addr, double[] xs );
}

public interface Pointer {

  public static Pointer newPointer( final long addr ) {
    return new Pointer() {
      public long getNativeAddress() {
        return addr;
      }
    };
  }

  public static Pointer newPointer( Pointer ptr ) {
    return newPointer(ptr.getNativeAddress());
  }

  public static Pointer allocate( long size ) {
    return newPointer(PointerNative.Pointer_allocate( size ));
  }


  /////////////////////////////////////////////////////////////////////


  public static Pointer newInt8() {
    return allocate(1);
  }

  public static Pointer newInt8( int x ) {
    Pointer p = newInt8();
    p.setInt8Value(x);
    return p;
  }

  public static Pointer newUInt8() {
    return allocate(1);
  }

  public static Pointer newUInt8( int x ) {
    Pointer p = newUInt8();
    p.setUInt8Value(x);
    return p;
  }

  public static Pointer newInt16() {
    return allocate(2);
  }

  public static Pointer newInt16( int x ) {
    Pointer p = newInt16();
    p.setInt16Value(x);
    return p;
  }

  public static Pointer newUInt16() {
    return allocate(2);
  }

  public static Pointer newUInt16( int x ) {
    Pointer p = newUInt16();
    p.setUInt16Value(x);
    return p;
  }

  public static Pointer newInt32() {
    return allocate(4);
  }

  public static Pointer newInt32( int x ) {
    Pointer p = newInt32();
    p.setInt32Value(x);
    return p;
  }

  public static Pointer newUInt32() {
    return allocate(4);
  }

  public static Pointer newUInt32( long x ) {
    Pointer p = newUInt32();
    p.setUInt32Value(x);
    return p;
  }

  public static Pointer newInt() {
    return allocate(4);
  }

  public static Pointer newInt( int x ) {
    Pointer p = newInt();
    p.setIntValue(x);
    return p;
  }

  public static Pointer newLong() {
    return allocate(8);
  }

  public static Pointer newLong( long x ) {
    Pointer p = newLong();
    p.setLongValue(x);
    return p;
  }

  public static Pointer newFloat() {
    return allocate(4);
  }

  public static Pointer newFloat( float x ) {
    Pointer p = newFloat();
    p.setFloatValue(x);
    return p;
  }

  public static Pointer newDouble() {
    return allocate(8);
  }

  public static Pointer newDouble( double x ) {
    Pointer p = newDouble();
    p.setDoubleValue(x);
    return p;
  }

  public static Pointer newInt8Array( int size ) {
    return allocate(size);
  }

  public static Pointer newUInt8Array( int size ) {
    return allocate(size);
  }

  public static Pointer newInt16Array( int size ) {
    return allocate(2*size);
  }

  public static Pointer newUInt16Array( int size ) {
    return allocate(2*size);
  }

  public static Pointer newInt32Array( int size ) {
    return allocate(4*size);
  }

  public static Pointer newUInt32Array( int size ) {
    return allocate(4*size);
  }

  public static Pointer newIntArray( int size ) {
    return allocate(4*size);
  }

  public static Pointer newLongArray( int size ) {
    return allocate(8*size);
  }

  public static Pointer newFloatArray( int size ) {
    return allocate(4*size);
  }

  public static Pointer newDoubleArray( int size ) {
    return allocate(8*size);
  }

  /////////////////////////////////////////////////////////////////////

  default public Pointer getReference() {
    return newPointer(PointerNative.Pointer_getReference( getNativeAddress()));
  }

  default public void free() {
    PointerNative.Pointer_free( getNativeAddress() );
  }


  default public Pointer at( long pos ) {
    return newPointer(PointerNative.Pointer_getPosition( getNativeAddress(), pos ));
  }

  /////////////////////////////////////////////////////////////////////

  default public int    getInt8Value() {
    return PointerNative.Pointer_getInt8Value( getNativeAddress() );
  }

  default public int    getUInt8Value() {
    return PointerNative.Pointer_getUInt8Value( getNativeAddress() );
  }

  default public int    getInt16Value() {
    return PointerNative.Pointer_getInt16Value( getNativeAddress() );
  }

  default public int    getUInt16Value() {
    return PointerNative.Pointer_getUInt16Value( getNativeAddress() );
  }

  default public int    getInt32Value() {
    return PointerNative.Pointer_getInt32Value( getNativeAddress() );
  }

  default public long    getUInt32Value() {
    return PointerNative.Pointer_getUInt32Value( getNativeAddress() );
  }

  default public int    getIntValue() {
    return PointerNative.Pointer_getIntValue( getNativeAddress() );
  }

  default public long   getLongValue() {
    return PointerNative.Pointer_getLongValue( getNativeAddress() );
  }

  default public float  getFloatValue() {
    return PointerNative.Pointer_getFloatValue( getNativeAddress() );
  }

  default public double getDoubleValue() {
    return PointerNative.Pointer_getDoubleValue( getNativeAddress() );
  }


  /////////////////////////////////////////////////////////////////////


  default void setInt8Value( int value ) {
    PointerNative.Pointer_setInt8Value( getNativeAddress(), value );
  }

  default void setUInt8Value( int value ) {
    PointerNative.Pointer_setUInt8Value( getNativeAddress(), value );
  }

  default void setInt16Value( int value ) {
    PointerNative.Pointer_setInt16Value( getNativeAddress(), value );
  }

  default void setUInt16Value( int value ) {
    PointerNative.Pointer_setUInt16Value( getNativeAddress(), value );
  }

  default void setInt32Value( int value ) {
    PointerNative.Pointer_setInt32Value( getNativeAddress(), value );
  }

  default void setUInt32Value( long value ) {
    PointerNative.Pointer_setUInt32Value( getNativeAddress(), value );
  }

  default void setIntValue( int value ) {
    PointerNative.Pointer_setIntValue( getNativeAddress(), value );
  }

  default void setLongValue( long value ) {
    PointerNative.Pointer_setLongValue( getNativeAddress(), value );
  }

  default void setFloatValue( float value ) {
    PointerNative.Pointer_setFloatValue( getNativeAddress(), value );
  }

  default void setDoubleValue( double value ) {
    PointerNative.Pointer_setDoubleValue( getNativeAddress(), value );
  };


  /////////////////////////////////////////////////////////////////////


  default byte[] getByteArray( int size ) {
    return PointerNative.Pointer_getByteArray( getNativeAddress(), size );
  }

  default int[] getInt8Array( int size ) {
    int[] xs = new int[size];
    for( int i=0; i<size; i++ ) xs[i] = at(i).getInt8Value();
    return xs;
  }

  default int[] getUInt8Array( int size ) {
    int[] xs = new int[size];
    for( int i=0; i<size; i++ ) xs[i] = at(i).getUInt8Value();
    return xs;
  }

  default int[] getInt16Array( int size ) {
    int[] xs = new int[size];
    for( int i=0; i<size; i++ ) xs[i] = at(i*2).getInt16Value();
    return xs;
  }

  default int[] getUInt16Array( int size ) {
    int[] xs = new int[size];
    for( int i=0; i<size; i++ ) xs[i] = at(i*2).getUInt16Value();
    return xs;
  }

  default int[] getInt32Array( int size ) {
    int[] xs = new int[size];
    for( int i=0; i<size; i++ ) xs[i] = at(i*4).getInt32Value();
    return xs;
  }

  default long[] getUInt32Array( int size ) {
    long[] xs = new long[size];
    for( int i=0; i<size; i++ ) xs[i] = at(i*4).getUInt32Value();
    return xs;
  }

  default int[] getIntArray( int size ) {
    return PointerNative.Pointer_getIntArray( getNativeAddress(), size );
  }

  default long[] getLongArray( int size ) {
    return PointerNative.Pointer_getLongArray( getNativeAddress(), size );
  }

  default float[] getFloatArray( int size ) {
    return PointerNative.Pointer_getFloatArray( getNativeAddress(), size );
  }

  default double[] getDoubleArray( int size ) {
    return PointerNative.Pointer_getDoubleArray( getNativeAddress(), size );
  }

  default String[] getStringArray( int size ) {
    return PointerNative.Pointer_getStringArray( getNativeAddress(), size );
  }

  /////////////////////////////////////////////////////////////////////

  default void setByteArray( byte[] xs ) {
    PointerNative.Pointer_setByteArray( getNativeAddress(), xs );
  }

  default void setInt8Array( int[] xs ) {
    for( int i=0; i<xs.length; i++ ) {
      at(i).setInt8Value(xs[i]);
    }
  }

  default void setUInt8Array( int[] xs ) {
    for( int i=0; i<xs.length; i++ ) {
      at(i).setUInt8Value(xs[i]);
    }
  }

  default void setInt16Array( int[] xs ) {
    for( int i=0; i<xs.length; i++ ) {
      at(i*2).setInt16Value(xs[i]);
    }
  }

  default void setUInt16Array( int[] xs ) {
    for( int i=0; i<xs.length; i++ ) {
      at(i*2).setUInt16Value(xs[i]);
    }
  }

  default void setInt32Array( int[] xs ) {
    for( int i=0; i<xs.length; i++ ) {
      at(i*4).setInt32Value(xs[i]);
    }
  }

  default void setUInt32Array( int[] xs ) {
    for( int i=0; i<xs.length; i++ ) {
      at(i*4).setUInt32Value(xs[i]);
    }
  }

  default void setIntArray( int[] xs ) {
    PointerNative.Pointer_setIntArray( getNativeAddress(), xs );
  }

  default void setLongArray( long[] xs ) {
    PointerNative.Pointer_setLongArray( getNativeAddress(), xs );
  }

  default void setFloatArray( float[] xs ) {
    PointerNative.Pointer_setFloatArray( getNativeAddress(), xs );
  }

  default void setDoubleArray( double[] xs ) {
    PointerNative.Pointer_setDoubleArray( getNativeAddress(), xs );
  }

  public long getNativeAddress();

}
