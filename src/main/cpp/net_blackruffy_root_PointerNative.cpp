#include <net_blackruffy_root_PointerNative.h>
#include <addr.h>

/*
 * Class:     net_blackruffy_root_PointerNative
 * Method:    Pointer_getReference
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_net_blackruffy_root_PointerNative_Pointer_1getReference
(JNIEnv *evn, jclass thisj, jlong addr)
{
  return Pointer_getReference(addr);
};

/*
 * Class:     net_blackruffy_root_PointerNative
 * Method:    Pointer_allocate
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_net_blackruffy_root_PointerNative_Pointer_1allocate
(JNIEnv *env, jclass thisj, jlong size)
{
  return Pointer_allocate(size);
};

/*
 * Class:     net_blackruffy_root_PointerNative
 * Method:    Pointer_free
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_net_blackruffy_root_PointerNative_Pointer_1free
(JNIEnv *env, jclass thisj, jlong addr)
{
  Pointer_free(addr);
};

/*
 * Class:     net_blackruffy_root_PointerNative
 * Method:    Pointer_getPosition
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_net_blackruffy_root_PointerNative_Pointer_1getPosition
(JNIEnv *env, jclass thisj, jlong addr, jlong pos)
{
  return Pointer_getPosition(addr, pos);
};

/*
 * Class:     net_blackruffy_root_PointerNative
 * Method:    Pointer_getIntValue
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_net_blackruffy_root_PointerNative_Pointer_1getIntValue
(JNIEnv *env, jclass thisj, jlong addr)
{
  return Pointer_getIntValue( addr );
};

/*
 * Class:     net_blackruffy_root_PointerNative
 * Method:    Pointer_getLongValue
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_net_blackruffy_root_PointerNative_Pointer_1getLongValue
(JNIEnv *env, jclass thisj, jlong addr)
{
  return Pointer_getLongValue( addr );
};

/*
 * Class:     net_blackruffy_root_PointerNative
 * Method:    Pointer_getFloatValue
 * Signature: (J)F
 */
JNIEXPORT jfloat JNICALL Java_net_blackruffy_root_PointerNative_Pointer_1getFloatValue
(JNIEnv *env, jclass thisj, jlong addr)
{
  return Pointer_getFloatValue( addr );
};

/*
 * Class:     net_blackruffy_root_PointerNative
 * Method:    Pointer_getDoubleValue
 * Signature: (J)D
 */
JNIEXPORT jdouble JNICALL Java_net_blackruffy_root_PointerNative_Pointer_1getDoubleValue
(JNIEnv *env, jclass thisj, jlong addr)
{
  return Pointer_getDoubleValue(addr);
};

/*
 * Class:     net_blackruffy_root_PointerNative
 * Method:    Pointer_setIntValue
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_net_blackruffy_root_PointerNative_Pointer_1setIntValue
(JNIEnv *env, jclass thisj, jlong addr, jint value)
{
  Pointer_setIntValue(addr, value);
};

/*
 * Class:     net_blackruffy_root_PointerNative
 * Method:    Pointer_setLongValue
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_net_blackruffy_root_PointerNative_Pointer_1setLongValue
(JNIEnv *env, jclass thisj, jlong addr, jlong value)
{
  Pointer_setLongValue(addr, value);
};

/*
 * Class:     net_blackruffy_root_PointerNative
 * Method:    Pointer_setFloatValue
 * Signature: (JF)V
 */
JNIEXPORT void JNICALL Java_net_blackruffy_root_PointerNative_Pointer_1setFloatValue
(JNIEnv *env, jclass thisj, jlong addr, jfloat value)
{
  Pointer_setFloatValue(addr, value);
};

/*
 * Class:     net_blackruffy_root_PointerNative
 * Method:    Pointer_setDoubleValue
 * Signature: (JD)V
 */
JNIEXPORT void JNICALL Java_net_blackruffy_root_PointerNative_Pointer_1setDoubleValue
(JNIEnv *env, jclass thisj, jlong addr, jdouble value)
{
  Pointer_setDoubleValue(addr, value);
};

JNIEXPORT jintArray JNICALL Java_net_blackruffy_root_PointerNative_Pointer_1getIntArray
(JNIEnv *env, jclass thisj, jlong addr, jint size)
{
  int* xs = reinterpret_cast<int*>(addr);
  jintArray jxs = env->NewIntArray(size);
  env->SetIntArrayRegion(jxs, 0, size, xs);
  return jxs;
};

JNIEXPORT jlongArray JNICALL Java_net_blackruffy_root_PointerNative_Pointer_1getLongArray
(JNIEnv *env, jclass thisj, jlong addr, jint size)
{
  long int* xs = reinterpret_cast<long int*>(addr);
  jlongArray jxs = env->NewLongArray(size);
  env->SetLongArrayRegion(jxs, 0, size, xs);
  return jxs;
};

JNIEXPORT jfloatArray JNICALL Java_net_blackruffy_root_PointerNative_Pointer_1getFloatArray
(JNIEnv *env, jclass thisj, jlong addr, jint size)
{
  float* xs = reinterpret_cast<float*>(addr);
  jfloatArray jxs = env->NewFloatArray(size);
  env->SetFloatArrayRegion(jxs, 0, size, xs);
  return jxs;
};

JNIEXPORT jdoubleArray JNICALL Java_net_blackruffy_root_PointerNative_Pointer_1getDoubleArray
(JNIEnv *env, jclass thisj, jlong addr, jint size)
{
  double* xs = reinterpret_cast<double*>(addr);
  jdoubleArray jxs = env->NewDoubleArray(size);
  env->SetDoubleArrayRegion(jxs, 0, size, xs);
  return jxs;
};

JNIEXPORT jobjectArray JNICALL Java_net_blackruffy_root_PointerNative_Pointer_1getStringArray
(JNIEnv *env, jclass thisj, jlong addr, jint size)
{
  char** xs = reinterpret_cast<char**>(addr);
  jclass cls = env->FindClass("Ljava/lang/String;");
  jobjectArray jxs = env->NewObjectArray(size, cls, NULL);
  for( jint i=0; i<size; i++ ) {
    env->SetObjectArrayElement(jxs, i, env->NewStringUTF(xs[i]));
  }
  return jxs;
};

JNIEXPORT void JNICALL Java_net_blackruffy_root_PointerNative_Pointer_1setIntArray
(JNIEnv *env, jclass thisj, jlong addr, jintArray jxs)
{
  jsize size = env->GetArrayLength(jxs);
  int* xs = reinterpret_cast<int*>(addr);
  env->GetIntArrayRegion(jxs, 0, size, xs);
};

JNIEXPORT void JNICALL Java_net_blackruffy_root_PointerNative_Pointer_1setLongArray
(JNIEnv *env, jclass thisj, jlong addr, jlongArray jxs)
{
  jsize size = env->GetArrayLength(jxs);
  long* xs = reinterpret_cast<long*>(addr);
  env->GetLongArrayRegion(jxs, 0, size, xs);
};

JNIEXPORT void JNICALL Java_net_blackruffy_root_PointerNative_Pointer_1setFloatArray
(JNIEnv *env, jclass thisj, jlong addr, jfloatArray jxs)
{
  jsize size = env->GetArrayLength(jxs);
  float* xs = reinterpret_cast<float*>(addr);
  env->GetFloatArrayRegion(jxs, 0, size, xs);
};

JNIEXPORT void JNICALL Java_net_blackruffy_root_PointerNative_Pointer_1setDoubleArray
(JNIEnv *env, jclass thisj, jlong addr, jdoubleArray jxs)
{
  jsize size = env->GetArrayLength(jxs);
  double* xs = reinterpret_cast<double*>(addr);
  env->GetDoubleArrayRegion(jxs, 0, size, xs);
};
