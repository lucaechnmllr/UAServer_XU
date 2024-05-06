

#ifndef __XU_IDENTIFIERS_H__
#define __XU_IDENTIFIERS_H__

#define XU_NameSpaceUri "http://yourorganisation.org/XU_Test_ServerConfig/"

#pragma warning(push, 0)
#include "version_coremodule.h"
#pragma warning(pop)


/*============================================================================
* ObjectType Identifiers
 *===========================================================================*/
#define XUId_PDUObjectType 1004
#define XUId_FloatPDUObjectType 1010
#define XUId_IntPDUObjectType 1007
#define XUId_XUObjectType 1003

/*============================================================================
* VariableType Identifiers
 *===========================================================================*/

/*============================================================================
* DataType Identifiers
 *===========================================================================*/

/*============================================================================
* ReferenceType Identifiers
 *===========================================================================*/

/*============================================================================
* Object Identifiers
 *===========================================================================*/
#define XUId_PDUObjectType_States 5001

/*============================================================================
* Variable Identifiers
 *===========================================================================*/
#define XUId_PDUObjectType_States_state_doubt 6017
#define XUId_PDUObjectType_States_state_hwfail 6018
#define XUId_PDUObjectType_States_state_invalid 6019
#define XUId_PDUObjectType_States_state_repval 6020
#define XUId_PDUObjectType_States_state_service 6021
#define XUId_PDUObjectType_States_state_simulat 6022
#define XUId_PDUObjectType_States_state_timeadj 6023
#define XUId_PDUObjectType_States_state_uninit 6024
#define XUId_PDUObjectType_States_state_valid 6016
#define XUId_PDUObjectType_Timestamp 6003
#define XUId_FloatPDUObjectType_Value 6006
#define XUId_FloatPDUObjectType_writeSignal_InputArguments 6008
#define XUId_IntPDUObjectType_Value 6004
#define XUId_IntPDUObjectType_writeSignal_InputArguments 6009
#define XUId_XUObjectType_Signals 6001

/*============================================================================
* Method Identifiers
 *===========================================================================*/
#define XUId_FloatPDUObjectType_writeSignal 7001
#define XUId_IntPDUObjectType_writeSignal 7002


#ifdef _WIN32
#ifdef _xu_exp_BUILD_DLL
# define xu_exp_EXPORT __declspec(dllexport)
#elif defined _xu_exp_USE_DLL
# define xu_exp_EXPORT __declspec(dllimport)
#else
# define xu_exp_EXPORT
#endif
#else
# define xu_exp_EXPORT
#endif

///////////////////////////////////////////////
// Private Class Hiding Macros
#define XU_DECLARE_PRIVATE(Class) \
    inline Class##Private* d_func() { return reinterpret_cast<Class##Private *>(d_ptr); } \
    inline const Class##Private* d_func() const { return reinterpret_cast<const Class##Private *>(d_ptr); } \
    friend class Class##Private; \
    Class##Private *d_ptr;

#define XU_DECLARE_PUBLIC(Class) \
    inline Class* q_func() { return static_cast<Class *>(q_ptr); } \
    inline const Class* q_func() const { return static_cast<const Class *>(q_ptr); } \
    friend class Class;

#define XU_D(Class) Class##Private * const d = d_func()
#define XU_CD(Class) const Class##Private * const d = d_func()
#define XU_Q(Class) Class * const q = q_func()

#endif // __XU_IDENTIFIERS_H__

