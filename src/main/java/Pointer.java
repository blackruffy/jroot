
package net.blackruffy.root;

class PointerNative {
  static {
    System.loadLibrary("JRoot");
  }

  public static native long Pointer_getReference( long addr );
  public static native long Pointer_allocate( long size );
  public static native void Pointer_free( long addr );
  public static native long Pointer_getPosition( long addr, long pos );

  public static native int Pointer_getIntValue( long addr );
  public static native long Pointer_getLongValue( long addr );
  public static native float Pointer_getFloatValue( long addr );
  public static native double Pointer_getDoubleValue( long addr );

  public static native void Pointer_setIntValue( long addr, int value );
  public static native void Pointer_setLongValue( long addr, long value );
  public static native void Pointer_setFloatValue( long addr, float value );
  public static native void Pointer_setDoubleValue( long addr, double value );

  public static native int[] Pointer_getIntArray( long addr, int size );
  public static native long[] Pointer_getLongArray( long addr, int size );
  public static native float[] Pointer_getFloatArray( long addr, int size );
  public static native double[] Pointer_getDoubleArray( long addr, int size );
  public static native String[] Pointer_getStringArray( long addr, int size );

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

  default public Pointer getReference() {
    return newPointer(PointerNative.Pointer_getReference( getNativeAddress()));
  }

  default public void free() {
    PointerNative.Pointer_free( getNativeAddress() );
  }

  default public Pointer at( long pos ) {
    return newPointer(PointerNative.Pointer_getPosition( getNativeAddress(), pos ));
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

  default void setIntArray( int[] xs ) {
    PointerNative.Pointer_setIntArray( getNativeAddress(), xs );
  }

  default void setLongArray( long[] xs ) {
    PointerNative.Pointer_setLongArray( getNativeAddress(), xs );
  }

  default void setFloatArray( float[] xs ) {
    PointerNative.Pointer_setFloatArray( getNativeAddress(), xs );
  }

  default void getDoubleArray( double[] xs ) {
    PointerNative.Pointer_setDoubleArray( getNativeAddress(), xs );
  }

  public long getNativeAddress();

}
