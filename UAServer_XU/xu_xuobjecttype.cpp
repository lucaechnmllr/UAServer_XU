

#pragma warning(push, 2)
#include "uagenericnodes.h"
#include "nodemanagerroot.h"
#include "methodhandleuanode.h"
#include "instancefactory.h"
#include "opcua_extensionobject.h"
#pragma warning(pop)


#include "xu_xuobjecttype.h"
#include "xu_nodemanagerxunamespace.h"


// Namespace for the UA information model http://yourorganisation.org/XU_Test_ServerConfig/
namespace XU {

    bool XUObjectType::s_typeNodesCreated = false;
    OpcUa::DataItemType* XUObjectType::s_pSignals = NULL;

    /** Constructs an XUObjectType object using an instance declaration node as base
    */
    XUObjectType::XUObjectType(const UaNodeId& nodeId, UaObject* pInstanceDeclarationObject, NodeManagerConfig* pNodeConfig, UaMutexRefCounted* pSharedMutex)
        : OpcUa::BaseObjectType(nodeId, pInstanceDeclarationObject, pNodeConfig, pSharedMutex)
    {
        initialize();
    }

    /** Constructs an XUObjectType object
    */
    XUObjectType::XUObjectType(const UaNodeId& nodeId, const UaString& name, OpcUa_UInt16 browseNameNameSpaceIndex, NodeManagerConfig* pNodeConfig, UaMutexRefCounted* pSharedMutex)
        : OpcUa::BaseObjectType(nodeId, name, browseNameNameSpaceIndex, pNodeConfig, pSharedMutex)
    {
        initialize();
    }

    /**  Constructs an instance of the class XUObjectType with all components
    */
    XUObjectType::XUObjectType(
        UaBase::Object* pBaseNode,    //!< [in] The "tree" of nodes that specifies the attributes of the instance and its children
        XmlUaNodeFactoryManager* pFactory, //!< [in] The factory to create the children
        NodeManagerConfig* pNodeConfig,  //!< [in] Interface pointer to the NodeManagerConfig interface used to add and delete node and references in the address space
        UaMutexRefCounted* pSharedMutex) //!< [in] Shared mutex object used to synchronize access to the variable. Can be NULL if no shared mutex is provided
        : OpcUa::BaseObjectType(pBaseNode, pFactory, pNodeConfig, pSharedMutex)
    {
        UaStatus      addStatus;

        if (s_typeNodesCreated == false)
        {
            createTypes();
        }

        m_pSignals = NULL;

        std::list<UaBase::BaseNode*> lstReferencedNodes = pBaseNode->hierarchicallyReferencedNodes();
        for (std::list<UaBase::BaseNode*>::const_iterator it = lstReferencedNodes.begin(); it != lstReferencedNodes.end(); it++)
        {
            UaBase::BaseNode* pChild = *it;
            if (pChild->browseName() == UaQualifiedName("Signals", XU::NodeManagerXUNamespace::getTypeNamespace()))
            {
                if (!m_pSignals)
                {
                    m_pSignals = (OpcUa::DataItemType*)pFactory->createVariable((UaBase::Variable*)pChild, pNodeConfig, pSharedMutex);
                    addStatus = pNodeConfig->addNodeAndReference(this, m_pSignals, OpcUaId_HasComponent);
                    UA_ASSERT(addStatus.isGood());
                    if (!((UaBase::Variable*)pChild)->modellingRuleId().isNull())
                    {
                        m_pSignals->setModellingRuleId(((UaBase::Variable*)pChild)->modellingRuleId());
                    }
                }
            }
        }
        UA_ASSERT(m_pSignals);
    }

    /** Initialize the object with all member nodes
    */
    void XUObjectType::initialize()
    {
        UaStatus      addStatus;
        UaVariant     defaultValue;

        if (s_typeNodesCreated == false)
        {
            createTypes();
        }

        m_pSignals = NULL;
        // Mandatory variable Signals
        m_pSignals = new OpcUa::DataItemType(this, s_pSignals, m_pNodeConfig, m_pSharedMutex);
        addStatus = m_pNodeConfig->addNodeAndReference(this, m_pSignals, OpcUaId_HasComponent);
        UA_ASSERT(addStatus.isGood());


    }

    /** Destruction
    */
    XUObjectType::~XUObjectType()
    {
    }

    /** Create the related type nodes
    */
    void XUObjectType::createTypes()
    {
        if (s_typeNodesCreated == false)
        {
            s_typeNodesCreated = true;

            UaStatus      addStatus;
            UaVariant     defaultValue;
            NodeManagerRoot* pNodeManagerRoot = NodeManagerRoot::CreateRootNodeManager();
            OpcUa_Int16 nsTypeIdx = NodeManagerXUNamespace::getTypeNamespace();
            OpcUa_Int16 nsSuperTypeIdx = NodeManagerRoot::getTypeNamespace();
            NodeManagerConfig* pTypeNodeConfig = pNodeManagerRoot->getNodeManagerByNamespace(nsTypeIdx)->getNodeManagerConfig();

            OpcUa::GenericObjectType* pObjectType;
            pObjectType = new OpcUa::GenericObjectType(
                UaNodeId(XUId_XUObjectType, nsTypeIdx),
                UaQualifiedName("XUObjectType", nsTypeIdx),
                UaLocalizedText("", "XUObjectType"),
                UaLocalizedText("", ""),
                OpcUa_False,
                &XUObjectType::clearStaticMembers);
            addStatus = pTypeNodeConfig->addNodeAndReference(UaNodeId(OpcUaId_BaseObjectType, nsSuperTypeIdx), pObjectType, OpcUaId_HasSubtype);
            UA_ASSERT(addStatus.isGood());

            // Mandatory variable Signals
            defaultValue.setUInt64(0);
            s_pSignals = new OpcUa::DataItemType(UaNodeId(XUId_XUObjectType_Signals, nsTypeIdx), "Signals", XU::NodeManagerXUNamespace::getTypeNamespace(), defaultValue, 3, pTypeNodeConfig);
            s_pSignals->setModellingRuleId(OpcUaId_ModellingRule_Mandatory);
            s_pSignals->setValueRank(-1);
            addStatus = pTypeNodeConfig->addNodeAndReference(pObjectType, s_pSignals, OpcUaId_HasComponent);
            UA_ASSERT(addStatus.isGood());

        }
    }

    /** Clear the static members of the class
    */
    void XUObjectType::clearStaticMembers()
    {
        s_typeNodesCreated = false;

        s_pSignals = NULL;
    }

    /** Returns the type definition NodeId for the XUObjectType
    */
    UaNodeId XUObjectType::typeDefinitionId() const
    {
        UaNodeId ret(XUId_XUObjectType, NodeManagerXUNamespace::getTypeNamespace());
        return ret;
    }

    /**
     *  Sets the Signals value
     */
    void XUObjectType::setSignals(OpcUa_UInt64 Signals)
    {
        UaVariant value;
        value.setUInt64(Signals);
        UaDataValue dataValue;
        dataValue.setValue(value, OpcUa_True, OpcUa_True);
        m_pSignals->setValue(NULL, dataValue, OpcUa_False);
    }

    /**
     *  Returns the value of Signals
     */
    OpcUa_UInt64 XUObjectType::getSignals() const
    {
        UaVariant defaultValue;
        OpcUa_UInt64 ret = 0;
        UaDataValue dataValue(m_pSignals->value(NULL));
        defaultValue = *dataValue.value();
        defaultValue.toUInt64(ret);
        return ret;
    }

    /** Returns the Signals node.
     */
    OpcUa::DataItemType* XUObjectType::getSignalsNode()
    {
        return m_pSignals;
    }

    /** Returns the Signals node.
     */
    const OpcUa::DataItemType* XUObjectType::getSignalsNode() const
    {
        return m_pSignals;
    }


    /** Uses the NodeAccessInfo of pOther and its children. */
    void XUObjectType::useAccessInfoFromInstance(XUObjectType* pOther)
    {
        OpcUa::BaseObjectType::useAccessInfoFromInstance(pOther);

        // children
        m_pSignals->useAccessInfoFromInstance(pOther->m_pSignals);
    }

    /** Set the NodeAccessInfo for this node and all of its children. */
    void XUObjectType::setAccessInfoWithChildren(NodeAccessInfo* pNodeAccessInfo)
    {
        OpcUa::BaseObjectType::setAccessInfoWithChildren(pNodeAccessInfo);

        // children
        m_pSignals->setAccessInfoWithChildren(pNodeAccessInfo);
    }


} // End namespace for the UA information model http://yourorganisation.org/XU_Test_ServerConfig/
