#include <QApplication>
#include <QDebug>
#include "qtjni.h"
#include "base.h"
static JavaVM *m_javaVM=0;
static JNIEnv *jnienv=0;
static jclass qtapp = 0;
static jclass qtact= 0;

#if 0
#define ATTACHTHREAD
#define DETACHTHREAD
#else
#define ATTACHTHREAD m_javaVM->AttachCurrentThread(&jnienv, NULL);
#define DETACHTHREAD m_javaVM->DetachCurrentThread();
#endif

void javaOnLoad(JavaVM * vm, JNIEnv * env)
{
  m_javaVM=vm;
  jnienv=env;
  jclass ap,ac;
  qDebug() << "JNI_OnLoad vm " << QString::number((long)vm);
  qDebug() << "JNI_OnLoad env " << QString::number((long)jnienv);
  ap=jnienv->FindClass("org/qtproject/qt5/android/bindings/QtApplication");
  ac=jnienv->FindClass("org/qtproject/qt5/android/bindings/QtActivity");
  qtapp=(jclass)jnienv->NewGlobalRef(ap);
  qtact=(jclass)jnienv->NewGlobalRef(ac);
  jnienv->DeleteLocalRef(ap);
  jnienv->DeleteLocalRef(ac);
  qDebug() << "org/qtproject/qt5/android/bindings/QtApplication jclass " << QString::number((long)qtapp);
  qDebug() << "org/qtproject/qt5/android/bindings/QtActivity jclass " << QString::number((long)qtact);
}

int GetJavaVMENV(JavaVM ** pvm, JNIEnv ** penv)
{
  *pvm = m_javaVM;
  *penv = jnienv;
  return 0;
}

jclass GetClassID(int id)
{
  ATTACHTHREAD
  jclass z=0;
  switch (id) {
  case 0 :
    z=(jclass)jnienv->NewLocalRef((jobject)qtapp);
    qDebug() << "qtapp " << QString::number((long)qtapp);
    break;
  case 1 :
    z=(jclass)jnienv->NewLocalRef((jobject)qtact);
    qDebug() << "qtact " << QString::number((long)qtact);
    break;
  }
  DETACHTHREAD
  return z;
}

jint GetVersion()
{
  ATTACHTHREAD
  return jnienv->functions->GetVersion(jnienv);
}
jclass DefineClass(const char *name, jobject loader, const jbyte *buf, jsize len)
{
  ATTACHTHREAD
  return jnienv->functions->DefineClass(jnienv, name, loader, buf, len);
}
jclass FindClass(const char *name)
{
  ATTACHTHREAD
  return jnienv->functions->FindClass(jnienv, name);
}
jmethodID FromReflectedMethod(jobject method)
{
  ATTACHTHREAD
  return jnienv->functions->FromReflectedMethod(jnienv,method);
}
jfieldID FromReflectedField(jobject field)
{
  ATTACHTHREAD
  return jnienv->functions->FromReflectedField(jnienv,field);
}
jobject ToReflectedMethod(jclass cls, jmethodID methodID, jboolean isStatic)
{
  ATTACHTHREAD
  return jnienv->functions->ToReflectedMethod(jnienv, cls, methodID, isStatic);
}
jclass GetSuperclass(jclass sub)
{
  ATTACHTHREAD
  return jnienv->functions->GetSuperclass(jnienv, sub);
}
jboolean IsAssignableFrom(jclass sub, jclass sup)
{
  ATTACHTHREAD
  return jnienv->functions->IsAssignableFrom(jnienv, sub, sup);
}
jobject ToReflectedField(jclass cls, jfieldID fieldID, jboolean isStatic)
{
  ATTACHTHREAD
  return jnienv->functions->ToReflectedField(jnienv,cls,fieldID,isStatic);
}
jint Throw(jthrowable obj)
{
  ATTACHTHREAD
  return jnienv->functions->Throw(jnienv, obj);
}
jint ThrowNew(jclass clazz, const char *msg)
{
  ATTACHTHREAD
  return jnienv->functions->ThrowNew(jnienv, clazz, msg);
}
jthrowable ExceptionOccurred()
{
  ATTACHTHREAD
  return jnienv->functions->ExceptionOccurred(jnienv);
}
void ExceptionDescribe()
{
  ATTACHTHREAD
  jnienv->functions->ExceptionDescribe(jnienv);
}
void ExceptionClear()
{
  ATTACHTHREAD
  jnienv->functions->ExceptionClear(jnienv);
}
void FatalError(const char *msg)
{
  ATTACHTHREAD
  jnienv->functions->FatalError(jnienv, msg);
}
jint PushLocalFrame(jint capacity)
{
  ATTACHTHREAD
  return jnienv->functions->PushLocalFrame(jnienv,capacity);
}
jobject PopLocalFrame(jobject result)
{
  ATTACHTHREAD
  return jnienv->functions->PopLocalFrame(jnienv,result);
}
jobject NewGlobalRef(jobject lobj)
{
  ATTACHTHREAD
  return jnienv->functions->NewGlobalRef(jnienv,lobj);
}
void DeleteGlobalRef(jobject gref)
{
  ATTACHTHREAD
  jnienv->functions->DeleteGlobalRef(jnienv,gref);
}
void DeleteLocalRef(jobject obj)
{
  ATTACHTHREAD
  jnienv->functions->DeleteLocalRef(jnienv, obj);
}
jboolean IsSameObject(jobject obj1, jobject obj2)
{
  ATTACHTHREAD
  return jnienv->functions->IsSameObject(jnienv,obj1,obj2);
}
jobject NewLocalRef(jobject ref)
{
  ATTACHTHREAD
  return jnienv->functions->NewLocalRef(jnienv,ref);
}
jint EnsureLocalCapacity(jint capacity)
{
  ATTACHTHREAD
  return jnienv->functions->EnsureLocalCapacity(jnienv,capacity);
}
jobject AllocObject(jclass clazz)
{
  ATTACHTHREAD
  return jnienv->functions->AllocObject(jnienv,clazz);
}
jobject NewObject(jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jobject result;
  va_start(args, methodID);
  result = jnienv->functions->NewObjectV(jnienv,clazz,methodID,args);
  va_end(args);
  return result;
}
jobject NewObjectV(jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->functions->NewObjectV(jnienv,clazz,methodID,args);
}
jobject NewObjectA(jclass clazz, jmethodID methodID, jvalue *args)
{
  ATTACHTHREAD
  return jnienv->functions->NewObjectA(jnienv,clazz,methodID,args);
}
jclass GetObjectClass(jobject obj)
{
  ATTACHTHREAD
  return jnienv->functions->GetObjectClass(jnienv,obj);
}
jboolean IsInstanceOf(jobject obj, jclass clazz)
{
  ATTACHTHREAD
  return jnienv->functions->IsInstanceOf(jnienv,obj,clazz);
}
jmethodID GetMethodID(jclass clazz, const char *name, const char *sig)
{
  ATTACHTHREAD
  return jnienv->functions->GetMethodID(jnienv,clazz,name,sig);
}
jobject CallObjectMethod(jobject obj, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jobject result;
  va_start(args,methodID);
  result = jnienv->functions->CallObjectMethodV(jnienv,obj,methodID,args);
  va_end(args);
  return result;
}
jobject CallObjectMethodV(jobject obj, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->functions->CallObjectMethodV(jnienv,obj,methodID,args);
}
jobject CallObjectMethodA(jobject obj, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->functions->CallObjectMethodA(jnienv,obj,methodID,args);
}
jboolean CallBooleanMethod(jobject obj, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jboolean result;
  va_start(args,methodID);
  result = jnienv->functions->CallBooleanMethodV(jnienv,obj,methodID,args);
  va_end(args);
  return result;
}
jboolean CallBooleanMethodV(jobject obj, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->functions->CallBooleanMethodV(jnienv,obj,methodID,args);
}
jboolean CallBooleanMethodA(jobject obj, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->functions->CallBooleanMethodA(jnienv,obj,methodID, args);
}
jbyte CallByteMethod(jobject obj, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jbyte result;
  va_start(args,methodID);
  result = jnienv->functions->CallByteMethodV(jnienv,obj,methodID,args);
  va_end(args);
  return result;
}
jbyte CallByteMethodV(jobject obj, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->functions->CallByteMethodV(jnienv,obj,methodID,args);
}
jbyte CallByteMethodA(jobject obj, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->functions->CallByteMethodA(jnienv,obj,methodID,args);
}
jchar CallCharMethod(jobject obj, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jchar result;
  va_start(args,methodID);
  result = jnienv->functions->CallCharMethodV(jnienv,obj,methodID,args);
  va_end(args);
  return result;
}
jchar CallCharMethodV(jobject obj, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->functions->CallCharMethodV(jnienv,obj,methodID,args);
}
jchar CallCharMethodA(jobject obj, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->functions->CallCharMethodA(jnienv,obj,methodID,args);
}
jshort CallShortMethod(jobject obj, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jshort result;
  va_start(args,methodID);
  result = jnienv->functions->CallShortMethodV(jnienv,obj,methodID,args);
  va_end(args);
  return result;
}
jshort CallShortMethodV(jobject obj, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->functions->CallShortMethodV(jnienv,obj,methodID,args);
}
jshort CallShortMethodA(jobject obj, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->functions->CallShortMethodA(jnienv,obj,methodID,args);
}
jint CallIntMethod(jobject obj, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jint result;
  va_start(args,methodID);
  result = jnienv->functions->CallIntMethodV(jnienv,obj,methodID,args);
  va_end(args);
  return result;
}
jint CallIntMethodV(jobject obj, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->functions->CallIntMethodV(jnienv,obj,methodID,args);
}
jint CallIntMethodA(jobject obj, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->functions->CallIntMethodA(jnienv,obj,methodID,args);
}
jlong CallLongMethod(jobject obj, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jlong result;
  va_start(args,methodID);
  result = jnienv->functions->CallLongMethodV(jnienv,obj,methodID,args);
  va_end(args);
  return result;
}
jlong CallLongMethodV(jobject obj, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->functions->CallLongMethodV(jnienv,obj,methodID,args);
}
jlong CallLongMethodA(jobject obj, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->functions->CallLongMethodA(jnienv,obj,methodID,args);
}
jfloat CallFloatMethod(jobject obj, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jfloat result;
  va_start(args,methodID);
  result = jnienv->functions->CallFloatMethodV(jnienv,obj,methodID,args);
  va_end(args);
  return result;
}
jfloat CallFloatMethodV(jobject obj, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->functions->CallFloatMethodV(jnienv,obj,methodID,args);
}
jfloat CallFloatMethodA(jobject obj, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->functions->CallFloatMethodA(jnienv,obj,methodID,args);
}
jdouble CallDoubleMethod(jobject obj, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jdouble result;
  va_start(args,methodID);
  result = jnienv->functions->CallDoubleMethodV(jnienv,obj,methodID,args);
  va_end(args);
  return result;
}
jdouble CallDoubleMethodV(jobject obj, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->functions->CallDoubleMethodV(jnienv,obj,methodID,args);
}
jdouble CallDoubleMethodA(jobject obj, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->functions->CallDoubleMethodA(jnienv,obj,methodID,args);
}
void CallVoidMethod(jobject obj, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  va_start(args,methodID);
  jnienv->functions->CallVoidMethodV(jnienv,obj,methodID,args);
  va_end(args);
}
void CallVoidMethodV(jobject obj, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  jnienv->functions->CallVoidMethodV(jnienv,obj,methodID,args);
}
void CallVoidMethodA(jobject obj, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  jnienv->functions->CallVoidMethodA(jnienv,obj,methodID,args);
}
jobject CallNonvirtualObjectMethod(jobject obj, jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jobject result;
  va_start(args,methodID);
  result = jnienv->functions->CallNonvirtualObjectMethodV(jnienv,obj,clazz, methodID,args);
  va_end(args);
  return result;
}
jobject CallNonvirtualObjectMethodV(jobject obj, jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->functions->CallNonvirtualObjectMethodV(jnienv,obj,clazz, methodID,args);
}
jobject CallNonvirtualObjectMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->functions->CallNonvirtualObjectMethodA(jnienv,obj,clazz, methodID,args);
}
jboolean CallNonvirtualBooleanMethod(jobject obj, jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jboolean result;
  va_start(args,methodID);
  result = jnienv->functions->CallNonvirtualBooleanMethodV(jnienv,obj,clazz, methodID,args);
  va_end(args);
  return result;
}
jboolean CallNonvirtualBooleanMethodV(jobject obj, jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->functions->CallNonvirtualBooleanMethodV(jnienv,obj,clazz, methodID,args);
}
jboolean CallNonvirtualBooleanMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->functions->CallNonvirtualBooleanMethodA(jnienv,obj,clazz, methodID, args);
}
jbyte CallNonvirtualByteMethod(jobject obj, jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jbyte result;
  va_start(args,methodID);
  result = jnienv->functions->CallNonvirtualByteMethodV(jnienv,obj,clazz, methodID,args);
  va_end(args);
  return result;
}
jbyte CallNonvirtualByteMethodV(jobject obj, jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->functions->CallNonvirtualByteMethodV(jnienv,obj,clazz, methodID,args);
}
jbyte CallNonvirtualByteMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->functions->CallNonvirtualByteMethodA(jnienv,obj,clazz, methodID,args);
}
jchar CallNonvirtualCharMethod(jobject obj, jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jchar result;
  va_start(args,methodID);
  result = jnienv->functions->CallNonvirtualCharMethodV(jnienv,obj,clazz, methodID,args);
  va_end(args);
  return result;
}
jchar CallNonvirtualCharMethodV(jobject obj, jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->functions->CallNonvirtualCharMethodV(jnienv,obj,clazz, methodID,args);
}
jchar CallNonvirtualCharMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->functions->CallNonvirtualCharMethodA(jnienv,obj,clazz, methodID,args);
}
jshort CallNonvirtualShortMethod(jobject obj, jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jshort result;
  va_start(args,methodID);
  result = jnienv->functions->CallNonvirtualShortMethodV(jnienv,obj,clazz, methodID,args);
  va_end(args);
  return result;
}
jshort CallNonvirtualShortMethodV(jobject obj, jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->functions->CallNonvirtualShortMethodV(jnienv,obj,clazz, methodID,args);
}
jshort CallNonvirtualShortMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->functions->CallNonvirtualShortMethodA(jnienv,obj,clazz, methodID,args);
}
jint CallNonvirtualIntMethod(jobject obj, jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jint result;
  va_start(args,methodID);
  result = jnienv->functions->CallNonvirtualIntMethodV(jnienv,obj,clazz, methodID,args);
  va_end(args);
  return result;
}
jint CallNonvirtualIntMethodV(jobject obj, jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->functions->CallNonvirtualIntMethodV(jnienv,obj,clazz, methodID,args);
}
jint CallNonvirtualIntMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->functions->CallNonvirtualIntMethodA(jnienv,obj,clazz, methodID,args);
}
jlong CallNonvirtualLongMethod(jobject obj, jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jlong result;
  va_start(args,methodID);
  result = jnienv->functions->CallNonvirtualLongMethodV(jnienv,obj,clazz, methodID,args);
  va_end(args);
  return result;
}
jlong CallNonvirtualLongMethodV(jobject obj, jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->functions->CallNonvirtualLongMethodV(jnienv,obj,clazz, methodID,args);
}
jlong CallNonvirtualLongMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->functions->CallNonvirtualLongMethodA(jnienv,obj,clazz, methodID,args);
}
jfloat CallNonvirtualFloatMethod(jobject obj, jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jfloat result;
  va_start(args,methodID);
  result = jnienv->functions->CallNonvirtualFloatMethodV(jnienv,obj,clazz, methodID,args);
  va_end(args);
  return result;
}
jfloat CallNonvirtualFloatMethodV(jobject obj, jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->functions->CallNonvirtualFloatMethodV(jnienv,obj,clazz, methodID,args);
}
jfloat CallNonvirtualFloatMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->functions->CallNonvirtualFloatMethodA(jnienv,obj,clazz, methodID,args);
}
jdouble CallNonvirtualDoubleMethod(jobject obj, jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jdouble result;
  va_start(args,methodID);
  result = jnienv->functions->CallNonvirtualDoubleMethodV(jnienv,obj,clazz, methodID,args);
  va_end(args);
  return result;
}
jdouble CallNonvirtualDoubleMethodV(jobject obj, jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->functions->CallNonvirtualDoubleMethodV(jnienv,obj,clazz, methodID,args);
}
jdouble CallNonvirtualDoubleMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  return jnienv->functions->CallNonvirtualDoubleMethodA(jnienv,obj,clazz, methodID,args);
}
void CallNonvirtualVoidMethod(jobject obj, jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  va_start(args,methodID);
  jnienv->functions->CallNonvirtualVoidMethodV(jnienv,obj,clazz,methodID,args);
  va_end(args);
}
void CallNonvirtualVoidMethodV(jobject obj, jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  jnienv->functions->CallNonvirtualVoidMethodV(jnienv,obj,clazz,methodID,args);
}
void CallNonvirtualVoidMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  jnienv->functions->CallNonvirtualVoidMethodA(jnienv,obj,clazz,methodID,args);
}
jfieldID GetFieldID(jclass clazz, const char *name, const char *sig)
{
  ATTACHTHREAD
  return jnienv->functions->GetFieldID(jnienv,clazz,name,sig);
}
jobject GetObjectField(jobject obj, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->functions->GetObjectField(jnienv,obj,fieldID);
}
jboolean GetBooleanField(jobject obj, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->functions->GetBooleanField(jnienv,obj,fieldID);
}
jbyte GetByteField(jobject obj, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->functions->GetByteField(jnienv,obj,fieldID);
}
jchar GetCharField(jobject obj, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->functions->GetCharField(jnienv,obj,fieldID);
}
jshort GetShortField(jobject obj, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->functions->GetShortField(jnienv,obj,fieldID);
}
jint GetIntField(jobject obj, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->functions->GetIntField(jnienv,obj,fieldID);
}
jlong GetLongField(jobject obj, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->functions->GetLongField(jnienv,obj,fieldID);
}
jfloat GetFloatField(jobject obj, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->functions->GetFloatField(jnienv,obj,fieldID);
}
jdouble GetDoubleField(jobject obj, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->functions->GetDoubleField(jnienv,obj,fieldID);
}
void SetObjectField(jobject obj, jfieldID fieldID, jobject val)
{
  ATTACHTHREAD
  jnienv->functions->SetObjectField(jnienv,obj,fieldID,val);
}
void SetBooleanField(jobject obj, jfieldID fieldID, jboolean val)
{
  ATTACHTHREAD
  jnienv->functions->SetBooleanField(jnienv,obj,fieldID,val);
}
void SetByteField(jobject obj, jfieldID fieldID, jbyte val)
{
  ATTACHTHREAD
  jnienv->functions->SetByteField(jnienv,obj,fieldID,val);
}
void SetCharField(jobject obj, jfieldID fieldID, jchar val)
{
  ATTACHTHREAD
  jnienv->functions->SetCharField(jnienv,obj,fieldID,val);
}
void SetShortField(jobject obj, jfieldID fieldID, jshort val)
{
  ATTACHTHREAD
  jnienv->functions->SetShortField(jnienv,obj,fieldID,val);
}
void SetIntField(jobject obj, jfieldID fieldID, jint val)
{
  ATTACHTHREAD
  jnienv->functions->SetIntField(jnienv,obj,fieldID,val);
}
void SetLongField(jobject obj, jfieldID fieldID, jlong val)
{
  ATTACHTHREAD
  jnienv->functions->SetLongField(jnienv,obj,fieldID,val);
}
void SetFloatField(jobject obj, jfieldID fieldID, jfloat val)
{
  ATTACHTHREAD
  jnienv->functions->SetFloatField(jnienv,obj,fieldID,val);
}
void SetDoubleField(jobject obj, jfieldID fieldID, jdouble val)
{
  ATTACHTHREAD
  jnienv->functions->SetDoubleField(jnienv,obj,fieldID,val);
}
jmethodID GetStaticMethodID(jclass clazz, const char *name, const char *sig)
{
  ATTACHTHREAD
  return jnienv->functions->GetStaticMethodID(jnienv,clazz,name,sig);
}
jobject CallStaticObjectMethod(jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jobject result;
  va_start(args,methodID);
  result = jnienv->functions->CallStaticObjectMethodV(jnienv,clazz,methodID,args);
  va_end(args);
  return result;
}
jobject CallStaticObjectMethodV(jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->functions->CallStaticObjectMethodV(jnienv,clazz,methodID,args);
}
jobject CallStaticObjectMethodA(jclass clazz, jmethodID methodID, jvalue *args)
{
  ATTACHTHREAD
  return jnienv->functions->CallStaticObjectMethodA(jnienv,clazz,methodID,args);
}
jboolean CallStaticBooleanMethod(jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jboolean result;
  va_start(args,methodID);
  result = jnienv->functions->CallStaticBooleanMethodV(jnienv,clazz,methodID,args);
  va_end(args);
  return result;
}
jboolean CallStaticBooleanMethodV(jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->functions->CallStaticBooleanMethodV(jnienv,clazz,methodID,args);
}
jboolean CallStaticBooleanMethodA(jclass clazz, jmethodID methodID, jvalue *args)
{
  ATTACHTHREAD
  return jnienv->functions->CallStaticBooleanMethodA(jnienv,clazz,methodID,args);
}
jbyte CallStaticByteMethod(jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jbyte result;
  va_start(args,methodID);
  result = jnienv->functions->CallStaticByteMethodV(jnienv,clazz,methodID,args);
  va_end(args);
  return result;
}
jbyte CallStaticByteMethodV(jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->functions->CallStaticByteMethodV(jnienv,clazz,methodID,args);
}
jbyte CallStaticByteMethodA(jclass clazz, jmethodID methodID, jvalue *args)
{
  ATTACHTHREAD
  return jnienv->functions->CallStaticByteMethodA(jnienv,clazz,methodID,args);
}
jchar CallStaticCharMethod(jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jchar result;
  va_start(args,methodID);
  result = jnienv->functions->CallStaticCharMethodV(jnienv,clazz,methodID,args);
  va_end(args);
  return result;
}
jchar CallStaticCharMethodV(jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->functions->CallStaticCharMethodV(jnienv,clazz,methodID,args);
}
jchar CallStaticCharMethodA(jclass clazz, jmethodID methodID, jvalue *args)
{
  ATTACHTHREAD
  return jnienv->functions->CallStaticCharMethodA(jnienv,clazz,methodID,args);
}
jshort CallStaticShortMethod(jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jshort result;
  va_start(args,methodID);
  result = jnienv->functions->CallStaticShortMethodV(jnienv,clazz,methodID,args);
  va_end(args);
  return result;
}
jshort CallStaticShortMethodV(jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->functions->CallStaticShortMethodV(jnienv,clazz,methodID,args);
}
jshort CallStaticShortMethodA(jclass clazz, jmethodID methodID, jvalue *args)
{
  ATTACHTHREAD
  return jnienv->functions->CallStaticShortMethodA(jnienv,clazz,methodID,args);
}
jint CallStaticIntMethod(jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jint result;
  va_start(args,methodID);
  result = jnienv->functions->CallStaticIntMethodV(jnienv,clazz,methodID,args);
  va_end(args);
  return result;
}
jint CallStaticIntMethodV(jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->functions->CallStaticIntMethodV(jnienv,clazz,methodID,args);
}
jint CallStaticIntMethodA(jclass clazz, jmethodID methodID, jvalue *args)
{
  ATTACHTHREAD
  return jnienv->functions->CallStaticIntMethodA(jnienv,clazz,methodID,args);
}
jlong CallStaticLongMethod(jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jlong result;
  va_start(args,methodID);
  result = jnienv->functions->CallStaticLongMethodV(jnienv,clazz,methodID,args);
  va_end(args);
  return result;
}
jlong CallStaticLongMethodV(jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->functions->CallStaticLongMethodV(jnienv,clazz,methodID,args);
}
jlong CallStaticLongMethodA(jclass clazz, jmethodID methodID, jvalue *args)
{
  ATTACHTHREAD
  return jnienv->functions->CallStaticLongMethodA(jnienv,clazz,methodID,args);
}
jfloat CallStaticFloatMethod(jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jfloat result;
  va_start(args,methodID);
  result = jnienv->functions->CallStaticFloatMethodV(jnienv,clazz,methodID,args);
  va_end(args);
  return result;
}
jfloat CallStaticFloatMethodV(jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->functions->CallStaticFloatMethodV(jnienv,clazz,methodID,args);
}
jfloat CallStaticFloatMethodA(jclass clazz, jmethodID methodID, jvalue *args)
{
  ATTACHTHREAD
  return jnienv->functions->CallStaticFloatMethodA(jnienv,clazz,methodID,args);
}
jdouble CallStaticDoubleMethod(jclass clazz, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  jdouble result;
  va_start(args,methodID);
  result = jnienv->functions->CallStaticDoubleMethodV(jnienv,clazz,methodID,args);
  va_end(args);
  return result;
}
jdouble CallStaticDoubleMethodV(jclass clazz, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  return jnienv->functions->CallStaticDoubleMethodV(jnienv,clazz,methodID,args);
}
jdouble CallStaticDoubleMethodA(jclass clazz, jmethodID methodID, jvalue *args)
{
  ATTACHTHREAD
  return jnienv->functions->CallStaticDoubleMethodA(jnienv,clazz,methodID,args);
}
void CallStaticVoidMethod(jclass cls, jmethodID methodID, ...)
{
  ATTACHTHREAD
  va_list args;
  va_start(args,methodID);
  jnienv->functions->CallStaticVoidMethodV(jnienv,cls,methodID,args);
  va_end(args);
}
void CallStaticVoidMethodV(jclass cls, jmethodID methodID, va_list args)
{
  ATTACHTHREAD
  jnienv->functions->CallStaticVoidMethodV(jnienv,cls,methodID,args);
}
void CallStaticVoidMethodA(jclass cls, jmethodID methodID, jvalue * args)
{
  ATTACHTHREAD
  jnienv->functions->CallStaticVoidMethodA(jnienv,cls,methodID,args);
}
jfieldID GetStaticFieldID(jclass clazz, const char *name, const char *sig)
{
  ATTACHTHREAD
  return jnienv->functions->GetStaticFieldID(jnienv,clazz,name,sig);
}
jobject GetStaticObjectField(jclass clazz, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->functions->GetStaticObjectField(jnienv,clazz,fieldID);
}
jboolean GetStaticBooleanField(jclass clazz, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->functions->GetStaticBooleanField(jnienv,clazz,fieldID);
}
jbyte GetStaticByteField(jclass clazz, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->functions->GetStaticByteField(jnienv,clazz,fieldID);
}
jchar GetStaticCharField(jclass clazz, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->functions->GetStaticCharField(jnienv,clazz,fieldID);
}
jshort GetStaticShortField(jclass clazz, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->functions->GetStaticShortField(jnienv,clazz,fieldID);
}
jint GetStaticIntField(jclass clazz, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->functions->GetStaticIntField(jnienv,clazz,fieldID);
}
jlong GetStaticLongField(jclass clazz, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->functions->GetStaticLongField(jnienv,clazz,fieldID);
}
jfloat GetStaticFloatField(jclass clazz, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->functions->GetStaticFloatField(jnienv,clazz,fieldID);
}
jdouble GetStaticDoubleField(jclass clazz, jfieldID fieldID)
{
  ATTACHTHREAD
  return jnienv->functions->GetStaticDoubleField(jnienv,clazz,fieldID);
}
void SetStaticObjectField(jclass clazz, jfieldID fieldID, jobject value)
{
  ATTACHTHREAD
  jnienv->functions->SetStaticObjectField(jnienv,clazz,fieldID,value);
}
void SetStaticBooleanField(jclass clazz, jfieldID fieldID, jboolean value)
{
  ATTACHTHREAD
  jnienv->functions->SetStaticBooleanField(jnienv,clazz,fieldID,value);
}
void SetStaticByteField(jclass clazz, jfieldID fieldID, jbyte value)
{
  ATTACHTHREAD
  jnienv->functions->SetStaticByteField(jnienv,clazz,fieldID,value);
}
void SetStaticCharField(jclass clazz, jfieldID fieldID, jchar value)
{
  ATTACHTHREAD
  jnienv->functions->SetStaticCharField(jnienv,clazz,fieldID,value);
}
void SetStaticShortField(jclass clazz, jfieldID fieldID, jshort value)
{
  ATTACHTHREAD
  jnienv->functions->SetStaticShortField(jnienv,clazz,fieldID,value);
}
void SetStaticIntField(jclass clazz, jfieldID fieldID, jint value)
{
  ATTACHTHREAD
  jnienv->functions->SetStaticIntField(jnienv,clazz,fieldID,value);
}
void SetStaticLongField(jclass clazz, jfieldID fieldID, jlong value)
{
  ATTACHTHREAD
  jnienv->functions->SetStaticLongField(jnienv,clazz,fieldID,value);
}
void SetStaticFloatField(jclass clazz, jfieldID fieldID, jfloat value)
{
  ATTACHTHREAD
  jnienv->functions->SetStaticFloatField(jnienv,clazz,fieldID,value);
}
void SetStaticDoubleField(jclass clazz, jfieldID fieldID, jdouble value)
{
  ATTACHTHREAD
  jnienv->functions->SetStaticDoubleField(jnienv,clazz,fieldID,value);
}
jstring NewString(const jchar *unicode, jsize len)
{
  ATTACHTHREAD
  return jnienv->functions->NewString(jnienv,unicode,len);
}
jsize GetStringLength(jstring str)
{
  ATTACHTHREAD
  return jnienv->functions->GetStringLength(jnienv,str);
}
const jchar *GetStringChars(jstring str, jboolean *isCopy)
{
  ATTACHTHREAD
  return jnienv->functions->GetStringChars(jnienv,str,isCopy);
}
void ReleaseStringChars(jstring str, const jchar *chars)
{
  ATTACHTHREAD
  jnienv->functions->ReleaseStringChars(jnienv,str,chars);
}
jstring NewStringUTF(const char *utf)
{
  ATTACHTHREAD
  return jnienv->functions->NewStringUTF(jnienv,utf);
}
jsize GetStringUTFLength(jstring str)
{
  ATTACHTHREAD
  return jnienv->functions->GetStringUTFLength(jnienv,str);
}
const char* GetStringUTFChars(jstring str, jboolean *isCopy)
{
  ATTACHTHREAD
  return jnienv->functions->GetStringUTFChars(jnienv,str,isCopy);
}
void ReleaseStringUTFChars(jstring str, const char* chars)
{
  ATTACHTHREAD
  jnienv->functions->ReleaseStringUTFChars(jnienv,str,chars);
}
jsize GetArrayLength(jarray array)
{
  ATTACHTHREAD
  return jnienv->functions->GetArrayLength(jnienv,array);
}
jobjectArray NewObjectArray(jsize len, jclass clazz, jobject init)
{
  ATTACHTHREAD
  return jnienv->functions->NewObjectArray(jnienv,len,clazz,init);
}
jobject GetObjectArrayElement(jobjectArray array, jsize index)
{
  ATTACHTHREAD
  return jnienv->functions->GetObjectArrayElement(jnienv,array,index);
}
void SetObjectArrayElement(jobjectArray array, jsize index, jobject val)
{
  ATTACHTHREAD
  jnienv->functions->SetObjectArrayElement(jnienv,array,index,val);
}
jbooleanArray NewBooleanArray(jsize len)
{
  ATTACHTHREAD
  return jnienv->functions->NewBooleanArray(jnienv,len);
}
jbyteArray NewByteArray(jsize len)
{
  ATTACHTHREAD
  return jnienv->functions->NewByteArray(jnienv,len);
}
jcharArray NewCharArray(jsize len)
{
  ATTACHTHREAD
  return jnienv->functions->NewCharArray(jnienv,len);
}
jshortArray NewShortArray(jsize len)
{
  ATTACHTHREAD
  return jnienv->functions->NewShortArray(jnienv,len);
}
jintArray NewIntArray(jsize len)
{
  ATTACHTHREAD
  return jnienv->functions->NewIntArray(jnienv,len);
}
jlongArray NewLongArray(jsize len)
{
  ATTACHTHREAD
  return jnienv->functions->NewLongArray(jnienv,len);
}
jfloatArray NewFloatArray(jsize len)
{
  ATTACHTHREAD
  return jnienv->functions->NewFloatArray(jnienv,len);
}
jdoubleArray NewDoubleArray(jsize len)
{
  ATTACHTHREAD
  return jnienv->functions->NewDoubleArray(jnienv,len);
}
jboolean * GetBooleanArrayElements(jbooleanArray array, jboolean *isCopy)
{
  ATTACHTHREAD
  return jnienv->functions->GetBooleanArrayElements(jnienv,array,isCopy);
}
jbyte * GetByteArrayElements(jbyteArray array, jboolean *isCopy)
{
  ATTACHTHREAD
  return jnienv->functions->GetByteArrayElements(jnienv,array,isCopy);
}
jchar * GetCharArrayElements(jcharArray array, jboolean *isCopy)
{
  ATTACHTHREAD
  return jnienv->functions->GetCharArrayElements(jnienv,array,isCopy);
}
jshort * GetShortArrayElements(jshortArray array, jboolean *isCopy)
{
  ATTACHTHREAD
  return jnienv->functions->GetShortArrayElements(jnienv,array,isCopy);
}
jint * GetIntArrayElements(jintArray array, jboolean *isCopy)
{
  ATTACHTHREAD
  return jnienv->functions->GetIntArrayElements(jnienv,array,isCopy);
}
jlong * GetLongArrayElements(jlongArray array, jboolean *isCopy)
{
  ATTACHTHREAD
  return jnienv->functions->GetLongArrayElements(jnienv,array,isCopy);
}
jfloat * GetFloatArrayElements(jfloatArray array, jboolean *isCopy)
{
  ATTACHTHREAD
  return jnienv->functions->GetFloatArrayElements(jnienv,array,isCopy);
}
jdouble * GetDoubleArrayElements(jdoubleArray array, jboolean *isCopy)
{
  ATTACHTHREAD
  return jnienv->functions->GetDoubleArrayElements(jnienv,array,isCopy);
}
void ReleaseBooleanArrayElements(jbooleanArray array, jboolean *elems, jint mode)
{
  ATTACHTHREAD
  jnienv->functions->ReleaseBooleanArrayElements(jnienv,array,elems,mode);
}
void ReleaseByteArrayElements(jbyteArray array, jbyte *elems, jint mode)
{
  ATTACHTHREAD
  jnienv->functions->ReleaseByteArrayElements(jnienv,array,elems,mode);
}
void ReleaseCharArrayElements(jcharArray array, jchar *elems, jint mode)
{
  ATTACHTHREAD
  jnienv->functions->ReleaseCharArrayElements(jnienv,array,elems,mode);
}
void ReleaseShortArrayElements(jshortArray array, jshort *elems, jint mode)
{
  ATTACHTHREAD
  jnienv->functions->ReleaseShortArrayElements(jnienv,array,elems,mode);
}
void ReleaseIntArrayElements(jintArray array, jint *elems, jint mode)
{
  ATTACHTHREAD
  jnienv->functions->ReleaseIntArrayElements(jnienv,array,elems,mode);
}
void ReleaseLongArrayElements(jlongArray array, jlong *elems, jint mode)
{
  ATTACHTHREAD
  jnienv->functions->ReleaseLongArrayElements(jnienv,array,elems,mode);
}
void ReleaseFloatArrayElements(jfloatArray array, jfloat *elems, jint mode)
{
  ATTACHTHREAD
  jnienv->functions->ReleaseFloatArrayElements(jnienv,array,elems,mode);
}
void ReleaseDoubleArrayElements(jdoubleArray array, jdouble *elems, jint mode)
{
  ATTACHTHREAD
  jnienv->functions->ReleaseDoubleArrayElements(jnienv,array,elems,mode);
}
void GetBooleanArrayRegion(jbooleanArray array, jsize start, jsize len, jboolean *buf)
{
  ATTACHTHREAD
  jnienv->functions->GetBooleanArrayRegion(jnienv,array,start,len,buf);
}
void GetByteArrayRegion(jbyteArray array, jsize start, jsize len, jbyte *buf)
{
  ATTACHTHREAD
  jnienv->functions->GetByteArrayRegion(jnienv,array,start,len,buf);
}
void GetCharArrayRegion(jcharArray array, jsize start, jsize len, jchar *buf)
{
  ATTACHTHREAD
  jnienv->functions->GetCharArrayRegion(jnienv,array,start,len,buf);
}
void GetShortArrayRegion(jshortArray array, jsize start, jsize len, jshort *buf)
{
  ATTACHTHREAD
  jnienv->functions->GetShortArrayRegion(jnienv,array,start,len,buf);
}
void GetIntArrayRegion(jintArray array, jsize start, jsize len, jint *buf)
{
  ATTACHTHREAD
  jnienv->functions->GetIntArrayRegion(jnienv,array,start,len,buf);
}
void GetLongArrayRegion(jlongArray array, jsize start, jsize len, jlong *buf)
{
  ATTACHTHREAD
  jnienv->functions->GetLongArrayRegion(jnienv,array,start,len,buf);
}
void GetFloatArrayRegion(jfloatArray array, jsize start, jsize len, jfloat *buf)
{
  ATTACHTHREAD
  jnienv->functions->GetFloatArrayRegion(jnienv,array,start,len,buf);
}
void GetDoubleArrayRegion(jdoubleArray array, jsize start, jsize len, jdouble *buf)
{
  ATTACHTHREAD
  jnienv->functions->GetDoubleArrayRegion(jnienv,array,start,len,buf);
}
void SetBooleanArrayRegion(jbooleanArray array, jsize start, jsize len, const jboolean *buf)
{
  ATTACHTHREAD
  jnienv->functions->SetBooleanArrayRegion(jnienv,array,start,len,buf);
}
void SetByteArrayRegion(jbyteArray array, jsize start, jsize len, const jbyte *buf)
{
  ATTACHTHREAD
  jnienv->functions->SetByteArrayRegion(jnienv,array,start,len,buf);
}
void SetCharArrayRegion(jcharArray array, jsize start, jsize len, const jchar *buf)
{
  ATTACHTHREAD
  jnienv->functions->SetCharArrayRegion(jnienv,array,start,len,buf);
}
void SetShortArrayRegion(jshortArray array, jsize start, jsize len, const jshort *buf)
{
  ATTACHTHREAD
  jnienv->functions->SetShortArrayRegion(jnienv,array,start,len,buf);
}
void SetIntArrayRegion(jintArray array, jsize start, jsize len, const jint *buf)
{
  ATTACHTHREAD
  jnienv->functions->SetIntArrayRegion(jnienv,array,start,len,buf);
}
void SetLongArrayRegion(jlongArray array, jsize start, jsize len, const jlong *buf)
{
  ATTACHTHREAD
  jnienv->functions->SetLongArrayRegion(jnienv,array,start,len,buf);
}
void SetFloatArrayRegion(jfloatArray array, jsize start, jsize len, const jfloat *buf)
{
  ATTACHTHREAD
  jnienv->functions->SetFloatArrayRegion(jnienv,array,start,len,buf);
}
void SetDoubleArrayRegion(jdoubleArray array, jsize start, jsize len, const jdouble *buf)
{
  ATTACHTHREAD
  jnienv->functions->SetDoubleArrayRegion(jnienv,array,start,len,buf);
}
jint RegisterNatives(jclass clazz, const JNINativeMethod *methods, jint nMethods)
{
  ATTACHTHREAD
  return jnienv->functions->RegisterNatives(jnienv,clazz,methods,nMethods);
}
jint UnregisterNatives(jclass clazz)
{
  ATTACHTHREAD
  return jnienv->functions->UnregisterNatives(jnienv,clazz);
}
jint MonitorEnter(jobject obj)
{
  ATTACHTHREAD
  return jnienv->functions->MonitorEnter(jnienv,obj);
}
jint MonitorExit(jobject obj)
{
  ATTACHTHREAD
  return jnienv->functions->MonitorExit(jnienv,obj);
}
jint GetJavaVM(JavaVM **vm)
{
  ATTACHTHREAD
  return jnienv->functions->GetJavaVM(jnienv,vm);
}
void GetStringRegion(jstring str, jsize start, jsize len, jchar *buf)
{
  ATTACHTHREAD
  jnienv->functions->GetStringRegion(jnienv,str,start,len,buf);
}
void GetStringUTFRegion(jstring str, jsize start, jsize len, char *buf)
{
  ATTACHTHREAD
  jnienv->functions->GetStringUTFRegion(jnienv,str,start,len,buf);
}
void * GetPrimitiveArrayCritical(jarray array, jboolean *isCopy)
{
  ATTACHTHREAD
  return jnienv->functions->GetPrimitiveArrayCritical(jnienv,array,isCopy);
}
void ReleasePrimitiveArrayCritical(jarray array, void *carray, jint mode)
{
  ATTACHTHREAD
  jnienv->functions->ReleasePrimitiveArrayCritical(jnienv,array,carray,mode);
}
const jchar * GetStringCritical(jstring string, jboolean *isCopy)
{
  ATTACHTHREAD
  return jnienv->functions->GetStringCritical(jnienv,string,isCopy);
}
void ReleaseStringCritical(jstring string, const jchar *cstring)
{
  ATTACHTHREAD
  jnienv->functions->ReleaseStringCritical(jnienv,string,cstring);
}
jweak NewWeakGlobalRef(jobject obj)
{
  ATTACHTHREAD
  return jnienv->functions->NewWeakGlobalRef(jnienv,obj);
}
void DeleteWeakGlobalRef(jweak ref)
{
  ATTACHTHREAD
  jnienv->functions->DeleteWeakGlobalRef(jnienv,ref);
}
jboolean ExceptionCheck()
{
  ATTACHTHREAD
  return jnienv->functions->ExceptionCheck(jnienv);
}
jobject NewDirectByteBuffer(void* address, jlong capacity)
{
  ATTACHTHREAD
  return jnienv->functions->NewDirectByteBuffer(jnienv, address, capacity);
}
void* GetDirectBufferAddress(jobject buf)
{
  ATTACHTHREAD
  return jnienv->functions->GetDirectBufferAddress(jnienv, buf);
}
jlong GetDirectBufferCapacity(jobject buf)
{
  ATTACHTHREAD
  return jnienv->functions->GetDirectBufferCapacity(jnienv, buf);
}
jobjectRefType GetObjectRefType(jobject obj)
{
  ATTACHTHREAD
  return jnienv->functions->GetObjectRefType(jnienv, obj);
}
