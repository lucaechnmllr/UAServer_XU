#ifndef __XU_FLOATPDUOBJECTTYPE_H__
#define __XU_FLOATPDUOBJECTTYPE_H__

#pragma warning(push, 2)
#include "basenodes.h"
#include "opcua_basedatavariabletype.h"
#include "uaargument.h"
#pragma warning(pop)

#include "xu_pduobjecttype.h"
#include "xu_identifiers.h"
#include "PDU.h"
#include "PDU_String.h"


// Namespace for the UA information model http://yourorganisation.org/XU_Test_ServerConfig/
namespace XU {


    /** Generated  class for a FloatPDUObjectType.
     *
     *  This class contains the generated  code for the object type FloatPDUObjectType
     *  representing an OPC UA ObjectType. This class is used to create the object type and to
     *  create and represent instances of the object type in the server address space.
     *
     *  **Variable members of the FloatPDUObjectType:**
     *
     *  Browse Name | DataType | TypeDefinition       | Modelling Rule | See Also
     *  ------------|----------|----------------------|----------------|-----------------------------
     *  Value       | Double   | DataVariableType | Mandatory      | \ref getValue, \ref setValue
     *
     */
    class FloatPDUObjectType :
        public XU::PDUObjectType
    {
    protected:
        virtual ~FloatPDUObjectType();
    public:
        // construction / destruction
        FloatPDUObjectType(const UaNodeId& nodeId, UaObject* pInstanceDeclarationObject, NodeManagerConfig* pNodeConfig, UaMutexRefCounted* pSharedMutex = NULL);
        FloatPDUObjectType(const UaNodeId& nodeId, const UaString& name, OpcUa_UInt16 browseNameNameSpaceIndex, NodeManagerConfig* pNodeConfig, float value, int state, const PDU::timeval time, bool write_permitted, UaMutexRefCounted* pSharedMutex = NULL);

        FloatPDUObjectType(UaBase::Object* pBaseNode, XmlUaNodeFactoryManager* pFactory, NodeManagerConfig* pNodeConfig, UaMutexRefCounted* pSharedMutex);

        static void createTypes();
        static void clearStaticMembers();

        TYPE getType() const { return FLOAT; };
        virtual float getFloat() const { return (float)this->getValue(); }

        virtual UaNodeId       typeDefinitionId() const;


        //- Interface MethodManager -----------------------------------------------------------
        /** Call method of an UA object. */
        virtual UaStatus beginCall(
            MethodManagerCallback* pCallback,
            const ServiceContext& serviceContext,
            OpcUa_UInt32           callbackHandle,
            MethodHandle* pMethodHandle,
            const UaVariantArray& inputArguments);
        virtual UaStatus call(
            const ServiceContext& serviceContext,
            MethodHandle* pMethodHandle,
            const UaVariantArray& inputArguments,
            UaVariantArray& outputArguments,
            UaStatusCodeArray& inputArgumentResults,
            UaDiagnosticInfos& inputArgumentDiag);
        //- Interface MethodManager -----------------------------------------------------------

        UaStatus writeValue(
            const ServiceContext& serviceContext,
            OpcUa_Double newVal,
            OpcUa_Boolean setValid,
            OpcUa_Boolean setInvalid,
            OpcUa_Boolean setDoubt,
            OpcUa_Boolean setHwFail,
            OpcUa_Boolean setRepval,
            OpcUa_Boolean setService,
            OpcUa_Boolean setTimeAdj,
            OpcUa_Boolean setSimulat);

        virtual void setValue(OpcUa_Double Value);
        virtual OpcUa_Double getValue() const;

        virtual OpcUa::BaseDataVariableType* getValueNode();
        virtual const OpcUa::BaseDataVariableType* getValueNode() const;
        virtual OpcUa::BaseMethod* getwriteValue();
        virtual const OpcUa::BaseMethod* getwriteValue() const;

        // NodeAccessInfo management
        void useAccessInfoFromInstance(FloatPDUObjectType* pInstance);
        virtual void setAccessInfoWithChildren(NodeAccessInfo* pNodeAccessInfo);


    protected:
        // Variable nodes
        // Variable Value
        static OpcUa::BaseDataVariableType* s_pValue;
        OpcUa::BaseDataVariableType* m_pValue;


        // Method nodes
        static OpcUa::BaseMethod* s_pwriteValueMethod;
        OpcUa::BaseMethod* m_pwriteValueMethod;


    private:
        void initialize(float init_value);

    private:
        static bool s_typeNodesCreated;
    };

} // End namespace for the UA information model http://yourorganisation.org/XU_Test_ServerConfig/

#endif // #ifndef __XUFLOATPDUOBJECTTYPE_H__