

#include "xu_pduobjecttype.h"
#include "xu_nodemanagerxunamespace.h"

#pragma warning(push,2)
#include "uagenericnodes.h"
#include "nodemanagerroot.h"
#include "methodhandleuanode.h"
#include "instancefactory.h"
#include "opcua_extensionobject.h"
#pragma warning(pop)

#include "PDU.h"

// Namespace for the UA information model http://yourorganisation.org/XU_Test_ServerConfig/
namespace XU {

    bool PDUObjectType::s_typeNodesCreated = false;
    OpcUa::FolderType* PDUObjectType::s_pStates = NULL;
    OpcUa::PropertyType* PDUObjectType::s_pTimestamp = NULL;
    OpcUa::PropertyType* PDUObjectType::s_pStates_state_doubt = NULL;
    OpcUa::PropertyType* PDUObjectType::s_pStates_state_hwfail = NULL;
    OpcUa::PropertyType* PDUObjectType::s_pStates_state_invalid = NULL;
    OpcUa::PropertyType* PDUObjectType::s_pStates_state_repval = NULL;
    OpcUa::PropertyType* PDUObjectType::s_pStates_state_service = NULL;
    OpcUa::PropertyType* PDUObjectType::s_pStates_state_simulat = NULL;
    OpcUa::PropertyType* PDUObjectType::s_pStates_state_timeadj = NULL;
    OpcUa::PropertyType* PDUObjectType::s_pStates_state_uninit = NULL;
    OpcUa::PropertyType* PDUObjectType::s_pStates_state_valid = NULL;

    /** Constructs an PDUObjectType object using an instance declaration node as base
    */
    PDUObjectType::PDUObjectType(const UaNodeId& nodeId, UaObject* pInstanceDeclarationObject, NodeManagerConfig* pNodeConfig, UaMutexRefCounted* pSharedMutex)
        : OpcUa::BaseObjectType(nodeId, pInstanceDeclarationObject, pNodeConfig, pSharedMutex)
    {
        initialize({ 0, 0});
    }

    /** Constructs an PDUObjectType object
    */
    PDUObjectType::PDUObjectType(const UaNodeId& nodeId, const UaString& name, OpcUa_UInt16 browseNameNameSpaceIndex, NodeManagerConfig* pNodeConfig, const PDU::timeval time, UaMutexRefCounted* pSharedMutex)
        : OpcUa::BaseObjectType(nodeId, name, browseNameNameSpaceIndex, pNodeConfig, pSharedMutex)
    {
        initialize(time);
    }

    /**  Constructs an instance of the class PDUObjectType with all components
    */
    PDUObjectType::PDUObjectType(
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

        m_pStates = NULL;
        m_pTimestamp = NULL;
        m_pStates_state_doubt = NULL;
        m_pStates_state_hwfail = NULL;
        m_pStates_state_invalid = NULL;
        m_pStates_state_repval = NULL;
        m_pStates_state_service = NULL;
        m_pStates_state_simulat = NULL;
        m_pStates_state_timeadj = NULL;
        m_pStates_state_uninit = NULL;
        m_pStates_state_valid = NULL;

        std::list<UaBase::BaseNode*> lstReferencedNodes = pBaseNode->hierarchicallyReferencedNodes();
        for (std::list<UaBase::BaseNode*>::const_iterator it = lstReferencedNodes.begin(); it != lstReferencedNodes.end(); it++)
        {
            UaBase::BaseNode* pChild = *it;
            if (pChild->browseName() == UaQualifiedName("Timestamp", XU::NodeManagerXUNamespace::getTypeNamespace()))
            {
                if (!m_pTimestamp)
                {
                    m_pTimestamp = (OpcUa::PropertyType*)pFactory->createVariable((UaBase::Variable*)pChild, pNodeConfig, pSharedMutex);
                    addStatus = pNodeConfig->addNodeAndReference(this, m_pTimestamp, OpcUaId_HasProperty);
                    UA_ASSERT(addStatus.isGood());
                    if (!((UaBase::Variable*)pChild)->modellingRuleId().isNull())
                    {
                        m_pTimestamp->setModellingRuleId(((UaBase::Variable*)pChild)->modellingRuleId());
                    }
                }
            }
            else if (pChild->browseName() == UaQualifiedName("States", XU::NodeManagerXUNamespace::getTypeNamespace()))
            {
                if (!m_pStates)
                {
                    m_pStates = (OpcUa::FolderType*)pFactory->createObject((UaBase::Object*)pChild, pNodeConfig, pSharedMutex);
                    addStatus = pNodeConfig->addNodeAndReference(this, m_pStates, OpcUaId_HasComponent);
                    UA_ASSERT(addStatus.isGood());
                    if (!((UaBase::Object*)pChild)->modellingRuleId().isNull())
                    {
                        m_pStates->setModellingRuleId(((UaBase::Object*)pChild)->modellingRuleId());
                    }
                    std::list<UaBase::BaseNode*> lstReferencedNodes2 = pChild->hierarchicallyReferencedNodes();
                    for (std::list<UaBase::BaseNode*>::const_iterator it2 = lstReferencedNodes2.begin(); it2 != lstReferencedNodes2.end(); it2++)
                    {
                        UaBase::BaseNode* pChild2 = *it2;
                        if (!m_pStates_state_doubt)
                        {
                            if (pChild2->browseName() == UaQualifiedName("state_doubt", XU::NodeManagerXUNamespace::getTypeNamespace()))
                            {
                                m_pStates_state_doubt = (OpcUa::PropertyType*)pFactory->createVariable((UaBase::Variable*)pChild2, pNodeConfig, pSharedMutex);
                                addStatus = pNodeConfig->addNodeAndReference(m_pStates, m_pStates_state_doubt, OpcUaId_HasProperty);
                                UA_ASSERT(addStatus.isGood());
                                if (!((UaBase::Object*)pChild2)->modellingRuleId().isNull())
                                {
                                    m_pStates_state_doubt->setModellingRuleId(((UaBase::Object*)pChild2)->modellingRuleId());
                                }
                            }
                        }
                        if (!m_pStates_state_hwfail)
                        {
                            if (pChild2->browseName() == UaQualifiedName("state_hwfail", XU::NodeManagerXUNamespace::getTypeNamespace()))
                            {
                                m_pStates_state_hwfail = (OpcUa::PropertyType*)pFactory->createVariable((UaBase::Variable*)pChild2, pNodeConfig, pSharedMutex);
                                addStatus = pNodeConfig->addNodeAndReference(m_pStates, m_pStates_state_hwfail, OpcUaId_HasProperty);
                                UA_ASSERT(addStatus.isGood());
                                if (!((UaBase::Object*)pChild2)->modellingRuleId().isNull())
                                {
                                    m_pStates_state_hwfail->setModellingRuleId(((UaBase::Object*)pChild2)->modellingRuleId());
                                }
                            }
                        }
                        if (!m_pStates_state_invalid)
                        {
                            if (pChild2->browseName() == UaQualifiedName("state_invalid", XU::NodeManagerXUNamespace::getTypeNamespace()))
                            {
                                m_pStates_state_invalid = (OpcUa::PropertyType*)pFactory->createVariable((UaBase::Variable*)pChild2, pNodeConfig, pSharedMutex);
                                addStatus = pNodeConfig->addNodeAndReference(m_pStates, m_pStates_state_invalid, OpcUaId_HasProperty);
                                UA_ASSERT(addStatus.isGood());
                                if (!((UaBase::Object*)pChild2)->modellingRuleId().isNull())
                                {
                                    m_pStates_state_invalid->setModellingRuleId(((UaBase::Object*)pChild2)->modellingRuleId());
                                }
                            }
                        }
                        if (!m_pStates_state_repval)
                        {
                            if (pChild2->browseName() == UaQualifiedName("state_repval", XU::NodeManagerXUNamespace::getTypeNamespace()))
                            {
                                m_pStates_state_repval = (OpcUa::PropertyType*)pFactory->createVariable((UaBase::Variable*)pChild2, pNodeConfig, pSharedMutex);
                                addStatus = pNodeConfig->addNodeAndReference(m_pStates, m_pStates_state_repval, OpcUaId_HasProperty);
                                UA_ASSERT(addStatus.isGood());
                                if (!((UaBase::Object*)pChild2)->modellingRuleId().isNull())
                                {
                                    m_pStates_state_repval->setModellingRuleId(((UaBase::Object*)pChild2)->modellingRuleId());
                                }
                            }
                        }
                        if (!m_pStates_state_service)
                        {
                            if (pChild2->browseName() == UaQualifiedName("state_service", XU::NodeManagerXUNamespace::getTypeNamespace()))
                            {
                                m_pStates_state_service = (OpcUa::PropertyType*)pFactory->createVariable((UaBase::Variable*)pChild2, pNodeConfig, pSharedMutex);
                                addStatus = pNodeConfig->addNodeAndReference(m_pStates, m_pStates_state_service, OpcUaId_HasProperty);
                                UA_ASSERT(addStatus.isGood());
                                if (!((UaBase::Object*)pChild2)->modellingRuleId().isNull())
                                {
                                    m_pStates_state_service->setModellingRuleId(((UaBase::Object*)pChild2)->modellingRuleId());
                                }
                            }
                        }
                        if (!m_pStates_state_simulat)
                        {
                            if (pChild2->browseName() == UaQualifiedName("state_simulat", XU::NodeManagerXUNamespace::getTypeNamespace()))
                            {
                                m_pStates_state_simulat = (OpcUa::PropertyType*)pFactory->createVariable((UaBase::Variable*)pChild2, pNodeConfig, pSharedMutex);
                                addStatus = pNodeConfig->addNodeAndReference(m_pStates, m_pStates_state_simulat, OpcUaId_HasProperty);
                                UA_ASSERT(addStatus.isGood());
                                if (!((UaBase::Object*)pChild2)->modellingRuleId().isNull())
                                {
                                    m_pStates_state_simulat->setModellingRuleId(((UaBase::Object*)pChild2)->modellingRuleId());
                                }
                            }
                        }
                        if (!m_pStates_state_timeadj)
                        {
                            if (pChild2->browseName() == UaQualifiedName("state_timeadj", XU::NodeManagerXUNamespace::getTypeNamespace()))
                            {
                                m_pStates_state_timeadj = (OpcUa::PropertyType*)pFactory->createVariable((UaBase::Variable*)pChild2, pNodeConfig, pSharedMutex);
                                addStatus = pNodeConfig->addNodeAndReference(m_pStates, m_pStates_state_timeadj, OpcUaId_HasProperty);
                                UA_ASSERT(addStatus.isGood());
                                if (!((UaBase::Object*)pChild2)->modellingRuleId().isNull())
                                {
                                    m_pStates_state_timeadj->setModellingRuleId(((UaBase::Object*)pChild2)->modellingRuleId());
                                }
                            }
                        }
                        if (!m_pStates_state_uninit)
                        {
                            if (pChild2->browseName() == UaQualifiedName("state_uninit", XU::NodeManagerXUNamespace::getTypeNamespace()))
                            {
                                m_pStates_state_uninit = (OpcUa::PropertyType*)pFactory->createVariable((UaBase::Variable*)pChild2, pNodeConfig, pSharedMutex);
                                addStatus = pNodeConfig->addNodeAndReference(m_pStates, m_pStates_state_uninit, OpcUaId_HasProperty);
                                UA_ASSERT(addStatus.isGood());
                                if (!((UaBase::Object*)pChild2)->modellingRuleId().isNull())
                                {
                                    m_pStates_state_uninit->setModellingRuleId(((UaBase::Object*)pChild2)->modellingRuleId());
                                }
                            }
                        }
                        if (!m_pStates_state_valid)
                        {
                            if (pChild2->browseName() == UaQualifiedName("state_valid", XU::NodeManagerXUNamespace::getTypeNamespace()))
                            {
                                m_pStates_state_valid = (OpcUa::PropertyType*)pFactory->createVariable((UaBase::Variable*)pChild2, pNodeConfig, pSharedMutex);
                                addStatus = pNodeConfig->addNodeAndReference(m_pStates, m_pStates_state_valid, OpcUaId_HasProperty);
                                UA_ASSERT(addStatus.isGood());
                                if (!((UaBase::Object*)pChild2)->modellingRuleId().isNull())
                                {
                                    m_pStates_state_valid->setModellingRuleId(((UaBase::Object*)pChild2)->modellingRuleId());
                                }
                            }
                        }
                    }
                }
            }
        }
        UA_ASSERT(m_pStates);
        UA_ASSERT(m_pTimestamp);
        UA_ASSERT(m_pStates_state_doubt);
        UA_ASSERT(m_pStates_state_hwfail);
        UA_ASSERT(m_pStates_state_invalid);
        UA_ASSERT(m_pStates_state_repval);
        UA_ASSERT(m_pStates_state_service);
        UA_ASSERT(m_pStates_state_simulat);
        UA_ASSERT(m_pStates_state_timeadj);
        UA_ASSERT(m_pStates_state_uninit);
        UA_ASSERT(m_pStates_state_valid);
    }

    /** Initialize the object with all member nodes
    */
    void PDUObjectType::initialize(const PDU::timeval time)
    {
        OpcUa_Int16 nsIdx = m_pNodeConfig->getNameSpaceIndex();
        UaStatus      addStatus;
        UaVariant     defaultValue;

        if (s_typeNodesCreated == false)
        {
            createTypes();
        }

        m_pStates = NULL;
        m_pTimestamp = NULL;
        m_pStates_state_doubt = NULL;
        m_pStates_state_hwfail = NULL;
        m_pStates_state_invalid = NULL;
        m_pStates_state_repval = NULL;
        m_pStates_state_service = NULL;
        m_pStates_state_simulat = NULL;
        m_pStates_state_timeadj = NULL;
        m_pStates_state_uninit = NULL;
        m_pStates_state_valid = NULL;
        // Object States
        m_pStates = new OpcUa::FolderType(UaNodeId(UaString("%1.States").arg(nodeId().toString()), nsIdx), s_pStates, m_pNodeConfig, m_pSharedMutex);
        addStatus = m_pNodeConfig->addNodeAndReference(this, m_pStates, OpcUaId_HasComponent);
        UA_ASSERT(addStatus.isGood());
        // Variable state_doubt
        m_pStates_state_doubt = new OpcUa::PropertyType(
            m_pStates,
            s_pStates_state_doubt,
            m_pNodeConfig,
            m_pSharedMutex);
        addStatus = m_pNodeConfig->addNodeAndReference(m_pStates, m_pStates_state_doubt, OpcUaId_HasProperty);
        UA_ASSERT(addStatus.isGood());
        // Variable state_hwfail
        m_pStates_state_hwfail = new OpcUa::PropertyType(
            m_pStates,
            s_pStates_state_hwfail,
            m_pNodeConfig,
            m_pSharedMutex);
        addStatus = m_pNodeConfig->addNodeAndReference(m_pStates, m_pStates_state_hwfail, OpcUaId_HasProperty);
        UA_ASSERT(addStatus.isGood());
        // Variable state_invalid
        m_pStates_state_invalid = new OpcUa::PropertyType(
            m_pStates,
            s_pStates_state_invalid,
            m_pNodeConfig,
            m_pSharedMutex);
        addStatus = m_pNodeConfig->addNodeAndReference(m_pStates, m_pStates_state_invalid, OpcUaId_HasProperty);
        UA_ASSERT(addStatus.isGood());
        // Variable state_repval
        m_pStates_state_repval = new OpcUa::PropertyType(
            m_pStates,
            s_pStates_state_repval,
            m_pNodeConfig,
            m_pSharedMutex);
        addStatus = m_pNodeConfig->addNodeAndReference(m_pStates, m_pStates_state_repval, OpcUaId_HasProperty);
        UA_ASSERT(addStatus.isGood());
        // Variable state_service
        m_pStates_state_service = new OpcUa::PropertyType(
            m_pStates,
            s_pStates_state_service,
            m_pNodeConfig,
            m_pSharedMutex);
        addStatus = m_pNodeConfig->addNodeAndReference(m_pStates, m_pStates_state_service, OpcUaId_HasProperty);
        UA_ASSERT(addStatus.isGood());
        // Variable state_simulat
        m_pStates_state_simulat = new OpcUa::PropertyType(
            m_pStates,
            s_pStates_state_simulat,
            m_pNodeConfig,
            m_pSharedMutex);
        addStatus = m_pNodeConfig->addNodeAndReference(m_pStates, m_pStates_state_simulat, OpcUaId_HasProperty);
        UA_ASSERT(addStatus.isGood());
        // Variable state_timeadj
        m_pStates_state_timeadj = new OpcUa::PropertyType(
            m_pStates,
            s_pStates_state_timeadj,
            m_pNodeConfig,
            m_pSharedMutex);
        addStatus = m_pNodeConfig->addNodeAndReference(m_pStates, m_pStates_state_timeadj, OpcUaId_HasProperty);
        UA_ASSERT(addStatus.isGood());
        // Variable state_uninit
        m_pStates_state_uninit = new OpcUa::PropertyType(
            m_pStates,
            s_pStates_state_uninit,
            m_pNodeConfig,
            m_pSharedMutex);
        addStatus = m_pNodeConfig->addNodeAndReference(m_pStates, m_pStates_state_uninit, OpcUaId_HasProperty);
        UA_ASSERT(addStatus.isGood());
        // Variable state_valid
        m_pStates_state_valid = new OpcUa::PropertyType(
            m_pStates,
            s_pStates_state_valid,
            m_pNodeConfig,
            m_pSharedMutex);
        addStatus = m_pNodeConfig->addNodeAndReference(m_pStates, m_pStates_state_valid, OpcUaId_HasProperty);
        UA_ASSERT(addStatus.isGood());

        // Mandatory variable Timestamp
        m_pTimestamp = new OpcUa::PropertyType(this, s_pTimestamp, m_pNodeConfig, m_pSharedMutex);
        addStatus = m_pNodeConfig->addNodeAndReference(this, m_pTimestamp, OpcUaId_HasProperty);
        UaVariant val;
        val.setDateTime(UaDateTime::fromTime_t(static_cast<time_t>(time.tv_sec) + time.tv_usec));
        UaDataValue dataValue;
        dataValue.setValue(val, OpcUa_True, OpcUa_True);
        m_pTimestamp->setValue(NULL, dataValue, OpcUa_False);
        UA_ASSERT(addStatus.isGood());


    }

    /** Destruction
    */
    PDUObjectType::~PDUObjectType()
    {
    }

    /** Create the related type nodes
    */
    void PDUObjectType::createTypes()
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
                UaNodeId(XUId_PDUObjectType, nsTypeIdx),
                UaQualifiedName("PDUObjectType", nsTypeIdx),
                UaLocalizedText("", "PDUObjectType"),
                UaLocalizedText("", ""),
                OpcUa_True,
                &PDUObjectType::clearStaticMembers);
            addStatus = pTypeNodeConfig->addNodeAndReference(UaNodeId(OpcUaId_BaseObjectType, nsSuperTypeIdx), pObjectType, OpcUaId_HasSubtype);
            UA_ASSERT(addStatus.isGood());
            // Object States
            s_pStates = new OpcUa::FolderType(UaNodeId(XUId_PDUObjectType_States, nsTypeIdx), "States", XU::NodeManagerXUNamespace::getTypeNamespace(), pTypeNodeConfig);
            s_pStates->setModellingRuleId(OpcUaId_ModellingRule_Mandatory);
            addStatus = pTypeNodeConfig->addNodeAndReference(pObjectType, s_pStates, OpcUaId_HasComponent);
            UA_ASSERT(addStatus.isGood());

            // Mandatory variable Timestamp
            defaultValue.setDateTime(UaDateTime());
            s_pTimestamp = new OpcUa::PropertyType(UaNodeId(XUId_PDUObjectType_Timestamp, nsTypeIdx), "Timestamp", XU::NodeManagerXUNamespace::getTypeNamespace(), defaultValue, 1, pTypeNodeConfig);
            s_pTimestamp->setDescription(UaLocalizedText("", "Timestamp delivered from the XU"));
            s_pTimestamp->setModellingRuleId(OpcUaId_ModellingRule_Mandatory);
            s_pTimestamp->setValueRank(-1);
            addStatus = pTypeNodeConfig->addNodeAndReference(pObjectType, s_pTimestamp, OpcUaId_HasProperty);
            UA_ASSERT(addStatus.isGood());

            //  variable state_doubt
            defaultValue.setBool(OpcUa_False);
            s_pStates_state_doubt = new OpcUa::PropertyType(UaNodeId(XUId_PDUObjectType_States_state_doubt, nsTypeIdx), "state_doubt", XU::NodeManagerXUNamespace::getTypeNamespace(), defaultValue, 1, pTypeNodeConfig);
            s_pStates_state_doubt->setModellingRuleId(OpcUaId_ModellingRule_Mandatory);
            s_pStates_state_doubt->setValueRank(-1);

            addStatus = pTypeNodeConfig->addNodeAndReference(s_pStates, s_pStates_state_doubt, OpcUaId_HasProperty);
            UA_ASSERT(addStatus.isGood());

            //  variable state_hwfail
            defaultValue.setBool(OpcUa_False);
            s_pStates_state_hwfail = new OpcUa::PropertyType(UaNodeId(XUId_PDUObjectType_States_state_hwfail, nsTypeIdx), "state_hwfail", XU::NodeManagerXUNamespace::getTypeNamespace(), defaultValue, 1, pTypeNodeConfig);
            s_pStates_state_hwfail->setModellingRuleId(OpcUaId_ModellingRule_Mandatory);
            s_pStates_state_hwfail->setValueRank(-1);

            addStatus = pTypeNodeConfig->addNodeAndReference(s_pStates, s_pStates_state_hwfail, OpcUaId_HasProperty);
            UA_ASSERT(addStatus.isGood());

            //  variable state_invalid
            defaultValue.setBool(OpcUa_False);
            s_pStates_state_invalid = new OpcUa::PropertyType(UaNodeId(XUId_PDUObjectType_States_state_invalid, nsTypeIdx), "state_invalid", XU::NodeManagerXUNamespace::getTypeNamespace(), defaultValue, 1, pTypeNodeConfig);
            s_pStates_state_invalid->setModellingRuleId(OpcUaId_ModellingRule_Mandatory);
            s_pStates_state_invalid->setValueRank(-1);

            addStatus = pTypeNodeConfig->addNodeAndReference(s_pStates, s_pStates_state_invalid, OpcUaId_HasProperty);
            UA_ASSERT(addStatus.isGood());

            //  variable state_repval
            defaultValue.setBool(OpcUa_False);
            s_pStates_state_repval = new OpcUa::PropertyType(UaNodeId(XUId_PDUObjectType_States_state_repval, nsTypeIdx), "state_repval", XU::NodeManagerXUNamespace::getTypeNamespace(), defaultValue, 1, pTypeNodeConfig);
            s_pStates_state_repval->setModellingRuleId(OpcUaId_ModellingRule_Mandatory);
            s_pStates_state_repval->setValueRank(-1);

            addStatus = pTypeNodeConfig->addNodeAndReference(s_pStates, s_pStates_state_repval, OpcUaId_HasProperty);
            UA_ASSERT(addStatus.isGood());

            //  variable state_service
            defaultValue.setBool(OpcUa_False);
            s_pStates_state_service = new OpcUa::PropertyType(UaNodeId(XUId_PDUObjectType_States_state_service, nsTypeIdx), "state_service", XU::NodeManagerXUNamespace::getTypeNamespace(), defaultValue, 1, pTypeNodeConfig);
            s_pStates_state_service->setModellingRuleId(OpcUaId_ModellingRule_Mandatory);
            s_pStates_state_service->setValueRank(-1);

            addStatus = pTypeNodeConfig->addNodeAndReference(s_pStates, s_pStates_state_service, OpcUaId_HasProperty);
            UA_ASSERT(addStatus.isGood());

            //  variable state_simulat
            defaultValue.setBool(OpcUa_False);
            s_pStates_state_simulat = new OpcUa::PropertyType(UaNodeId(XUId_PDUObjectType_States_state_simulat, nsTypeIdx), "state_simulat", XU::NodeManagerXUNamespace::getTypeNamespace(), defaultValue, 1, pTypeNodeConfig);
            s_pStates_state_simulat->setModellingRuleId(OpcUaId_ModellingRule_Mandatory);
            s_pStates_state_simulat->setValueRank(-1);

            addStatus = pTypeNodeConfig->addNodeAndReference(s_pStates, s_pStates_state_simulat, OpcUaId_HasProperty);
            UA_ASSERT(addStatus.isGood());

            //  variable state_timeadj
            defaultValue.setBool(OpcUa_False);
            s_pStates_state_timeadj = new OpcUa::PropertyType(UaNodeId(XUId_PDUObjectType_States_state_timeadj, nsTypeIdx), "state_timeadj", XU::NodeManagerXUNamespace::getTypeNamespace(), defaultValue, 1, pTypeNodeConfig);
            s_pStates_state_timeadj->setModellingRuleId(OpcUaId_ModellingRule_Mandatory);
            s_pStates_state_timeadj->setValueRank(-1);

            addStatus = pTypeNodeConfig->addNodeAndReference(s_pStates, s_pStates_state_timeadj, OpcUaId_HasProperty);
            UA_ASSERT(addStatus.isGood());

            //  variable state_uninit
            defaultValue.setBool(OpcUa_False);
            s_pStates_state_uninit = new OpcUa::PropertyType(UaNodeId(XUId_PDUObjectType_States_state_uninit, nsTypeIdx), "state_uninit", XU::NodeManagerXUNamespace::getTypeNamespace(), defaultValue, 1, pTypeNodeConfig);
            s_pStates_state_uninit->setModellingRuleId(OpcUaId_ModellingRule_Mandatory);
            s_pStates_state_uninit->setValueRank(-1);

            addStatus = pTypeNodeConfig->addNodeAndReference(s_pStates, s_pStates_state_uninit, OpcUaId_HasProperty);
            UA_ASSERT(addStatus.isGood());

            //  variable state_valid
            defaultValue.setBool(OpcUa_False);
            s_pStates_state_valid = new OpcUa::PropertyType(UaNodeId(XUId_PDUObjectType_States_state_valid, nsTypeIdx), "state_valid", XU::NodeManagerXUNamespace::getTypeNamespace(), defaultValue, 1, pTypeNodeConfig);
            s_pStates_state_valid->setModellingRuleId(OpcUaId_ModellingRule_Mandatory);
            s_pStates_state_valid->setValueRank(-1);

            addStatus = pTypeNodeConfig->addNodeAndReference(s_pStates, s_pStates_state_valid, OpcUaId_HasProperty);
            UA_ASSERT(addStatus.isGood());

        }
    }

    /** Clear the static members of the class
    */
    void PDUObjectType::clearStaticMembers()
    {
        s_typeNodesCreated = false;

        s_pStates = NULL;
        s_pTimestamp = NULL;
        s_pStates_state_doubt = NULL;
        s_pStates_state_hwfail = NULL;
        s_pStates_state_invalid = NULL;
        s_pStates_state_repval = NULL;
        s_pStates_state_service = NULL;
        s_pStates_state_simulat = NULL;
        s_pStates_state_timeadj = NULL;
        s_pStates_state_uninit = NULL;
        s_pStates_state_valid = NULL;
    }

    /** Returns the type definition NodeId for the PDUObjectType
    */
    UaNodeId PDUObjectType::typeDefinitionId() const
    {
        UaNodeId ret(XUId_PDUObjectType, NodeManagerXUNamespace::getTypeNamespace());
        return ret;
    }

    /**
     *  Sets the Timestamp value
     *
     *  Timestamp delivered from the XU
     */
    void PDUObjectType::setTimestamp(const UaDateTime& Timestamp)
    {
        UaVariant value;
        value.setDateTime(Timestamp);
        UaDataValue dataValue;
        dataValue.setValue(value, OpcUa_True, OpcUa_True);
        m_pTimestamp->setValue(NULL, dataValue, OpcUa_False);
    }

    /**
     *  Returns the value of Timestamp
     *
     *  Timestamp delivered from the XU
     */
    UaDateTime PDUObjectType::getTimestamp() const
    {
        UaVariant defaultValue;
        UaDateTime ret = UaDateTime();
        UaDataValue dataValue(m_pTimestamp->value(NULL));
        defaultValue = *dataValue.value();
        defaultValue.toDateTime(ret);
        return ret;
    }

    /**
     *  Sets the state_doubt value
     */
    void PDUObjectType::setStates_state_doubt(OpcUa_Boolean state_doubt)
    {
        UaVariant value;
        value.setBool(state_doubt);
        UaDataValue dataValue;
        dataValue.setValue(value, OpcUa_True, OpcUa_True);
        m_pStates_state_doubt->setValue(NULL, dataValue, OpcUa_False);
    }

    /**
     *  Returns the value of state_doubt
     */
    OpcUa_Boolean PDUObjectType::getStates_state_doubt() const
    {
        OpcUa_Boolean ret = OpcUa_False;
        UaDataValue dataValue(m_pStates_state_doubt->value(NULL));
        UaVariant defaultValue = *dataValue.value();
        defaultValue.toBool(ret);
        return ret;
    }

    /**
     *  Sets the state_hwfail value
     */
    void PDUObjectType::setStates_state_hwfail(OpcUa_Boolean state_hwfail)
    {
        UaVariant value;
        value.setBool(state_hwfail);
        UaDataValue dataValue;
        dataValue.setValue(value, OpcUa_True, OpcUa_True);
        m_pStates_state_hwfail->setValue(NULL, dataValue, OpcUa_False);
    }

    /**
     *  Returns the value of state_hwfail
     */
    OpcUa_Boolean PDUObjectType::getStates_state_hwfail() const
    {
        UaVariant defaultValue;
        OpcUa_Boolean ret = OpcUa_False;
        UaDataValue dataValue(m_pStates_state_hwfail->value(NULL));
        defaultValue = *dataValue.value();
        defaultValue.toBool(ret);
        return ret;
    }

    /**
     *  Sets the state_invalid value
     */
    void PDUObjectType::setStates_state_invalid(OpcUa_Boolean state_invalid)
    {
        UaVariant value;
        value.setBool(state_invalid);
        UaDataValue dataValue;
        dataValue.setValue(value, OpcUa_True, OpcUa_True);
        m_pStates_state_invalid->setValue(NULL, dataValue, OpcUa_False);
    }

    /**
     *  Returns the value of state_invalid
     */
    OpcUa_Boolean PDUObjectType::getStates_state_invalid() const
    {
        UaVariant defaultValue;
        OpcUa_Boolean ret = OpcUa_False;
        UaDataValue dataValue(m_pStates_state_invalid->value(NULL));
        defaultValue = *dataValue.value();
        defaultValue.toBool(ret);
        return ret;
    }

    /**
     *  Sets the state_repval value
     */
    void PDUObjectType::setStates_state_repval(OpcUa_Boolean state_repval)
    {
        UaVariant value;
        value.setBool(state_repval);
        UaDataValue dataValue;
        dataValue.setValue(value, OpcUa_True, OpcUa_True);
        m_pStates_state_repval->setValue(NULL, dataValue, OpcUa_False);
    }

    /**
     *  Returns the value of state_repval
     */
    OpcUa_Boolean PDUObjectType::getStates_state_repval() const
    {
        UaVariant defaultValue;
        OpcUa_Boolean ret = OpcUa_False;
        UaDataValue dataValue(m_pStates_state_repval->value(NULL));
        defaultValue = *dataValue.value();
        defaultValue.toBool(ret);
        return ret;
    }

    /**
     *  Sets the state_service value
     */
    void PDUObjectType::setStates_state_service(OpcUa_Boolean state_service)
    {
        UaVariant value;
        value.setBool(state_service);
        UaDataValue dataValue;
        dataValue.setValue(value, OpcUa_True, OpcUa_True);
        m_pStates_state_service->setValue(NULL, dataValue, OpcUa_False);
    }

    /**
     *  Returns the value of state_service
     */
    OpcUa_Boolean PDUObjectType::getStates_state_service() const
    {
        UaVariant defaultValue;
        OpcUa_Boolean ret = OpcUa_False;
        UaDataValue dataValue(m_pStates_state_service->value(NULL));
        defaultValue = *dataValue.value();
        defaultValue.toBool(ret);
        return ret;
    }

    /**
     *  Sets the state_simulat value
     */
    void PDUObjectType::setStates_state_simulat(OpcUa_Boolean state_simulat)
    {
        UaVariant value;
        value.setBool(state_simulat);
        UaDataValue dataValue;
        dataValue.setValue(value, OpcUa_True, OpcUa_True);
        m_pStates_state_simulat->setValue(NULL, dataValue, OpcUa_False);
    }

    /**
     *  Returns the value of state_simulat
     */
    OpcUa_Boolean PDUObjectType::getStates_state_simulat() const
    {
        UaVariant defaultValue;
        OpcUa_Boolean ret = OpcUa_False;
        UaDataValue dataValue(m_pStates_state_simulat->value(NULL));
        defaultValue = *dataValue.value();
        defaultValue.toBool(ret);
        return ret;
    }

    /**
     *  Sets the state_timeadj value
     */
    void PDUObjectType::setStates_state_timeadj(OpcUa_Boolean state_timeadj)
    {
        UaVariant value;
        value.setBool(state_timeadj);
        UaDataValue dataValue;
        dataValue.setValue(value, OpcUa_True, OpcUa_True);
        m_pStates_state_timeadj->setValue(NULL, dataValue, OpcUa_False);
    }

    /**
     *  Returns the value of state_timeadj
     */
    OpcUa_Boolean PDUObjectType::getStates_state_timeadj() const
    {
        UaVariant defaultValue;
        OpcUa_Boolean ret = OpcUa_False;
        UaDataValue dataValue(m_pStates_state_timeadj->value(NULL));
        defaultValue = *dataValue.value();
        defaultValue.toBool(ret);
        return ret;
    }

    /**
     *  Sets the state_uninit value
     */
    void PDUObjectType::setStates_state_uninit(OpcUa_Boolean state_uninit)
    {
        UaVariant value;
        value.setBool(state_uninit);
        UaDataValue dataValue;
        dataValue.setValue(value, OpcUa_True, OpcUa_True);
        m_pStates_state_uninit->setValue(NULL, dataValue, OpcUa_False);
    }

    /**
     *  Returns the value of state_uninit
     */
    OpcUa_Boolean PDUObjectType::getStates_state_uninit() const
    {
        UaVariant defaultValue;
        OpcUa_Boolean ret = OpcUa_False;
        UaDataValue dataValue(m_pStates_state_uninit->value(NULL));
        defaultValue = *dataValue.value();
        defaultValue.toBool(ret);
        return ret;
    }

    /**
     *  Sets the state_valid value
     */
    void PDUObjectType::setStates_state_valid(OpcUa_Boolean state_valid)
    {
        UaVariant value;
        value.setBool(state_valid);
        UaDataValue dataValue;
        dataValue.setValue(value, OpcUa_True, OpcUa_True);
        m_pStates_state_valid->setValue(NULL, dataValue, OpcUa_False);
    }

    /**
     *  Returns the value of state_valid
     */
    OpcUa_Boolean PDUObjectType::getStates_state_valid() const
    {
        UaVariant defaultValue;
        OpcUa_Boolean ret = OpcUa_False;
        UaDataValue dataValue(m_pStates_state_valid->value(NULL));
        defaultValue = *dataValue.value();
        defaultValue.toBool(ret);
        return ret;
    }

    /** Returns the States node.
     */
    OpcUa::FolderType* PDUObjectType::getStates()
    {
        return m_pStates;
    }

    /** Returns the States node.
     */
    const OpcUa::FolderType* PDUObjectType::getStates() const
    {
        return m_pStates;
    }

    /** Returns the Timestamp node.
     */
    OpcUa::PropertyType* PDUObjectType::getTimestampNode()
    {
        return m_pTimestamp;
    }

    /** Returns the Timestamp node.
     */
    const OpcUa::PropertyType* PDUObjectType::getTimestampNode() const
    {
        return m_pTimestamp;
    }

    /** Returns the state_doubt node.
      */
    OpcUa::PropertyType* PDUObjectType::getStates_state_doubtNode()
    {
        return m_pStates_state_doubt;
    }

    /** Returns the state_doubt node.
      */
    const OpcUa::PropertyType* PDUObjectType::getStates_state_doubtNode() const
    {
        return m_pStates_state_doubt;
    }
    /** Returns the state_hwfail node.
      */
    OpcUa::PropertyType* PDUObjectType::getStates_state_hwfailNode()
    {
        return m_pStates_state_hwfail;
    }

    /** Returns the state_hwfail node.
      */
    const OpcUa::PropertyType* PDUObjectType::getStates_state_hwfailNode() const
    {
        return m_pStates_state_hwfail;
    }
    /** Returns the state_invalid node.
      */
    OpcUa::PropertyType* PDUObjectType::getStates_state_invalidNode()
    {
        return m_pStates_state_invalid;
    }

    /** Returns the state_invalid node.
      */
    const OpcUa::PropertyType* PDUObjectType::getStates_state_invalidNode() const
    {
        return m_pStates_state_invalid;
    }
    /** Returns the state_repval node.
      */
    OpcUa::PropertyType* PDUObjectType::getStates_state_repvalNode()
    {
        return m_pStates_state_repval;
    }

    /** Returns the state_repval node.
      */
    const OpcUa::PropertyType* PDUObjectType::getStates_state_repvalNode() const
    {
        return m_pStates_state_repval;
    }
    /** Returns the state_service node.
      */
    OpcUa::PropertyType* PDUObjectType::getStates_state_serviceNode()
    {
        return m_pStates_state_service;
    }

    /** Returns the state_service node.
      */
    const OpcUa::PropertyType* PDUObjectType::getStates_state_serviceNode() const
    {
        return m_pStates_state_service;
    }
    /** Returns the state_simulat node.
      */
    OpcUa::PropertyType* PDUObjectType::getStates_state_simulatNode()
    {
        return m_pStates_state_simulat;
    }

    /** Returns the state_simulat node.
      */
    const OpcUa::PropertyType* PDUObjectType::getStates_state_simulatNode() const
    {
        return m_pStates_state_simulat;
    }
    /** Returns the state_timeadj node.
      */
    OpcUa::PropertyType* PDUObjectType::getStates_state_timeadjNode()
    {
        return m_pStates_state_timeadj;
    }

    /** Returns the state_timeadj node.
      */
    const OpcUa::PropertyType* PDUObjectType::getStates_state_timeadjNode() const
    {
        return m_pStates_state_timeadj;
    }
    /** Returns the state_uninit node.
      */
    OpcUa::PropertyType* PDUObjectType::getStates_state_uninitNode()
    {
        return m_pStates_state_uninit;
    }

    /** Returns the state_uninit node.
      */
    const OpcUa::PropertyType* PDUObjectType::getStates_state_uninitNode() const
    {
        return m_pStates_state_uninit;
    }
    /** Returns the state_valid node.
      */
    OpcUa::PropertyType* PDUObjectType::getStates_state_validNode()
    {
        return m_pStates_state_valid;
    }

    /** Returns the state_valid node.
      */
    const OpcUa::PropertyType* PDUObjectType::getStates_state_validNode() const
    {
        return m_pStates_state_valid;
    }

    /** Uses the NodeAccessInfo of pOther and its children. */
    void PDUObjectType::useAccessInfoFromInstance(PDUObjectType* pOther)
    {
        OpcUa::BaseObjectType::useAccessInfoFromInstance(pOther);

        // children
        m_pStates->useAccessInfoFromInstance(pOther->m_pStates);
        m_pTimestamp->useAccessInfoFromInstance(pOther->m_pTimestamp);
    }

    /** Set the NodeAccessInfo for this node and all of its children. */
    void PDUObjectType::setAccessInfoWithChildren(NodeAccessInfo* pNodeAccessInfo)
    {
        OpcUa::BaseObjectType::setAccessInfoWithChildren(pNodeAccessInfo);

        // children
        m_pStates->setAccessInfoWithChildren(pNodeAccessInfo);
        m_pTimestamp->setAccessInfoWithChildren(pNodeAccessInfo);
    }

    int PDUObjectType::getInt() const
    {
        throw std::runtime_error("Call of function 'getInt()' is not allowed for"
            " this kind of object");
    }

    float PDUObjectType::getFloat() const
    {
        throw std::runtime_error("Call of function 'getFloat()' is not allowed for"
            " this kind of object");
    }


    void PDUObjectType::setStates(XU::PDUObjectType* node, int state)
    {
        if (state == PDU::STATE::VALID) {
            node->setStates_state_valid(OpcUa_True);
        }
        else {
            node->setStates_state_valid(OpcUa_False);
        }
        if (state & PDU::STATE::UNINIT) {
            node->setStates_state_uninit(OpcUa_True);
            node->state_bitwise += PDU::STATE::UNINIT;
        }
        else {
            node->setStates_state_uninit(OpcUa_False);
        }
        if (state & PDU::STATE::INVALID) {
            node->setStates_state_invalid(OpcUa_True);
            node->state_bitwise += PDU::STATE::INVALID;
        }
        else {
            node->setStates_state_invalid(OpcUa_False);
        }
        if (state & PDU::STATE::REPVAL) {
            node->setStates_state_repval(OpcUa_True);
            node->state_bitwise += PDU::STATE::REPVAL;
        }
        else {
            node->setStates_state_repval(OpcUa_False);
        }
        if (state & PDU::STATE::SIMULAT) {
            node->setStates_state_simulat(OpcUa_True);
            node->state_bitwise += PDU::STATE::SIMULAT;
        }
        else {
            node->setStates_state_simulat(OpcUa_False);
        }
        if (state & PDU::STATE::DOUBT) {
            node->setStates_state_doubt(OpcUa_True);
            node->state_bitwise += PDU::STATE::DOUBT;
        }
        else {
            node->setStates_state_doubt(OpcUa_False);
        }
        if (state & PDU::STATE::HWFAIL) {
            node->setStates_state_hwfail(OpcUa_True);
            node->state_bitwise += PDU::STATE::HWFAIL;
        }
        else {
            node->setStates_state_hwfail(OpcUa_False);
        }
        if (state & PDU::STATE::SERVICE) {
            node->setStates_state_service(OpcUa_True);
            node->state_bitwise += PDU::STATE::SERVICE;
        }
        else {
            node->setStates_state_service(OpcUa_False);
        }
        if (state & PDU::STATE::TIMADJ) {
            node->setStates_state_timeadj(OpcUa_True);
            node->state_bitwise += PDU::STATE::TIMADJ;
        }
        else {
            node->setStates_state_timeadj(OpcUa_False);
        }
    }


    void PDUObjectType::setStateBitwise(int state)
    {
        this->state_bitwise = state;
    }

    void PDUObjectType::resetStates()
    {
        this->setStates_state_valid(OpcUa_False);
        this->setStates_state_invalid(OpcUa_False);
        this->setStates_state_uninit(OpcUa_False);
        this->setStates_state_repval(OpcUa_False);
        this->setStates_state_simulat(OpcUa_False);
        this->setStates_state_doubt(OpcUa_False);
        this->setStates_state_hwfail(OpcUa_False);
        this->setStates_state_service(OpcUa_False);
        this->setStates_state_timeadj(OpcUa_False);
    }


} // End namespace for the UA information model http://yourorganisation.org/XU_Test_ServerConfig/