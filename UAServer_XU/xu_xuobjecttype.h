

#ifndef __XU_XUObjectType_H__
#define __XU_XUObjectType_H__

#pragma warning(push,2)
#include "opcua_baseobjecttype.h"
#include "basenodes.h"
#include "opcua_dataitemtype.h"
#pragma warning(pop)


#include "xu_identifiers.h"

// Namespace for the UA information model http://yourorganisation.org/XU_Test_ServerConfig/
namespace XU {


    /** Generated base class for a XUObjectType.
     *
     *  This class contains the generated base code for the object type XUObjectType
     *  representing an OPC UA ObjectType. This class is used to create the object type and to
     *  create and represent instances of the object type in the server address space.
     *
     *  **Variable members of the XUObjectType:**
     *
     *  Browse Name | DataType | TypeDefinition | Modelling Rule | See Also
     *  ------------|----------|----------------|----------------|---------------------------------
     *  Signals     | UInt64   | DataItemType   | Mandatory      | \ref getSignals, \ref setSignals
     *
     */
    class XUObjectType :
        public OpcUa::BaseObjectType
    {
    protected:
        virtual ~XUObjectType();
    public:
        // construction / destruction
        XUObjectType(const UaNodeId& nodeId, UaObject* pInstanceDeclarationObject, NodeManagerConfig* pNodeConfig, UaMutexRefCounted* pSharedMutex = NULL);
        XUObjectType(const UaNodeId& nodeId, const UaString& name, OpcUa_UInt16 browseNameNameSpaceIndex, NodeManagerConfig* pNodeConfig, UaMutexRefCounted* pSharedMutex = NULL);
        XUObjectType(
            UaBase::Object* pBaseNode,
            XmlUaNodeFactoryManager* pFactory,
            NodeManagerConfig* pNodeConfig,
            UaMutexRefCounted* pSharedMutex = NULL);
        static void createTypes();
        static void clearStaticMembers();

        virtual UaNodeId       typeDefinitionId() const;


        virtual void setSignals(OpcUa_UInt64 Signals);
        virtual OpcUa_UInt64 getSignals() const;

        virtual OpcUa::DataItemType* getSignalsNode();
        virtual const OpcUa::DataItemType* getSignalsNode() const;

        // NodeAccessInfo management
        void useAccessInfoFromInstance(XUObjectType* pInstance);
        virtual void setAccessInfoWithChildren(NodeAccessInfo* pNodeAccessInfo);

    protected:
        // Variable nodes
        // Variable Signals
        static OpcUa::DataItemType* s_pSignals;
        OpcUa::DataItemType* m_pSignals;



    private:
        void initialize();

    private:
        static bool s_typeNodesCreated;
    };

} // End namespace for the UA information model http://yourorganisation.org/XU_Test_ServerConfig/

#endif // #ifndef __XUXUObjectType_H__

