

#ifndef __XU_IntPDUObjectType_H__
#define __XU_IntPDUObjectType_H__

#pragma warning(push, 2)
#include "basenodes.h"
#include "opcua_dataitemtype.h"
#include "uaargument.h"
#pragma warning(pop)

#include "xu_pduobjecttype.h"
#include "xu_identifiers.h"
#include "PDU.h"
#include "PDU_String.h"

// Namespace for the UA information model http://yourorganisation.org/XU_Test_ServerConfig/
namespace XU {


    /** Generated base class for a IntPDUObjectType.
     *
     *  This class contains the generated base code for the object type IntPDUObjectType
     *  representing an OPC UA ObjectType. This class is used to create the object type and to
     *  create and represent instances of the object type in the server address space.
     *
     *  **Variable members of the IntPDUObjectType:**
     *
     *  Browse Name | DataType | TypeDefinition | Modelling Rule | See Also
     *  ------------|----------|----------------|----------------|-----------------------------
     *  Value       | Int16    | DataItemType   | Mandatory      | \ref getValue, \ref setValue
     *
     */
    class IntPDUObjectType :
        public XU::PDUObjectType
    {
    protected:
        virtual ~IntPDUObjectType();
    public:
        // construction / destruction
        IntPDUObjectType(const UaNodeId& nodeId, UaObject* pInstanceDeclarationObject, NodeManagerConfig* pNodeConfig, UaMutexRefCounted* pSharedMutex = NULL);
        IntPDUObjectType(const UaNodeId& nodeId, const UaString& name, OpcUa_UInt16 browseNameNameSpaceIndex, NodeManagerConfig* pNodeConfig, int value, int state, PDU::timeval time, UaMutexRefCounted* pSharedMutex = NULL);
        IntPDUObjectType(
            UaBase::Object* pBaseNode,
            XmlUaNodeFactoryManager* pFactory,
            NodeManagerConfig* pNodeConfig,
            UaMutexRefCounted* pSharedMutex = NULL);

        static void createTypes();
        static void clearStaticMembers();

        TYPE getType() const { return INT; };
        virtual int getInt() const { return this->getValue(); }

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
            OpcUa_Int16 newVal,
            OpcUa_Boolean setValid,
            OpcUa_Boolean setInvalid,
            OpcUa_Boolean setDoubt,
            OpcUa_Boolean setHwFail,
            OpcUa_Boolean setRepval,
            OpcUa_Boolean setService,
            OpcUa_Boolean setTimeAdj,
            OpcUa_Boolean setSimulat);

        virtual void setValue(OpcUa_Int16 Value);
        virtual OpcUa_Int16 getValue() const;

        virtual OpcUa::DataItemType* getValueNode();
        virtual const OpcUa::DataItemType* getValueNode() const;
        virtual OpcUa::BaseMethod* getwriteValue();
        virtual const OpcUa::BaseMethod* getwriteValue() const;

        // NodeAccessInfo management
        void useAccessInfoFromInstance(IntPDUObjectType* pInstance);
        virtual void setAccessInfoWithChildren(NodeAccessInfo* pNodeAccessInfo);

    protected:
        // Variable nodes
        // Variable Value
        static OpcUa::DataItemType* s_pValue;
        OpcUa::DataItemType* m_pValue;


        // Method nodes
        static OpcUa::BaseMethod* s_pwriteValueMethod;
        OpcUa::BaseMethod* m_pwriteValueMethod;


    private:
        void initialize(int init_value);

    private:
        static bool s_typeNodesCreated;
    };

} // End namespace for the UA information model http://yourorganisation.org/XU_Test_ServerConfig/

#endif // #ifndef __XUIntPDUObjectType_H__



