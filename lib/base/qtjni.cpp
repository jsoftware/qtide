#include <QApplication>
#include <QDebug>
#if 0
#ifdef Q_OS_ANDROID
#include <QtAndroid>
#include <QAndroidJniEnvironment>
#endif
#include "qtjni.h"
#include "base.h"

#define ATTACHTHREAD QAndroidJniEnvironment jnienv;
#define DETACHTHREAD

int GetJavaVMENV(JavaVM ** pvm, JNIEnv ** penv)
{
  ATTACHTHREAD
  *pvm = QAndroidJniEnvironment::javaVM();
  *penv = jnienv;
  return 0;
}

jclass GetClassID(int id)
{
  ATTACHTHREAD
  jclass z=0;
  switch (id) {
  case 0 :
    z=(jclass)jnienv->NewGlobalRef(jnienv->FindClass("org/qtproject/qt5/android/bindings/QtApplication"));
    break;
  case 1 :
    z=(jclass)jnienv->NewGlobalRef(jnienv->FindClass("org/qtproject/qt5/android/bindings/QtActivity"));
    break;
  }
  return z;
}

jint GetVersion()
{
  ATTACHTHREAD
  return jnienv->GetVersion();
}
jclass DefineClass(const char *name, jobject loader, const jbyte *buf, jsize len)
{
  ATTACHTHREAD
  return jnienv->DefineClass(name, loader, buf, len);
}
jclass FindClass(const char *name)
{
  ATTACHTHREAD
  return jnienv->FindClass(name);
}
jmethodID FromReflectedMethod(jobject method)
{
  ATTACHTHREAD
  return jnienv->FromReflectedMethod(method);
}
jfieldID FromReflectedField(jobject field)
{
  ATTACHTHREAD
  return jnienv->FromReflectedField(field);
}
jobject ToReflectedMethod(jclass cls, jmethodID methodID, jboolean isStatic)
{
  ATTACHTHREAD
  return jnienv->ToReflectedMethod(cls, methodID, isStatic);
}
jclass GetSuperclass(jclass sub)
{
  ATTACHTHREAD
  return jnienv->GetSuperclass(sub);
}
jboolean IsAssignableFrom(jclass sub, jclass sup)
{
  ATTACHTHREAD
  return jnienv->IsAssignableFrom(sub, sup);
}
jobject ToReflectedField(jclass cls, jfieldID fieldID, jboolean isStatic)
{
  ATTACHTHREAD
  return jnienv->ToReflectedField(cls,fieldID,isStatic);
}
jint Throw(jthrowable obj)
{
  ATTACHTHREAD
  return jnienv->Throw(obj);
}
jint ThrowNew(jclass clazz, const char *msg)
{
  ATTACHTHREAD
  return jnienv->ThrowNew(clazz, msg);
}
jthrowable ExceptionOccurred()
{
  ATTACHTHREAD
  return jnienv->ExceptionOccurred();
}
void ExceptionDescribe()
{
  ATTACHTHREAD
  jnienv->ExceptionDescribe();
}
void ExceptionClear()
{
  ATTACHTHREAD
  jnienv->ExceptionClear();
}
void FatalError(const char *msg)
{
  ATTACHTHREAD
  jnienv->FatalError(msg);
}
jint PushLocalFrame(jint capacity)
{
  ATTACHTHREAD
  return jnienv->PushLocalFrame(capacity);
}
jobject PopLocalFrame(jobject result)
{
  ATTACHTHREAD
  return jnienv->PopLocalFrame(result);
}
jobject NewGlobalRef(jobject lobj)
{
  ATTACHTHREAD
  return jnienv->NewGlobalRef(lobj);
}
void DeleteGlobalRef(jobject gref)
{
  ATTACHTHREAD
  jnienv->DeleteGlobalRef(gref);
}
void DeleteLocalRef(jobject obj)
{
  ATTACHTHREAD
  jnienv->DeleteLocalRef(obj);
}
jboolean IsSameObject(jobject obj1, jobject obj2)
{
  ATTACHTHREAD
  return jnienv->IsSameObject(obj1,obj2);
}
jobject NewLocalRef(jobject ref)
{
  ATTACHTHREAD
  return jnienv->NewLocalRef(ref);
}
jint EnsureLocalCapacity(jint capacity)
{
  ATTACHTHREAD
  return jnienv->EnsureLocalCapacity(capacity);
}
jobject AllocObject(jclass clazz)
{
  ATTACHTHREAD
  return jnienv->AllocObject(clazz);
}
jobject NewObject(jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jobject result;
  va_start(args, methodID);
  result = jnienv->NewObjectV(clazz,methodID,args);
  va_end(args);
  return result;
}
jobject NewObjectV(jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->NewObjectV(clazz,methodID,args);
}
jobject NewObjectA(jclass clazz, jmethodID methodID, jvalue *args)
{
  ATTACHTHREAD
  return jnienv->NewObjectA(clazz,methodID,args);
}
jclass GetObjectClass(jobject obj)
{
  ATTACHTHREAD
  return jnienv->GetObjectClass(obj);
}
jboolean IsInstanceOf(jobject obj, jclass clazz)
{
  ATTACHTHREAD
  return jnienv->IsInstanceOf(obj,clazz);
}
jmethodID GetMethodID(jclass clazz, const char *name, const char *sig)
{
  ATTACHTHREAD
  return jnienv->GetMethodID(clazz,name,sig);
}
jobject CallObjectMethod(jobject obj, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jobject result;
  va_start(args,methodID);
  result = jnienv->CallObjectMethodV(obj,methodID,args);
  va_end(args);
  return result;
}
jobject CallObjectMethodV(jobject obj, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->CallObjectMethodV(obj,methodID,args);
}
jobject CallObjectMethodA(jobject obj, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->CallObjectMethodA(obj,methodID,args);
}
jboolean CallBooleanMethod(jobject obj, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jboolean result;
  va_start(args,methodID);
  result = jnienv->CallBooleanMethodV(obj,methodID,args);
  va_end(args);
  return result;
}
jboolean CallBooleanMethodV(jobject obj, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->CallBooleanMethodV(obj,methodID,args);
}
jboolean CallBooleanMethodA(jobject obj, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->CallBooleanMethodA(obj,methodID, args);
}
jbyte CallByteMethod(jobject obj, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jbyte result;
  va_start(args,methodID);
  result = jnienv->CallByteMethodV(obj,methodID,args);
  va_end(args);
  return result;
}
jbyte CallByteMethodV(jobject obj, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->CallByteMethodV(obj,methodID,args);
}
jbyte CallByteMethodA(jobject obj, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->CallByteMethodA(obj,methodID,args);
}
jchar CallCharMethod(jobject obj, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jchar result;
  va_start(args,methodID);
  result = jnienv->CallCharMethodV(obj,methodID,args);
  va_end(args);
  return result;
}
jchar CallCharMethodV(jobject obj, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->CallCharMethodV(obj,methodID,args);
}
jchar CallCharMethodA(jobject obj, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->CallCharMethodA(obj,methodID,args);
}
jshort CallShortMethod(jobject obj, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jshort result;
  va_start(args,methodID);
  result = jnienv->CallShortMethodV(obj,methodID,args);
  va_end(args);
  return result;
}
jshort CallShortMethodV(jobject obj, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->CallShortMethodV(obj,methodID,args);
}
jshort CallShortMethodA(jobject obj, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->CallShortMethodA(obj,methodID,args);
}
jint CallIntMethod(jobject obj, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jint result;
  va_start(args,methodID);
  result = jnienv->CallIntMethodV(obj,methodID,args);
  va_end(args);
  return result;
}
jint CallIntMethodV(jobject obj, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->CallIntMethodV(obj,methodID,args);
}
jint CallIntMethodA(jobject obj, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->CallIntMethodA(obj,methodID,args);
}
jlong CallLongMethod(jobject obj, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jlong result;
  va_start(args,methodID);
  result = jnienv->CallLongMethodV(obj,methodID,args);
  va_end(args);
  return result;
}
jlong CallLongMethodV(jobject obj, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->CallLongMethodV(obj,methodID,args);
}
jlong CallLongMethodA(jobject obj, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->CallLongMethodA(obj,methodID,args);
}
jfloat CallFloatMethod(jobject obj, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jfloat result;
  va_start(args,methodID);
  result = jnienv->CallFloatMethodV(obj,methodID,args);
  va_end(args);
  return result;
}
jfloat CallFloatMethodV(jobject obj, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->CallFloatMethodV(obj,methodID,args);
}
jfloat CallFloatMethodA(jobject obj, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->CallFloatMethodA(obj,methodID,args);
}
jdouble CallDoubleMethod(jobject obj, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jdouble result;
  va_start(args,methodID);
  result = jnienv->CallDoubleMethodV(obj,methodID,args);
  va_end(args);
  return result;
}
jdouble CallDoubleMethodV(jobject obj, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->CallDoubleMethodV(obj,methodID,args);
}
jdouble CallDoubleMethodA(jobject obj, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->CallDoubleMethodA(obj,methodID,args);
}
void CallVoidMethod(jobject obj, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  va_start(args,methodID);
  jnienv->CallVoidMethodV(obj,methodID,args);
  va_end(args);
}
void CallVoidMethodV(jobject obj, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  jnienv->CallVoidMethodV(obj,methodID,args);
}
void CallVoidMethodA(jobject obj, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  jnienv->CallVoidMethodA(obj,methodID,args);
}
jobject CallNonvirtualObjectMethod(jobject obj, jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jobject result;
  va_start(args,methodID);
  result = jnienv->CallNonvirtualObjectMethodV(obj,clazz, methodID,args);
  va_end(args);
  return result;
}
jobject CallNonvirtualObjectMethodV(jobject obj, jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->CallNonvirtualObjectMethodV(obj,clazz, methodID,args);
}
jobject CallNonvirtualObjectMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->CallNonvirtualObjectMethodA(obj,clazz, methodID,args);
}
jboolean CallNonvirtualBooleanMethod(jobject obj, jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jboolean result;
  va_start(args,methodID);
  result = jnienv->CallNonvirtualBooleanMethodV(obj,clazz, methodID,args);
  va_end(args);
  return result;
}
jboolean CallNonvirtualBooleanMethodV(jobject obj, jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->CallNonvirtualBooleanMethodV(obj,clazz, methodID,args);
}
jboolean CallNonvirtualBooleanMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->CallNonvirtualBooleanMethodA(obj,clazz, methodID, args);
}
jbyte CallNonvirtualByteMethod(jobject obj, jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jbyte result;
  va_start(args,methodID);
  result = jnienv->CallNonvirtualByteMethodV(obj,clazz, methodID,args);
  va_end(args);
  return result;
}
jbyte CallNonvirtualByteMethodV(jobject obj, jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->CallNonvirtualByteMethodV(obj,clazz, methodID,args);
}
jbyte CallNonvirtualByteMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->CallNonvirtualByteMethodA(obj,clazz, methodID,args);
}
jchar CallNonvirtualCharMethod(jobject obj, jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jchar result;
  va_start(args,methodID);
  result = jnienv->CallNonvirtualCharMethodV(obj,clazz, methodID,args);
  va_end(args);
  return result;
}
jchar CallNonvirtualCharMethodV(jobject obj, jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->CallNonvirtualCharMethodV(obj,clazz, methodID,args);
}
jchar CallNonvirtualCharMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->CallNonvirtualCharMethodA(obj,clazz, methodID,args);
}
jshort CallNonvirtualShortMethod(jobject obj, jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jshort result;
  va_start(args,methodID);
  result = jnienv->CallNonvirtualShortMethodV(obj,clazz, methodID,args);
  va_end(args);
  return result;
}
jshort CallNonvirtualShortMethodV(jobject obj, jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->CallNonvirtualShortMethodV(obj,clazz, methodID,args);
}
jshort CallNonvirtualShortMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->CallNonvirtualShortMethodA(obj,clazz, methodID,args);
}
jint CallNonvirtualIntMethod(jobject obj, jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jint result;
  va_start(args,methodID);
  result = jnienv->CallNonvirtualIntMethodV(obj,clazz, methodID,args);
  va_end(args);
  return result;
}
jint CallNonvirtualIntMethodV(jobject obj, jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->CallNonvirtualIntMethodV(obj,clazz, methodID,args);
}
jint CallNonvirtualIntMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->CallNonvirtualIntMethodA(obj,clazz, methodID,args);
}
jlong CallNonvirtualLongMethod(jobject obj, jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jlong result;
  va_start(args,methodID);
  result = jnienv->CallNonvirtualLongMethodV(obj,clazz, methodID,args);
  va_end(args);
  return result;
}
jlong CallNonvirtualLongMethodV(jobject obj, jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->CallNonvirtualLongMethodV(obj,clazz, methodID,args);
}
jlong CallNonvirtualLongMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->CallNonvirtualLongMethodA(obj,clazz, methodID,args);
}
jfloat CallNonvirtualFloatMethod(jobject obj, jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jfloat result;
  va_start(args,methodID);
  result = jnienv->CallNonvirtualFloatMethodV(obj,clazz, methodID,args);
  va_end(args);
  return result;
}
jfloat CallNonvirtualFloatMethodV(jobject obj, jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->CallNonvirtualFloatMethodV(obj,clazz, methodID,args);
}
jfloat CallNonvirtualFloatMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->CallNonvirtualFloatMethodA(obj,clazz, methodID,args);
}
jdouble CallNonvirtualDoubleMethod(jobject obj, jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jdouble result;
  va_start(args,methodID);
  result = jnienv->CallNonvirtualDoubleMethodV(obj,clazz, methodID,args);
  va_end(args);
  return result;
}
jdouble CallNonvirtualDoubleMethodV(jobject obj, jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->CallNonvirtualDoubleMethodV(obj,clazz, methodID,args);
}
jdouble CallNonvirtualDoubleMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->CallNonvirtualDoubleMethodA(obj,clazz, methodID,args);
}
void CallNonvirtualVoidMethod(jobject obj, jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  va_start(args,methodID);
  jnienv->CallNonvirtualVoidMethodV(obj,clazz,methodID,args);
  va_end(args);
}
void CallNonvirtualVoidMethodV(jobject obj, jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  jnienv->CallNonvirtualVoidMethodV(obj,clazz,methodID,args);
}
void CallNonvirtualVoidMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  jnienv->CallNonvirtualVoidMethodA(obj,clazz,methodID,args);
}
jfieldID GetFieldID(jclass clazz, const char *name, const char *sig)
{
  ATTACHTHREAD
  return jnienv->GetFieldID(clazz,name,sig);
}
jobject GetObjectField(jobject obj, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->GetObjectField(obj,fieldID);
}
jboolean GetBooleanField(jobject obj, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->GetBooleanField(obj,fieldID);
}
jbyte GetByteField(jobject obj, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->GetByteField(obj,fieldID);
}
jchar GetCharField(jobject obj, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->GetCharField(obj,fieldID);
}
jshort GetShortField(jobject obj, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->GetShortField(obj,fieldID);
}
jint GetIntField(jobject obj, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->GetIntField(obj,fieldID);
}
jlong GetLongField(jobject obj, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->GetLongField(obj,fieldID);
}
jfloat GetFloatField(jobject obj, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->GetFloatField(obj,fieldID);
}
jdouble GetDoubleField(jobject obj, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->GetDoubleField(obj,fieldID);
}
void SetObjectField(jobject obj, jfieldID fieldID, jobject val)
{
  ATTACHTHREAD
  jnienv->SetObjectField(obj,fieldID,val);
}
void SetBooleanField(jobject obj, jfieldID fieldID, jboolean val)
{
  ATTACHTHREAD
  jnienv->SetBooleanField(obj,fieldID,val);
}
void SetByteField(jobject obj, jfieldID fieldID, jbyte val)
{
  ATTACHTHREAD
  jnienv->SetByteField(obj,fieldID,val);
}
void SetCharField(jobject obj, jfieldID fieldID, jchar val)
{
  ATTACHTHREAD
  jnienv->SetCharField(obj,fieldID,val);
}
void SetShortField(jobject obj, jfieldID fieldID, jshort val)
{
  ATTACHTHREAD
  jnienv->SetShortField(obj,fieldID,val);
}
void SetIntField(jobject obj, jfieldID fieldID, jint val)
{
  ATTACHTHREAD
  jnienv->SetIntField(obj,fieldID,val);
}
void SetLongField(jobject obj, jfieldID fieldID, jlong val)
{
  ATTACHTHREAD
  jnienv->SetLongField(obj,fieldID,val);
}
void SetFloatField(jobject obj, jfieldID fieldID, jfloat val)
{
  ATTACHTHREAD
  jnienv->SetFloatField(obj,fieldID,val);
}
void SetDoubleField(jobject obj, jfieldID fieldID, jdouble val)
{
  ATTACHTHREAD
  jnienv->SetDoubleField(obj,fieldID,val);
}
jmethodID GetStaticMethodID(jclass clazz, const char *name, const char *sig)
{
  ATTACHTHREAD
  return jnienv->GetStaticMethodID(clazz,name,sig);
}
jobject CallStaticObjectMethod(jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jobject result;
  va_start(args,methodID);
  result = jnienv->CallStaticObjectMethodV(clazz,methodID,args);
  va_end(args);
  return result;
}
jobject CallStaticObjectMethodV(jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->CallStaticObjectMethodV(clazz,methodID,args);
}
jobject CallStaticObjectMethodA(jclass clazz, jmethodID methodID, jvalue *args)
{
  ATTACHTHREAD
  return jnienv->CallStaticObjectMethodA(clazz,methodID,args);
}
jboolean CallStaticBooleanMethod(jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jboolean result;
  va_start(args,methodID);
  result = jnienv->CallStaticBooleanMethodV(clazz,methodID,args);
  va_end(args);
  return result;
}
jboolean CallStaticBooleanMethodV(jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->CallStaticBooleanMethodV(clazz,methodID,args);
}
jboolean CallStaticBooleanMethodA(jclass clazz, jmethodID methodID, jvalue *args)
{
  ATTACHTHREAD
  return jnienv->CallStaticBooleanMethodA(clazz,methodID,args);
}
jbyte CallStaticByteMethod(jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jbyte result;
  va_start(args,methodID);
  result = jnienv->CallStaticByteMethodV(clazz,methodID,args);
  va_end(args);
  return result;
}
jbyte CallStaticByteMethodV(jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->CallStaticByteMethodV(clazz,methodID,args);
}
jbyte CallStaticByteMethodA(jclass clazz, jmethodID methodID, jvalue *args)
{
  ATTACHTHREAD
  return jnienv->CallStaticByteMethodA(clazz,methodID,args);
}
jchar CallStaticCharMethod(jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jchar result;
  va_start(args,methodID);
  result = jnienv->CallStaticCharMethodV(clazz,methodID,args);
  va_end(args);
  return result;
}
jchar CallStaticCharMethodV(jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->CallStaticCharMethodV(clazz,methodID,args);
}
jchar CallStaticCharMethodA(jclass clazz, jmethodID methodID, jvalue *args)
{
  ATTACHTHREAD
  return jnienv->CallStaticCharMethodA(clazz,methodID,args);
}
jshort CallStaticShortMethod(jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jshort result;
  va_start(args,methodID);
  result = jnienv->CallStaticShortMethodV(clazz,methodID,args);
  va_end(args);
  return result;
}
jshort CallStaticShortMethodV(jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->CallStaticShortMethodV(clazz,methodID,args);
}
jshort CallStaticShortMethodA(jclass clazz, jmethodID methodID, jvalue *args)
{
  ATTACHTHREAD
  return jnienv->CallStaticShortMethodA(clazz,methodID,args);
}
jint CallStaticIntMethod(jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jint result;
  va_start(args,methodID);
  result = jnienv->CallStaticIntMethodV(clazz,methodID,args);
  va_end(args);
  return result;
}
jint CallStaticIntMethodV(jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->CallStaticIntMethodV(clazz,methodID,args);
}
jint CallStaticIntMethodA(jclass clazz, jmethodID methodID, jvalue *args)
{
  ATTACHTHREAD
  return jnienv->CallStaticIntMethodA(clazz,methodID,args);
}
jlong CallStaticLongMethod(jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jlong result;
  va_start(args,methodID);
  result = jnienv->CallStaticLongMethodV(clazz,methodID,args);
  va_end(args);
  return result;
}
jlong CallStaticLongMethodV(jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->CallStaticLongMethodV(clazz,methodID,args);
}
jlong CallStaticLongMethodA(jclass clazz, jmethodID methodID, jvalue *args)
{
  ATTACHTHREAD
  return jnienv->CallStaticLongMethodA(clazz,methodID,args);
}
jfloat CallStaticFloatMethod(jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jfloat result;
  va_start(args,methodID);
  result = jnienv->CallStaticFloatMethodV(clazz,methodID,args);
  va_end(args);
  return result;
}
jfloat CallStaticFloatMethodV(jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->CallStaticFloatMethodV(clazz,methodID,args);
}
jfloat CallStaticFloatMethodA(jclass clazz, jmethodID methodID, jvalue *args)
{
  ATTACHTHREAD
  return jnienv->CallStaticFloatMethodA(clazz,methodID,args);
}
jdouble CallStaticDoubleMethod(jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jdouble result;
  va_start(args,methodID);
  result = jnienv->CallStaticDoubleMethodV(clazz,methodID,args);
  va_end(args);
  return result;
}
jdouble CallStaticDoubleMethodV(jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->CallStaticDoubleMethodV(clazz,methodID,args);
}
jdouble CallStaticDoubleMethodA(jclass clazz, jmethodID methodID, jvalue *args)
{
  ATTACHTHREAD
  return jnienv->CallStaticDoubleMethodA(clazz,methodID,args);
}
void CallStaticVoidMethod(jclass cls, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  va_start(args,methodID);
  jnienv->CallStaticVoidMethodV(cls,methodID,args);
  va_end(args);
}
void CallStaticVoidMethodV(jclass cls, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  jnienv->CallStaticVoidMethodV(cls,methodID,args);
}
void CallStaticVoidMethodA(jclass cls, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  jnienv->CallStaticVoidMethodA(cls,methodID,args);
}
jfieldID GetStaticFieldID(jclass clazz, const char *name, const char *sig)
{
  ATTACHTHREAD
  return jnienv->GetStaticFieldID(clazz,name,sig);
}
jobject GetStaticObjectField(jclass clazz, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->GetStaticObjectField(clazz,fieldID);
}
jboolean GetStaticBooleanField(jclass clazz, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->GetStaticBooleanField(clazz,fieldID);
}
jbyte GetStaticByteField(jclass clazz, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->GetStaticByteField(clazz,fieldID);
}
jchar GetStaticCharField(jclass clazz, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->GetStaticCharField(clazz,fieldID);
}
jshort GetStaticShortField(jclass clazz, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->GetStaticShortField(clazz,fieldID);
}
jint GetStaticIntField(jclass clazz, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->GetStaticIntField(clazz,fieldID);
}
jlong GetStaticLongField(jclass clazz, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->GetStaticLongField(clazz,fieldID);
}
jfloat GetStaticFloatField(jclass clazz, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->GetStaticFloatField(clazz,fieldID);
}
jdouble GetStaticDoubleField(jclass clazz, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->GetStaticDoubleField(clazz,fieldID);
}
void SetStaticObjectField(jclass clazz, jfieldID fieldID, jobject value)
{
  ATTACHTHREAD
  jnienv->SetStaticObjectField(clazz,fieldID,value);
}
void SetStaticBooleanField(jclass clazz, jfieldID fieldID, jboolean value)
{
  ATTACHTHREAD
  jnienv->SetStaticBooleanField(clazz,fieldID,value);
}
void SetStaticByteField(jclass clazz, jfieldID fieldID, jbyte value)
{
  ATTACHTHREAD
  jnienv->SetStaticByteField(clazz,fieldID,value);
}
void SetStaticCharField(jclass clazz, jfieldID fieldID, jchar value)
{
  ATTACHTHREAD
  jnienv->SetStaticCharField(clazz,fieldID,value);
}
void SetStaticShortField(jclass clazz, jfieldID fieldID, jshort value)
{
  ATTACHTHREAD
  jnienv->SetStaticShortField(clazz,fieldID,value);
}
void SetStaticIntField(jclass clazz, jfieldID fieldID, jint value)
{
  ATTACHTHREAD
  jnienv->SetStaticIntField(clazz,fieldID,value);
}
void SetStaticLongField(jclass clazz, jfieldID fieldID, jlong value)
{
  ATTACHTHREAD
  jnienv->SetStaticLongField(clazz,fieldID,value);
}
void SetStaticFloatField(jclass clazz, jfieldID fieldID, jfloat value)
{
  ATTACHTHREAD
  jnienv->SetStaticFloatField(clazz,fieldID,value);
}
void SetStaticDoubleField(jclass clazz, jfieldID fieldID, jdouble value)
{
  ATTACHTHREAD
  jnienv->SetStaticDoubleField(clazz,fieldID,value);
}
jstring NewString(const jchar *unicode, jsize len)
{
  ATTACHTHREAD
  return jnienv->NewString(unicode,len);
}
jsize GetStringLength(jstring str)
{
  ATTACHTHREAD
  return jnienv->GetStringLength(str);
}
const jchar *GetStringChars(jstring str, jboolean *isCopy)
{
  ATTACHTHREAD
  return jnienv->GetStringChars(str,isCopy);
}
void ReleaseStringChars(jstring str, const jchar *chars)
{
  ATTACHTHREAD
  jnienv->ReleaseStringChars(str,chars);
}
jstring NewStringUTF(const char *utf)
{
  ATTACHTHREAD
  return jnienv->NewStringUTF(utf);
}
jsize GetStringUTFLength(jstring str)
{
  ATTACHTHREAD
  return jnienv->GetStringUTFLength(str);
}
const char* GetStringUTFChars(jstring str, jboolean *isCopy)
{
  ATTACHTHREAD
  return jnienv->GetStringUTFChars(str,isCopy);
}
void ReleaseStringUTFChars(jstring str, const char* chars)
{
  ATTACHTHREAD
  jnienv->ReleaseStringUTFChars(str,chars);
}
jsize GetArrayLength(jarray array)
{
  ATTACHTHREAD
  return jnienv->GetArrayLength(array);
}
jobjectArray NewObjectArray(jsize len, jclass clazz, jobject init)
{
  ATTACHTHREAD
  return jnienv->NewObjectArray(len,clazz,init);
}
jobject GetObjectArrayElement(jobjectArray array, jsize index)
{
  ATTACHTHREAD
  return jnienv->GetObjectArrayElement(array,index);
}
void SetObjectArrayElement(jobjectArray array, jsize index, jobject val)
{
  ATTACHTHREAD
  jnienv->SetObjectArrayElement(array,index,val);
}
jbooleanArray NewBooleanArray(jsize len)
{
  ATTACHTHREAD
  return jnienv->NewBooleanArray(len);
}
jbyteArray NewByteArray(jsize len)
{
  ATTACHTHREAD
  return jnienv->NewByteArray(len);
}
jcharArray NewCharArray(jsize len)
{
  ATTACHTHREAD
  return jnienv->NewCharArray(len);
}
jshortArray NewShortArray(jsize len)
{
  ATTACHTHREAD
  return jnienv->NewShortArray(len);
}
jintArray NewIntArray(jsize len)
{
  ATTACHTHREAD
  return jnienv->NewIntArray(len);
}
jlongArray NewLongArray(jsize len)
{
  ATTACHTHREAD
  return jnienv->NewLongArray(len);
}
jfloatArray NewFloatArray(jsize len)
{
  ATTACHTHREAD
  return jnienv->NewFloatArray(len);
}
jdoubleArray NewDoubleArray(jsize len)
{
  ATTACHTHREAD
  return jnienv->NewDoubleArray(len);
}
jboolean * GetBooleanArrayElements(jbooleanArray array, jboolean *isCopy)
{
  ATTACHTHREAD
  return jnienv->GetBooleanArrayElements(array,isCopy);
}
jbyte * GetByteArrayElements(jbyteArray array, jboolean *isCopy)
{
  ATTACHTHREAD
  return jnienv->GetByteArrayElements(array,isCopy);
}
jchar * GetCharArrayElements(jcharArray array, jboolean *isCopy)
{
  ATTACHTHREAD
  return jnienv->GetCharArrayElements(array,isCopy);
}
jshort * GetShortArrayElements(jshortArray array, jboolean *isCopy)
{
  ATTACHTHREAD
  return jnienv->GetShortArrayElements(array,isCopy);
}
jint * GetIntArrayElements(jintArray array, jboolean *isCopy)
{
  ATTACHTHREAD
  return jnienv->GetIntArrayElements(array,isCopy);
}
jlong * GetLongArrayElements(jlongArray array, jboolean *isCopy)
{
  ATTACHTHREAD
  return jnienv->GetLongArrayElements(array,isCopy);
}
jfloat * GetFloatArrayElements(jfloatArray array, jboolean *isCopy)
{
  ATTACHTHREAD
  return jnienv->GetFloatArrayElements(array,isCopy);
}
jdouble * GetDoubleArrayElements(jdoubleArray array, jboolean *isCopy)
{
  ATTACHTHREAD
  return jnienv->GetDoubleArrayElements(array,isCopy);
}
void ReleaseBooleanArrayElements(jbooleanArray array, jboolean *elems, jint mode)
{
  ATTACHTHREAD
  jnienv->ReleaseBooleanArrayElements(array,elems,mode);
}
void ReleaseByteArrayElements(jbyteArray array, jbyte *elems, jint mode)
{
  ATTACHTHREAD
  jnienv->ReleaseByteArrayElements(array,elems,mode);
}
void ReleaseCharArrayElements(jcharArray array, jchar *elems, jint mode)
{
  ATTACHTHREAD
  jnienv->ReleaseCharArrayElements(array,elems,mode);
}
void ReleaseShortArrayElements(jshortArray array, jshort *elems, jint mode)
{
  ATTACHTHREAD
  jnienv->ReleaseShortArrayElements(array,elems,mode);
}
void ReleaseIntArrayElements(jintArray array, jint *elems, jint mode)
{
  ATTACHTHREAD
  jnienv->ReleaseIntArrayElements(array,elems,mode);
}
void ReleaseLongArrayElements(jlongArray array, jlong *elems, jint mode)
{
  ATTACHTHREAD
  jnienv->ReleaseLongArrayElements(array,elems,mode);
}
void ReleaseFloatArrayElements(jfloatArray array, jfloat *elems, jint mode)
{
  ATTACHTHREAD
  jnienv->ReleaseFloatArrayElements(array,elems,mode);
}
void ReleaseDoubleArrayElements(jdoubleArray array, jdouble *elems, jint mode)
{
  ATTACHTHREAD
  jnienv->ReleaseDoubleArrayElements(array,elems,mode);
}
void GetBooleanArrayRegion(jbooleanArray array, jsize start, jsize len, jboolean *buf)
{
  ATTACHTHREAD
  jnienv->GetBooleanArrayRegion(array,start,len,buf);
}
void GetByteArrayRegion(jbyteArray array, jsize start, jsize len, jbyte *buf)
{
  ATTACHTHREAD
  jnienv->GetByteArrayRegion(array,start,len,buf);
}
void GetCharArrayRegion(jcharArray array, jsize start, jsize len, jchar *buf)
{
  ATTACHTHREAD
  jnienv->GetCharArrayRegion(array,start,len,buf);
}
void GetShortArrayRegion(jshortArray array, jsize start, jsize len, jshort *buf)
{
  ATTACHTHREAD
  jnienv->GetShortArrayRegion(array,start,len,buf);
}
void GetIntArrayRegion(jintArray array, jsize start, jsize len, jint *buf)
{
  ATTACHTHREAD
  jnienv->GetIntArrayRegion(array,start,len,buf);
}
void GetLongArrayRegion(jlongArray array, jsize start, jsize len, jlong *buf)
{
  ATTACHTHREAD
  jnienv->GetLongArrayRegion(array,start,len,buf);
}
void GetFloatArrayRegion(jfloatArray array, jsize start, jsize len, jfloat *buf)
{
  ATTACHTHREAD
  jnienv->GetFloatArrayRegion(array,start,len,buf);
}
void GetDoubleArrayRegion(jdoubleArray array, jsize start, jsize len, jdouble *buf)
{
  ATTACHTHREAD
  jnienv->GetDoubleArrayRegion(array,start,len,buf);
}
void SetBooleanArrayRegion(jbooleanArray array, jsize start, jsize len, const jboolean *buf)
{
  ATTACHTHREAD
  jnienv->SetBooleanArrayRegion(array,start,len,buf);
}
void SetByteArrayRegion(jbyteArray array, jsize start, jsize len, const jbyte *buf)
{
  ATTACHTHREAD
  jnienv->SetByteArrayRegion(array,start,len,buf);
}
void SetCharArrayRegion(jcharArray array, jsize start, jsize len, const jchar *buf)
{
  ATTACHTHREAD
  jnienv->SetCharArrayRegion(array,start,len,buf);
}
void SetShortArrayRegion(jshortArray array, jsize start, jsize len, const jshort *buf)
{
  ATTACHTHREAD
  jnienv->SetShortArrayRegion(array,start,len,buf);
}
void SetIntArrayRegion(jintArray array, jsize start, jsize len, const jint *buf)
{
  ATTACHTHREAD
  jnienv->SetIntArrayRegion(array,start,len,buf);
}
void SetLongArrayRegion(jlongArray array, jsize start, jsize len, const jlong *buf)
{
  ATTACHTHREAD
  jnienv->SetLongArrayRegion(array,start,len,buf);
}
void SetFloatArrayRegion(jfloatArray array, jsize start, jsize len, const jfloat *buf)
{
  ATTACHTHREAD
  jnienv->SetFloatArrayRegion(array,start,len,buf);
}
void SetDoubleArrayRegion(jdoubleArray array, jsize start, jsize len, const jdouble *buf)
{
  ATTACHTHREAD
  jnienv->SetDoubleArrayRegion(array,start,len,buf);
}
jint RegisterNatives(jclass clazz, const JNINativeMethod *methods, jint nMethods)
{
  ATTACHTHREAD
  return jnienv->RegisterNatives(clazz,methods,nMethods);
}
jint UnregisterNatives(jclass clazz)
{
  ATTACHTHREAD
  return jnienv->UnregisterNatives(clazz);
}
jint MonitorEnter(jobject obj)
{
  ATTACHTHREAD
  return jnienv->MonitorEnter(obj);
}
jint MonitorExit(jobject obj)
{
  ATTACHTHREAD
  return jnienv->MonitorExit(obj);
}
jint GetJavaVM(JavaVM **vm)
{
  ATTACHTHREAD
  return jnienv->GetJavaVM(vm);
}
void GetStringRegion(jstring str, jsize start, jsize len, jchar *buf)
{
  ATTACHTHREAD
  jnienv->GetStringRegion(str,start,len,buf);
}
void GetStringUTFRegion(jstring str, jsize start, jsize len, char *buf)
{
  ATTACHTHREAD
  jnienv->GetStringUTFRegion(str,start,len,buf);
}
void * GetPrimitiveArrayCritical(jarray array, jboolean *isCopy)
{
  ATTACHTHREAD
  return jnienv->GetPrimitiveArrayCritical(array,isCopy);
}
void ReleasePrimitiveArrayCritical(jarray array, void *carray, jint mode)
{
  ATTACHTHREAD
  jnienv->ReleasePrimitiveArrayCritical(array,carray,mode);
}
const jchar * GetStringCritical(jstring string, jboolean *isCopy)
{
  ATTACHTHREAD
  return jnienv->GetStringCritical(string,isCopy);
}
void ReleaseStringCritical(jstring string, const jchar *cstring)
{
  ATTACHTHREAD
  jnienv->ReleaseStringCritical(string,cstring);
}
jweak NewWeakGlobalRef(jobject obj)
{
  ATTACHTHREAD
  return jnienv->NewWeakGlobalRef(obj);
}
void DeleteWeakGlobalRef(jweak ref)
{
  ATTACHTHREAD
  jnienv->DeleteWeakGlobalRef(ref);
}
jboolean ExceptionCheck()
{
  ATTACHTHREAD
  return jnienv->ExceptionCheck();
}
jobject NewDirectByteBuffer(void* address, jlong capacity)
{
  ATTACHTHREAD
  return jnienv->NewDirectByteBuffer(address, capacity);
}
void* GetDirectBufferAddress(jobject buf)
{
  ATTACHTHREAD
  return jnienv->GetDirectBufferAddress(buf);
}
jlong GetDirectBufferCapacity(jobject buf)
{
  ATTACHTHREAD
  return jnienv->GetDirectBufferCapacity(buf);
}
jobjectRefType GetObjectRefType(jobject obj)
{
  ATTACHTHREAD
  return jnienv->GetObjectRefType(obj);
}
#endif
