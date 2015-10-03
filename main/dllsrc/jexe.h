/* Copyright 1990-2000, Jsoftware Inc.  All rights reserved. */
/* Licensed use only. Any other use is in violation of copyright. */
/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Jun 22 11:55:16 2000
 */
/* Compiler settings for default\jexe.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __jexr_h__
#define __jexe_h__

#ifdef __cplusplus
extern "C" {
#endif

/* Forward Declarations */

#ifndef __IJEXEServer_FWD_DEFINED__
#define __IJEXEServer_FWD_DEFINED__
typedef interface IJEXEServer IJEXEServer;
#endif 	/* __IJEXEServer_FWD_DEFINED__ */


#ifndef __JEXEServer_FWD_DEFINED__
#define __JEXEServer_FWD_DEFINED__

#ifdef __cplusplus
typedef class JEXEServer JEXEServer;
#else
typedef struct JEXEServer JEXEServer;
#endif /* __cplusplus */

#endif 	/* __JEXEServer_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(SIZE_T);
void __RPC_USER MIDL_user_free( void __RPC_FAR * );

#ifndef __IJEXEServer_INTERFACE_DEFINED__
#define __IJEXEServer_INTERFACE_DEFINED__

/* interface IJEXEServer */
/* [oleautomation][dual][unique][helpstring][uuid][object] */


EXTERN_C const IID IID_IJEXEServer;

#if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("21EB05E2-1AB3-11cf-A2AC-8FF70874C460")
IJEXEServer :
public IDispatch {
public:
  virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Do(
    /* [in] */ BSTR input,
    /* [retval][out] */ long __RPC_FAR *r) = 0;

  virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Show(
    /* [in] */ long b,
    /* [retval][out] */ long __RPC_FAR *r) = 0;

  virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Log(
    /* [in] */ long b,
    /* [retval][out] */ long __RPC_FAR *r) = 0;

  virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsBusy(
    /* [retval][out] */ long __RPC_FAR *r) = 0;

  virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Break(
    /* [retval][out] */ long __RPC_FAR *r) = 0;

  virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Quit(
    /* [retval][out] */ long __RPC_FAR *r) = 0;

  virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Get(
    /* [in] */ BSTR jname,
    /* [out] */ VARIANT __RPC_FAR *v,
    /* [retval][out] */ long __RPC_FAR *r) = 0;

  virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Set(
    /* [in] */ BSTR jname,
    /* [in] */ VARIANT __RPC_FAR *v,
    /* [retval][out] */ long __RPC_FAR *r) = 0;

  virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetM(
    /* [in] */ BSTR jname,
    /* [out] */ long __RPC_FAR *jtype,
    /* [out] */ long __RPC_FAR *jrank,
    /* [out] */ long __RPC_FAR *jshape,
    /* [out] */ long __RPC_FAR *jdata,
    /* [retval][out] */ long __RPC_FAR *r) = 0;

  virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetM(
    /* [in] */ BSTR jname,
    /* [in] */ long __RPC_FAR *jtype,
    /* [in] */ long __RPC_FAR *jrank,
    /* [in] */ long __RPC_FAR *jshape,
    /* [in] */ long __RPC_FAR *jdata,
    /* [retval][out] */ long __RPC_FAR *r) = 0;

  virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ErrorText(
    /* [in] */ long error,
    /* [out] */ VARIANT __RPC_FAR *v,
    /* [retval][out] */ long __RPC_FAR *r) = 0;

  virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ErrorTextM(
    /* [in] */ long error,
    /* [out] */ long __RPC_FAR *text,
    /* [retval][out] */ long __RPC_FAR *r) = 0;

  virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Clear(
    /* [retval][out] */ long __RPC_FAR *r) = 0;

  virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Transpose(
    /* [in] */ long b,
    /* [retval][out] */ long __RPC_FAR *r) = 0;

  virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ErrorTextB(
    /* [in] */ long error,
    /* [out] */ VARIANT __RPC_FAR *v,
    /* [retval][out] */ long __RPC_FAR *r) = 0;

  virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetB(
    /* [in] */ BSTR jname,
    /* [out] */ VARIANT __RPC_FAR *v,
    /* [retval][out] */ long __RPC_FAR *r) = 0;

  virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetB(
    /* [in] */ BSTR jname,
    /* [in] */ VARIANT __RPC_FAR *v,
    /* [retval][out] */ long __RPC_FAR *r) = 0;

  virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE DoR(
    /* [in] */ BSTR input,
    /* [out] */ VARIANT __RPC_FAR *v,
    /* [retval][out] */ long __RPC_FAR *r) = 0;

};

#else 	/* C style interface */

typedef struct IJEXEServerVtbl {
  BEGIN_INTERFACE

  HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )(
    IJEXEServer __RPC_FAR * This,
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

  ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )(
    IJEXEServer __RPC_FAR * This);

  ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )(
    IJEXEServer __RPC_FAR * This);

  HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )(
    IJEXEServer __RPC_FAR * This,
    /* [out] */ UINT __RPC_FAR *pctinfo);

  HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )(
    IJEXEServer __RPC_FAR * This,
    /* [in] */ UINT iTInfo,
    /* [in] */ LCID lcid,
    /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

  HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )(
    IJEXEServer __RPC_FAR * This,
    /* [in] */ REFIID riid,
    /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
    /* [in] */ UINT cNames,
    /* [in] */ LCID lcid,
    /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

  /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )(
    IJEXEServer __RPC_FAR * This,
    /* [in] */ DISPID dispIdMember,
    /* [in] */ REFIID riid,
    /* [in] */ LCID lcid,
    /* [in] */ WORD wFlags,
    /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
    /* [out] */ VARIANT __RPC_FAR *pVarResult,
    /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
    /* [out] */ UINT __RPC_FAR *puArgErr);

  /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Do )(
    IJEXEServer __RPC_FAR * This,
    /* [in] */ BSTR input,
    /* [retval][out] */ long __RPC_FAR *r);

  /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Show )(
    IJEXEServer __RPC_FAR * This,
    /* [in] */ long b,
    /* [retval][out] */ long __RPC_FAR *r);

  /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Log )(
    IJEXEServer __RPC_FAR * This,
    /* [in] */ long b,
    /* [retval][out] */ long __RPC_FAR *r);

  /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsBusy )(
    IJEXEServer __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *r);

  /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Break )(
    IJEXEServer __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *r);

  /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Quit )(
    IJEXEServer __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *r);

  /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Get )(
    IJEXEServer __RPC_FAR * This,
    /* [in] */ BSTR jname,
    /* [out] */ VARIANT __RPC_FAR *v,
    /* [retval][out] */ long __RPC_FAR *r);

  /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Set )(
    IJEXEServer __RPC_FAR * This,
    /* [in] */ BSTR jname,
    /* [in] */ VARIANT __RPC_FAR *v,
    /* [retval][out] */ long __RPC_FAR *r);

  /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetM )(
    IJEXEServer __RPC_FAR * This,
    /* [in] */ BSTR jname,
    /* [out] */ long __RPC_FAR *jtype,
    /* [out] */ long __RPC_FAR *jrank,
    /* [out] */ long __RPC_FAR *jshape,
    /* [out] */ long __RPC_FAR *jdata,
    /* [retval][out] */ long __RPC_FAR *r);

  /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetM )(
    IJEXEServer __RPC_FAR * This,
    /* [in] */ BSTR jname,
    /* [in] */ long __RPC_FAR *jtype,
    /* [in] */ long __RPC_FAR *jrank,
    /* [in] */ long __RPC_FAR *jshape,
    /* [in] */ long __RPC_FAR *jdata,
    /* [retval][out] */ long __RPC_FAR *r);

  /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ErrorText )(
    IJEXEServer __RPC_FAR * This,
    /* [in] */ long error,
    /* [out] */ VARIANT __RPC_FAR *v,
    /* [retval][out] */ long __RPC_FAR *r);

  /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ErrorTextM )(
    IJEXEServer __RPC_FAR * This,
    /* [in] */ long error,
    /* [out] */ long __RPC_FAR *text,
    /* [retval][out] */ long __RPC_FAR *r);

  /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clear )(
    IJEXEServer __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *r);

  /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Transpose )(
    IJEXEServer __RPC_FAR * This,
    /* [in] */ long b,
    /* [retval][out] */ long __RPC_FAR *r);

  /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ErrorTextB )(
    IJEXEServer __RPC_FAR * This,
    /* [in] */ long error,
    /* [out] */ VARIANT __RPC_FAR *v,
    /* [retval][out] */ long __RPC_FAR *r);

  /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetB )(
    IJEXEServer __RPC_FAR * This,
    /* [in] */ BSTR jname,
    /* [out] */ VARIANT __RPC_FAR *v,
    /* [retval][out] */ long __RPC_FAR *r);

  /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetB )(
    IJEXEServer __RPC_FAR * This,
    /* [in] */ BSTR jname,
    /* [in] */ VARIANT __RPC_FAR *v,
    /* [retval][out] */ long __RPC_FAR *r);

  /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DoR )(
    IJEXEServer __RPC_FAR * This,
    /* [in] */ BSTR input,
    /* [out] */ VARIANT __RPC_FAR *v,
    /* [retval][out] */ long __RPC_FAR *r);

  END_INTERFACE
} IJEXEServerVtbl;

interface IJEXEServer {
  CONST_VTBL struct IJEXEServerVtbl __RPC_FAR *lpVtbl;
};



#ifdef COBJMACROS


#define IJEXEServer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IJEXEServer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IJEXEServer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IJEXEServer_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IJEXEServer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IJEXEServer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IJEXEServer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IJEXEServer_Do(This,input,r)	\
    (This)->lpVtbl -> Do(This,input,r)

#define IJEXEServer_Show(This,b,r)	\
    (This)->lpVtbl -> Show(This,b,r)

#define IJEXEServer_Log(This,b,r)	\
    (This)->lpVtbl -> Log(This,b,r)

#define IJEXEServer_IsBusy(This,r)	\
    (This)->lpVtbl -> IsBusy(This,r)

#define IJEXEServer_Break(This,r)	\
    (This)->lpVtbl -> Break(This,r)

#define IJEXEServer_Quit(This,r)	\
    (This)->lpVtbl -> Quit(This,r)

#define IJEXEServer_Get(This,jname,v,r)	\
    (This)->lpVtbl -> Get(This,jname,v,r)

#define IJEXEServer_Set(This,jname,v,r)	\
    (This)->lpVtbl -> Set(This,jname,v,r)

#define IJEXEServer_GetM(This,jname,jtype,jrank,jshape,jdata,r)	\
    (This)->lpVtbl -> GetM(This,jname,jtype,jrank,jshape,jdata,r)

#define IJEXEServer_SetM(This,jname,jtype,jrank,jshape,jdata,r)	\
    (This)->lpVtbl -> SetM(This,jname,jtype,jrank,jshape,jdata,r)

#define IJEXEServer_ErrorText(This,error,v,r)	\
    (This)->lpVtbl -> ErrorText(This,error,v,r)

#define IJEXEServer_ErrorTextM(This,error,text,r)	\
    (This)->lpVtbl -> ErrorTextM(This,error,text,r)

#define IJEXEServer_Clear(This,r)	\
    (This)->lpVtbl -> Clear(This,r)

#define IJEXEServer_Transpose(This,b,r)	\
    (This)->lpVtbl -> Transpose(This,b,r)

#define IJEXEServer_ErrorTextB(This,error,v,r)	\
    (This)->lpVtbl -> ErrorTextB(This,error,v,r)

#define IJEXEServer_GetB(This,jname,v,r)	\
    (This)->lpVtbl -> GetB(This,jname,v,r)

#define IJEXEServer_SetB(This,jname,v,r)	\
    (This)->lpVtbl -> SetB(This,jname,v,r)

#define IJEXEServer_DoR(This,input,v,r)	\
    (This)->lpVtbl -> DoR(This,input,v,r)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IJEXEServer_Do_Proxy(
  IJEXEServer __RPC_FAR * This,
  /* [in] */ BSTR input,
  /* [retval][out] */ long __RPC_FAR *r);


void __RPC_STUB IJEXEServer_Do_Stub(
  IRpcStubBuffer *This,
  IRpcChannelBuffer *_pRpcChannelBuffer,
  PRPC_MESSAGE _pRpcMessage,
  DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IJEXEServer_Show_Proxy(
  IJEXEServer __RPC_FAR * This,
  /* [in] */ long b,
  /* [retval][out] */ long __RPC_FAR *r);


void __RPC_STUB IJEXEServer_Show_Stub(
  IRpcStubBuffer *This,
  IRpcChannelBuffer *_pRpcChannelBuffer,
  PRPC_MESSAGE _pRpcMessage,
  DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IJEXEServer_Log_Proxy(
  IJEXEServer __RPC_FAR * This,
  /* [in] */ long b,
  /* [retval][out] */ long __RPC_FAR *r);


void __RPC_STUB IJEXEServer_Log_Stub(
  IRpcStubBuffer *This,
  IRpcChannelBuffer *_pRpcChannelBuffer,
  PRPC_MESSAGE _pRpcMessage,
  DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IJEXEServer_IsBusy_Proxy(
  IJEXEServer __RPC_FAR * This,
  /* [retval][out] */ long __RPC_FAR *r);


void __RPC_STUB IJEXEServer_IsBusy_Stub(
  IRpcStubBuffer *This,
  IRpcChannelBuffer *_pRpcChannelBuffer,
  PRPC_MESSAGE _pRpcMessage,
  DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IJEXEServer_Break_Proxy(
  IJEXEServer __RPC_FAR * This,
  /* [retval][out] */ long __RPC_FAR *r);


void __RPC_STUB IJEXEServer_Break_Stub(
  IRpcStubBuffer *This,
  IRpcChannelBuffer *_pRpcChannelBuffer,
  PRPC_MESSAGE _pRpcMessage,
  DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IJEXEServer_Quit_Proxy(
  IJEXEServer __RPC_FAR * This,
  /* [retval][out] */ long __RPC_FAR *r);


void __RPC_STUB IJEXEServer_Quit_Stub(
  IRpcStubBuffer *This,
  IRpcChannelBuffer *_pRpcChannelBuffer,
  PRPC_MESSAGE _pRpcMessage,
  DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IJEXEServer_Get_Proxy(
  IJEXEServer __RPC_FAR * This,
  /* [in] */ BSTR jname,
  /* [out] */ VARIANT __RPC_FAR *v,
  /* [retval][out] */ long __RPC_FAR *r);


void __RPC_STUB IJEXEServer_Get_Stub(
  IRpcStubBuffer *This,
  IRpcChannelBuffer *_pRpcChannelBuffer,
  PRPC_MESSAGE _pRpcMessage,
  DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IJEXEServer_Set_Proxy(
  IJEXEServer __RPC_FAR * This,
  /* [in] */ BSTR jname,
  /* [in] */ VARIANT __RPC_FAR *v,
  /* [retval][out] */ long __RPC_FAR *r);


void __RPC_STUB IJEXEServer_Set_Stub(
  IRpcStubBuffer *This,
  IRpcChannelBuffer *_pRpcChannelBuffer,
  PRPC_MESSAGE _pRpcMessage,
  DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IJEXEServer_GetM_Proxy(
  IJEXEServer __RPC_FAR * This,
  /* [in] */ BSTR jname,
  /* [out] */ long __RPC_FAR *jtype,
  /* [out] */ long __RPC_FAR *jrank,
  /* [out] */ long __RPC_FAR *jshape,
  /* [out] */ long __RPC_FAR *jdata,
  /* [retval][out] */ long __RPC_FAR *r);


void __RPC_STUB IJEXEServer_GetM_Stub(
  IRpcStubBuffer *This,
  IRpcChannelBuffer *_pRpcChannelBuffer,
  PRPC_MESSAGE _pRpcMessage,
  DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IJEXEServer_SetM_Proxy(
  IJEXEServer __RPC_FAR * This,
  /* [in] */ BSTR jname,
  /* [in] */ long __RPC_FAR *jtype,
  /* [in] */ long __RPC_FAR *jrank,
  /* [in] */ long __RPC_FAR *jshape,
  /* [in] */ long __RPC_FAR *jdata,
  /* [retval][out] */ long __RPC_FAR *r);


void __RPC_STUB IJEXEServer_SetM_Stub(
  IRpcStubBuffer *This,
  IRpcChannelBuffer *_pRpcChannelBuffer,
  PRPC_MESSAGE _pRpcMessage,
  DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IJEXEServer_ErrorText_Proxy(
  IJEXEServer __RPC_FAR * This,
  /* [in] */ long error,
  /* [out] */ VARIANT __RPC_FAR *v,
  /* [retval][out] */ long __RPC_FAR *r);


void __RPC_STUB IJEXEServer_ErrorText_Stub(
  IRpcStubBuffer *This,
  IRpcChannelBuffer *_pRpcChannelBuffer,
  PRPC_MESSAGE _pRpcMessage,
  DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IJEXEServer_ErrorTextM_Proxy(
  IJEXEServer __RPC_FAR * This,
  /* [in] */ long error,
  /* [out] */ long __RPC_FAR *text,
  /* [retval][out] */ long __RPC_FAR *r);


void __RPC_STUB IJEXEServer_ErrorTextM_Stub(
  IRpcStubBuffer *This,
  IRpcChannelBuffer *_pRpcChannelBuffer,
  PRPC_MESSAGE _pRpcMessage,
  DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IJEXEServer_Clear_Proxy(
  IJEXEServer __RPC_FAR * This,
  /* [retval][out] */ long __RPC_FAR *r);


void __RPC_STUB IJEXEServer_Clear_Stub(
  IRpcStubBuffer *This,
  IRpcChannelBuffer *_pRpcChannelBuffer,
  PRPC_MESSAGE _pRpcMessage,
  DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IJEXEServer_Transpose_Proxy(
  IJEXEServer __RPC_FAR * This,
  /* [in] */ long b,
  /* [retval][out] */ long __RPC_FAR *r);


void __RPC_STUB IJEXEServer_Transpose_Stub(
  IRpcStubBuffer *This,
  IRpcChannelBuffer *_pRpcChannelBuffer,
  PRPC_MESSAGE _pRpcMessage,
  DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IJEXEServer_ErrorTextB_Proxy(
  IJEXEServer __RPC_FAR * This,
  /* [in] */ long error,
  /* [out] */ VARIANT __RPC_FAR *v,
  /* [retval][out] */ long __RPC_FAR *r);


void __RPC_STUB IJEXEServer_ErrorTextB_Stub(
  IRpcStubBuffer *This,
  IRpcChannelBuffer *_pRpcChannelBuffer,
  PRPC_MESSAGE _pRpcMessage,
  DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IJEXEServer_GetB_Proxy(
  IJEXEServer __RPC_FAR * This,
  /* [in] */ BSTR jname,
  /* [out] */ VARIANT __RPC_FAR *v,
  /* [retval][out] */ long __RPC_FAR *r);


void __RPC_STUB IJEXEServer_GetB_Stub(
  IRpcStubBuffer *This,
  IRpcChannelBuffer *_pRpcChannelBuffer,
  PRPC_MESSAGE _pRpcMessage,
  DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IJEXEServer_SetB_Proxy(
  IJEXEServer __RPC_FAR * This,
  /* [in] */ BSTR jname,
  /* [in] */ VARIANT __RPC_FAR *v,
  /* [retval][out] */ long __RPC_FAR *r);


void __RPC_STUB IJEXEServer_SetB_Stub(
  IRpcStubBuffer *This,
  IRpcChannelBuffer *_pRpcChannelBuffer,
  PRPC_MESSAGE _pRpcMessage,
  DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IJEXEServer_DoR_Proxy(
  IJEXEServer __RPC_FAR * This,
  /* [in] */ BSTR input,
  /* [out] */ VARIANT __RPC_FAR *v,
  /* [retval][out] */ long __RPC_FAR *r);


void __RPC_STUB IJEXEServer_DoR_Stub(
  IRpcStubBuffer *This,
  IRpcChannelBuffer *_pRpcChannelBuffer,
  PRPC_MESSAGE _pRpcMessage,
  DWORD *_pdwStubPhase);



#endif 	/* __IJEXEServer_INTERFACE_DEFINED__ */



#ifndef __JEXEServerLib_LIBRARY_DEFINED__
#define __JEXEServerLib_LIBRARY_DEFINED__

/* library JEXEServerLib */
/* [version][helpstring][uuid] */


EXTERN_C const IID LIBID_JEXEServerLib;

EXTERN_C const CLSID CLSID_JEXEServer;

#ifdef __cplusplus

class DECLSPEC_UUID("21EB05E0-1AB3-11cf-A2AC-8FF70874C460")
  JEXEServer;
#endif
#endif /* __JEXEServerLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * );
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * );
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * );
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * );

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * );
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * );
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * );
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * );

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
