
#pragma warning(push,0)
#include "uagenericnodes.h"
#include "nodemanagerroot.h"
#include "methodhandleuanode.h"
#include "instancefactory.h"
#include "opcua_extensionobject.h"
#pragma warning(pop)

#include "xu_intpduobjecttype.h"
#include "xu_nodemanagerxunamespace.h"

#include <iostream>

// Namespace for the UA information model http://yourorganisation.org/XU_Test_ServerConfig/
namespace XU {

    bool IntPDUObjectType::s_typeNodesCreated = false;
    OpcUa::BaseMethod* IntPDUObjectType::s_pwriteSignalMethod = NULL;
    OpcUa::DataItemType* IntPDUObjectType::s_pValue = NULL;

    /** Constructs an IntPDUObjectType object using an instance declaration node as base
    */
    IntPDUObjectType::IntPDUObjectType(const UaNodeId& nodeId, UaObject* pInstanceDeclarationObject, NodeManagerConfig* pNodeConfig, UaMutexRefCounted* pSharedMutex)
        : XU::PDUObjectType(nodeId, pInstanceDeclarationObject, pNodeConfig, pSharedMutex)
    {
        initialize(0);
    }

    /** Constructs an IntPDUObjectType object
    */
    IntPDUObjectType::IntPDUObjectType(
        const UaNodeId& nodeId, 
        const UaString& name, 
        OpcUa_UInt16 browseNameNameSpaceIndex, 
        NodeManagerConfig* pNodeConfig, int value,
        int state,
        PDU::timeval time,
        bool write_permitted,
        UaMutexRefCounted* pSharedMutex)
        : XU::PDUObjectType(nodeId, name, browseNameNameSpaceIndex, pNodeConfig, time, write_permitted, pSharedMutex)
    {
        initialize(value);
        this->setStates(this, state);
    }

    /**  Constructs an instance of the class IntPDUObjectType with all components
    */
    IntPDUObjectType::IntPDUObjectType(
        UaBase::Object* pBaseNode,    //!< [in] The "tree" of nodes that specifies the attributes of the instance and its children
        XmlUaNodeFactoryManager* pFactory, //!< [in] The factory to create the children
        NodeManagerConfig* pNodeConfig,  //!< [in] Interface pointer to the NodeManagerConfig interface used to add and delete node and references in the address space
        UaMutexRefCounted* pSharedMutex) //!< [in] Shared mutex object used to synchronize access to the variable. Can be NULL if no shared mutex is provided
        : XU::PDUObjectType(pBaseNode, pFactory, pNodeConfig, pSharedMutex)
    {
        UaStatus      addStatus;

        if (s_typeNodesCreated == false)
        {
            createTypes();
        }

        m_pValue = NULL;
        m_pwriteSignalMethod = NULL;

        std::list<UaBase::BaseNode*> lstReferencedNodes = pBaseNode->hierarchicallyReferencedNodes();
        for (std::list<UaBase::BaseNode*>::const_iterator it = lstReferencedNodes.begin(); it != lstReferencedNodes.end(); it++)
        {
            UaBase::BaseNode* pChild = *it;
            if (pChild->browseName() == UaQualifiedName("Value", XU::NodeManagerXUNamespace::getTypeNamespace()))
            {
                if (!m_pValue)
                {
                    m_pValue = (OpcUa::DataItemType*)pFactory->createVariable((UaBase::Variable*)pChild, pNodeConfig, pSharedMutex);
                    addStatus = pNodeConfig->addNodeAndReference(this, m_pValue, OpcUaId_HasComponent);
                    UA_ASSERT(addStatus.isGood());
                    if (!((UaBase::Variable*)pChild)->modellingRuleId().isNull())
                    {
                        m_pValue->setModellingRuleId(((UaBase::Variable*)pChild)->modellingRuleId());
                    }
                }
            }
            else if (s_pwriteSignalMethod && pChild->browseName() == s_pwriteSignalMethod->browseName())
            {
                if (!m_pwriteSignalMethod)
                {
                    m_pwriteSignalMethod = (OpcUa::BaseMethod*)pFactory->createMethod((UaBase::Method*)pChild, pNodeConfig, pSharedMutex);
                    addStatus = pNodeConfig->addNodeAndReference(this, m_pwriteSignalMethod, OpcUaId_HasComponent);
                    UA_ASSERT(addStatus.isGood());
                    if (!((UaBase::Method*)pChild)->modellingRuleId().isNull())
                    {
                        m_pwriteSignalMethod->setModellingRuleId(((UaBase::Method*)pChild)->modellingRuleId());
                    }
                }
            }
        }
        UA_ASSERT(m_pValue);
        UA_ASSERT(m_pwriteSignalMethod);
    }

    /** Initialize the object with all member nodes
    */
    void IntPDUObjectType::initialize(int init_value)
    {
        OpcUa_Int16 nsIdx = m_pNodeConfig->getNameSpaceIndex();
        UaStatus      addStatus;
        UaVariant     defaultValue;
        UaUInt32Array arrayDimensions;
        UaPropertyMethodArgument* pPropertyArg = NULL;

        if (s_typeNodesCreated == false)
        {
            createTypes();
        }

        m_pValue = NULL;
        m_pwriteSignalMethod = NULL;
        // Mandatory variable Value
        m_pValue = new OpcUa::DataItemType(this, s_pValue, m_pNodeConfig, m_pSharedMutex);
        addStatus = m_pNodeConfig->addNodeAndReference(this, m_pValue, OpcUaId_HasComponent);
        // Set init value
        UaVariant val;
        val.setInt16((OpcUa_Int16)init_value);
        UaDataValue dataValue;
        dataValue.setValue(val, OpcUa_True, OpcUa_True);
        m_pValue->setValue(NULL, dataValue, OpcUa_False);
        UA_ASSERT(addStatus.isGood());


        // Method writeSignal
        m_pwriteSignalMethod = new OpcUa::BaseMethod(this, s_pwriteSignalMethod, m_pSharedMutex);
        addStatus = m_pNodeConfig->addNodeAndReference(this, m_pwriteSignalMethod, OpcUaId_HasComponent);
        UA_ASSERT(addStatus.isGood());
        // ------------------------------------------------------------------------
        // Add InputArguments
        // Create property
        pPropertyArg = new UaPropertyMethodArgument(
            UaNodeId(UaString("%1.writeSignal.InputArguments").arg(nodeId().toString()), nsIdx), // NodeId of the property
            Ua_AccessLevel_CurrentRead,             // Access level of the property
            9,                                      // Number of arguments
            UaPropertyMethodArgument::INARGUMENTS); // IN arguments
        // Argument newVal
        pPropertyArg->setArgument(
            0,                      // Index of the argument
            "newVal",             // Name of the argument
            UaNodeId(OpcUaId_Int16),// Data type of the argument
            -1,                     // ValueRank of the argument
            arrayDimensions,
            UaLocalizedText("en", "")); // Description
        // Argument setValid
        pPropertyArg->setArgument(
            1,                      // Index of the argument
            "setValid",             // Name of the argument
            UaNodeId(OpcUaId_Boolean),// Data type of the argument
            -1,                     // ValueRank of the argument
            arrayDimensions,
            UaLocalizedText("en", "Valid")); // Description
        // Argument setInvalid
        pPropertyArg->setArgument(
            2,                      // Index of the argument
            "setInvalid",             // Name of the argument
            UaNodeId(OpcUaId_Boolean),// Data type of the argument
            -1,                     // ValueRank of the argument
            arrayDimensions,
            UaLocalizedText("en", "Invalid")); // Description
        // Argument setDoubt
        pPropertyArg->setArgument(
            3,                      // Index of the argument
            "setDoubt",             // Name of the argument
            UaNodeId(OpcUaId_Boolean),// Data type of the argument
            -1,                     // ValueRank of the argument
            arrayDimensions,
            UaLocalizedText("en", "Doubtful")); // Description
        // Argument setHwFail
        pPropertyArg->setArgument(
            4,                      // Index of the argument
            "setHwFail",             // Name of the argument
            UaNodeId(OpcUaId_Boolean),// Data type of the argument
            -1,                     // ValueRank of the argument
            arrayDimensions,
            UaLocalizedText("en", "HW failure")); // Description
        // Argument setRepval
        pPropertyArg->setArgument(
            5,                      // Index of the argument
            "setRepval",             // Name of the argument
            UaNodeId(OpcUaId_Boolean),// Data type of the argument
            -1,                     // ValueRank of the argument
            arrayDimensions,
            UaLocalizedText("en", "Replacement value")); // Description
        // Argument setService
        pPropertyArg->setArgument(
            6,                      // Index of the argument
            "setService",             // Name of the argument
            UaNodeId(OpcUaId_Boolean),// Data type of the argument
            -1,                     // ValueRank of the argument
            arrayDimensions,
            UaLocalizedText("en", "In service")); // Description
        // Argument setTimeAdj
        pPropertyArg->setArgument(
            7,                      // Index of the argument
            "setTimeAdj",             // Name of the argument
            UaNodeId(OpcUaId_Boolean),// Data type of the argument
            -1,                     // ValueRank of the argument
            arrayDimensions,
            UaLocalizedText("en", "Time adjusted")); // Description
        // Argument setSimulat
        pPropertyArg->setArgument(
            8,                      // Index of the argument
            "setSimulat",             // Name of the argument
            UaNodeId(OpcUaId_Boolean),// Data type of the argument
            -1,                     // ValueRank of the argument
            arrayDimensions,
            UaLocalizedText("en", "Simulated")); // Description
        // Add property to method
        addStatus = m_pNodeConfig->addNodeAndReference(m_pwriteSignalMethod, pPropertyArg, OpcUaId_HasProperty);
        UA_ASSERT(addStatus.isGood());

    }

    /** Destruction
    */
    IntPDUObjectType::~IntPDUObjectType()
    {
    }

    /** Create the related type nodes
    */
    void IntPDUObjectType::createTypes()
    {
        if (s_typeNodesCreated == false)
        {
            s_typeNodesCreated = true;

            // Check if supertype is already created
            XU::PDUObjectType::createTypes();

            UaStatus      addStatus;
            UaVariant     defaultValue;
            UaUInt32Array arrayDimensions;
            UaPropertyMethodArgument* pPropertyArg = NULL;
            NodeManagerRoot* pNodeManagerRoot = NodeManagerRoot::CreateRootNodeManager();
            OpcUa_Int16 nsTypeIdx = NodeManagerXUNamespace::getTypeNamespace();
            OpcUa_Int16 nsSuperTypeIdx = XU::NodeManagerXUNamespace::getTypeNamespace();
            NodeManagerConfig* pTypeNodeConfig = pNodeManagerRoot->getNodeManagerByNamespace(nsTypeIdx)->getNodeManagerConfig();

            OpcUa::GenericObjectType* pObjectType;
            pObjectType = new OpcUa::GenericObjectType(
                UaNodeId(XUId_IntPDUObjectType, nsTypeIdx),
                UaQualifiedName("IntPDUObjectType", nsTypeIdx),
                UaLocalizedText("", "IntPDUObjectType"),
                UaLocalizedText("", ""),
                OpcUa_False,
                &IntPDUObjectType::clearStaticMembers);
            addStatus = pTypeNodeConfig->addNodeAndReference(UaNodeId(XUId_PDUObjectType, nsSuperTypeIdx), pObjectType, OpcUaId_HasSubtype);
            UA_ASSERT(addStatus.isGood());
            // ------------------------------------------------------------------------
            // Method writeSignal
            // ------------------------------------------------------------------------
            s_pwriteSignalMethod = new OpcUa::BaseMethod(UaNodeId(XUId_IntPDUObjectType_writeSignal, nsTypeIdx), "writeSignal", XU::NodeManagerXUNamespace::getTypeNamespace());
            s_pwriteSignalMethod->setModellingRuleId(OpcUaId_ModellingRule_Mandatory);
            addStatus = pTypeNodeConfig->addNodeAndReference(pObjectType, s_pwriteSignalMethod, OpcUaId_HasComponent);
            UA_ASSERT(addStatus.isGood());
            // ------------------------------------------------------------------------
            // Add InputArguments
            // Create property
            pPropertyArg = new UaPropertyMethodArgument(
                UaNodeId(XUId_IntPDUObjectType_writeSignal_InputArguments, nsTypeIdx),       // NodeId of the property
                Ua_AccessLevel_CurrentRead,             // Access level of the property
                9,                                      // Number of arguments
                UaPropertyMethodArgument::INARGUMENTS); // IN arguments
            // Argument newVal
            pPropertyArg->setArgument(
                0,                      // Index of the argument
                "newVal",             // Name of the argument
                UaNodeId(OpcUaId_Int16),// Data type of the argument
                -1,                     // Array rank of the argument
                arrayDimensions,
                UaLocalizedText("en", "")); // Description
            // Argument setValid
            pPropertyArg->setArgument(
                1,                      // Index of the argument
                "setValid",             // Name of the argument
                UaNodeId(OpcUaId_Boolean),// Data type of the argument
                -1,                     // Array rank of the argument
                arrayDimensions,
                UaLocalizedText("en", "Valid")); // Description
            // Argument setInvalid
            pPropertyArg->setArgument(
                2,                      // Index of the argument
                "setInvalid",             // Name of the argument
                UaNodeId(OpcUaId_Boolean),// Data type of the argument
                -1,                     // Array rank of the argument
                arrayDimensions,
                UaLocalizedText("en", "Invalid")); // Description
            // Argument setDoubt
            pPropertyArg->setArgument(
                3,                      // Index of the argument
                "setDoubt",             // Name of the argument
                UaNodeId(OpcUaId_Boolean),// Data type of the argument
                -1,                     // Array rank of the argument
                arrayDimensions,
                UaLocalizedText("en", "Doubtful")); // Description
            // Argument setHwFail
            pPropertyArg->setArgument(
                4,                      // Index of the argument
                "setHwFail",             // Name of the argument
                UaNodeId(OpcUaId_Boolean),// Data type of the argument
                -1,                     // Array rank of the argument
                arrayDimensions,
                UaLocalizedText("en", "HW failure")); // Description
            // Argument setRepval
            pPropertyArg->setArgument(
                5,                      // Index of the argument
                "setRepval",             // Name of the argument
                UaNodeId(OpcUaId_Boolean),// Data type of the argument
                -1,                     // Array rank of the argument
                arrayDimensions,
                UaLocalizedText("en", "Replacement value")); // Description
            // Argument setService
            pPropertyArg->setArgument(
                6,                      // Index of the argument
                "setService",             // Name of the argument
                UaNodeId(OpcUaId_Boolean),// Data type of the argument
                -1,                     // Array rank of the argument
                arrayDimensions,
                UaLocalizedText("en", "In service")); // Description
            // Argument setTimeAdj
            pPropertyArg->setArgument(
                7,                      // Index of the argument
                "setTimeAdj",             // Name of the argument
                UaNodeId(OpcUaId_Boolean),// Data type of the argument
                -1,                     // Array rank of the argument
                arrayDimensions,
                UaLocalizedText("en", "Time adjusted")); // Description
            // Argument setSimulat
            pPropertyArg->setArgument(
                8,                      // Index of the argument
                "setSimulat",             // Name of the argument
                UaNodeId(OpcUaId_Boolean),// Data type of the argument
                -1,                     // Array rank of the argument
                arrayDimensions,
                UaLocalizedText("en", "Simulated")); // Description
            // Set the ModellingRule to InputArguments
            pPropertyArg->setModellingRuleId(OpcUaId_ModellingRule_Mandatory);
            // Add property to method
            pTypeNodeConfig->addNodeAndReference(s_pwriteSignalMethod, pPropertyArg, OpcUaId_HasProperty);


            // Mandatory variable Value
            defaultValue.setInt16(0);
            s_pValue = new OpcUa::DataItemType(UaNodeId(XUId_IntPDUObjectType_Value, nsTypeIdx), "Value", XU::NodeManagerXUNamespace::getTypeNamespace(), defaultValue, 1, pTypeNodeConfig);
            s_pValue->setDescription(UaLocalizedText("", "Datatype: Int\\nValue can be either 0 oder 1"));
            s_pValue->setModellingRuleId(OpcUaId_ModellingRule_Mandatory);
            s_pValue->setValueRank(-1);
            addStatus = pTypeNodeConfig->addNodeAndReference(pObjectType, s_pValue, OpcUaId_HasComponent);
            UA_ASSERT(addStatus.isGood());

        }
    }

    /** Clear the static members of the class
    */
    void IntPDUObjectType::clearStaticMembers()
    {
        s_typeNodesCreated = false;

        s_pValue = NULL;
        s_pwriteSignalMethod = NULL;
    }

    /** Returns the type definition NodeId for the IntPDUObjectType
    */
    UaNodeId IntPDUObjectType::typeDefinitionId() const
    {
        UaNodeId ret(XUId_IntPDUObjectType, NodeManagerXUNamespace::getTypeNamespace());
        return ret;
    }

    /** Implementation of the MethodManager interface method beginCall
    */
    UaStatus IntPDUObjectType::beginCall(
        MethodManagerCallback* pCallback,
        const ServiceContext& serviceContext,
        OpcUa_UInt32           callbackHandle,
        MethodHandle* pMethodHandle,
        const UaVariantArray& inputArguments)
    {
        UaStatus            ret;

        OpcUa::MethodCallJob* pCallJob = new OpcUa::MethodCallJob;
        pCallJob->initialize(this, pCallback, serviceContext, callbackHandle, pMethodHandle, inputArguments);
        ret = NodeManagerRoot::CreateRootNodeManager()->pServerManager()->getThreadPool()->addJob(pCallJob);
        if (ret.isBad())
        {
            delete pCallJob;
        }

        return ret;
    }

    /** Synchronous version of the MethodManager method call executed in a worker thread
    */
    UaStatus IntPDUObjectType::call(
        const ServiceContext& serviceContext,
        MethodHandle* pMethodHandle,
        const UaVariantArray& inputArguments,
        UaVariantArray& outputArguments,
        UaStatusCodeArray& inputArgumentResults,
        UaDiagnosticInfos& inputArgumentDiag)
    {
        UaStatus            ret;
        MethodHandleUaNode* pMethodHandleUaNode = (MethodHandleUaNode*)pMethodHandle;
        UaMethod* pMethod = NULL;

        if (pMethodHandleUaNode)
        {
            pMethod = pMethodHandleUaNode->pUaMethod();

            if (pMethod)
            {
                if (((m_pwriteSignalMethod != NULL) && (pMethod->nodeId() == m_pwriteSignalMethod->nodeId()))
                    || ((s_pwriteSignalMethod != NULL) && (pMethod->nodeId() == s_pwriteSignalMethod->nodeId())))
                {
                    if (inputArguments.length() < 9)
                    {
                        ret = OpcUa_BadArgumentsMissing;
                    }
                    else if (inputArguments.length() > 9)
                    {
                        ret = OpcUa_BadTooManyArguments;
                    }
                    else
                    {
                        inputArgumentResults.create(9);

                        UaVariant value;
                        UaStatus tmpRet;

                        OpcUa_Int16 newVal;
                        value = inputArguments[0];
                        tmpRet = value.toInt16(newVal);
                        inputArgumentResults[0] = tmpRet.statusCode();
                        if (tmpRet.isNotGood()) { ret = tmpRet; }

                        OpcUa_Boolean setValid;
                        value = inputArguments[1];
                        tmpRet = value.toBool(setValid);
                        inputArgumentResults[1] = tmpRet.statusCode();
                        if (tmpRet.isNotGood()) { ret = tmpRet; }

                        OpcUa_Boolean setInvalid;
                        value = inputArguments[2];
                        tmpRet = value.toBool(setInvalid);
                        inputArgumentResults[2] = tmpRet.statusCode();
                        if (tmpRet.isNotGood()) { ret = tmpRet; }

                        OpcUa_Boolean setDoubt;
                        value = inputArguments[3];
                        tmpRet = value.toBool(setDoubt);
                        inputArgumentResults[3] = tmpRet.statusCode();
                        if (tmpRet.isNotGood()) { ret = tmpRet; }

                        OpcUa_Boolean setHwFail;
                        value = inputArguments[4];
                        tmpRet = value.toBool(setHwFail);
                        inputArgumentResults[4] = tmpRet.statusCode();
                        if (tmpRet.isNotGood()) { ret = tmpRet; }

                        OpcUa_Boolean setRepval;
                        value = inputArguments[5];
                        tmpRet = value.toBool(setRepval);
                        inputArgumentResults[5] = tmpRet.statusCode();
                        if (tmpRet.isNotGood()) { ret = tmpRet; }

                        OpcUa_Boolean setService;
                        value = inputArguments[6];
                        tmpRet = value.toBool(setService);
                        inputArgumentResults[6] = tmpRet.statusCode();
                        if (tmpRet.isNotGood()) { ret = tmpRet; }

                        OpcUa_Boolean setTimeAdj;
                        value = inputArguments[7];
                        tmpRet = value.toBool(setTimeAdj);
                        inputArgumentResults[7] = tmpRet.statusCode();
                        if (tmpRet.isNotGood()) { ret = tmpRet; }

                        OpcUa_Boolean setSimulat;
                        value = inputArguments[8];
                        tmpRet = value.toBool(setSimulat);
                        inputArgumentResults[8] = tmpRet.statusCode();
                        if (tmpRet.isNotGood()) { ret = tmpRet; }

                        if (ret.isGood())
                        {
                            ret = this->writeSignal(serviceContext, newVal, setValid, setInvalid, setDoubt, setHwFail, setRepval, setService, setTimeAdj, setSimulat);
                        }
                    }

                }
                else
                {
                    return PDUObjectType::call(serviceContext, pMethodHandle, inputArguments, outputArguments, inputArgumentResults, inputArgumentDiag);
                }
            }
            else
            {
                assert(false);
                ret = OpcUa_BadInvalidArgument;
            }
        }
        else
        {
            assert(false);
            ret = OpcUa_BadInvalidArgument;
        }

        return ret;
    }

    UaStatus IntPDUObjectType::writeSignal(
        const ServiceContext&,
        OpcUa_Int16 newVal,
        OpcUa_Boolean setValid,
        OpcUa_Boolean setInvalid,
        OpcUa_Boolean setDoubt,
        OpcUa_Boolean setHwFail,
        OpcUa_Boolean setRepval,
        OpcUa_Boolean setService,
        OpcUa_Boolean setTimeAdj,
        OpcUa_Boolean setSimulat)
    {
        UaStatus ret;
        int temp_state = 1; // Set state "Uninitialized"

        // If signal has no write permission -> fail
        if (!getWritePermission())
        {
            ret = OpcUa_BadNotWritable;
            return ret;
        }

        if (setValid)
            temp_state = PDU::STATE::VALID;
        if (setInvalid)
            temp_state |= PDU::STATE::INVALID;
        if (setRepval)
            temp_state |= PDU::STATE::REPVAL;
        if (setSimulat)
            temp_state |= PDU::STATE::SIMULAT;
        if (setDoubt)
            temp_state |= PDU::STATE::DOUBT;
        if (setHwFail)
            temp_state |= PDU::STATE::HWFAIL;
        if (setService)
            temp_state |= PDU::STATE::SERVICE;
        if (setTimeAdj)
            temp_state |= PDU::STATE::TIMADJ;



        //! Limit the value to positive numbers
        if (newVal >= 0)
        {
            //this->setValue(newVal);
        }
        //! Return Error Code -> Value out of Range
        else
        {
            ret = OpcUa_BadEdited_OutOfRange;
            return ret;
        }

        //! Check if State is possible
        if (setValid)
        {
            if (setInvalid)
            {
                ret = OpcUa_BadInvalidState;
                return ret;
            }
            else
            {
                this->setStateBitwise(temp_state);
            }
        }
        else
        {
            if (setInvalid)
            {
                this->setStateBitwise(temp_state);
            }
            else
            {
                ret = OpcUa_BadInvalidState;
                return ret;
            }
        }

        //std::cerr << "ServerMode: " << NodeManagerXUNamespace::getServerMode() << std::endl;

        //this->setValue(newVal);
        std::cout << PDU::PDUObjecttoString<OpcUa_Int16>(*this, newVal, NodeManagerXUNamespace::getServerMode()) << '\n';


        return ret;
    }

    /**
     *  Sets the Value value
     *
     *  Datatype: Int\nValue can be either 0 oder 1
     */
    void IntPDUObjectType::setValue(OpcUa_Int16 Value)
    {
        UaVariant value;
        value.setInt16(Value);
        UaDataValue dataValue;
        dataValue.setValue(value, OpcUa_True, OpcUa_True);
        m_pValue->setValue(NULL, dataValue, OpcUa_False);
    }

    /**
     *  Returns the value of Value
     *
     *  Datatype: Int\nValue can be either 0 oder 1
     */
    OpcUa_Int16 IntPDUObjectType::getValue() const
    {
        UaVariant defaultValue;
        OpcUa_Int16 ret = 0;
        UaDataValue dataValue(m_pValue->value(NULL));
        defaultValue = *dataValue.value();
        defaultValue.toInt16(ret);
        return ret;
    }

    /** Returns the Value node.
     */
    OpcUa::DataItemType* IntPDUObjectType::getValueNode()
    {
        return m_pValue;
    }

    /** Returns the Value node.
     */
    const OpcUa::DataItemType* IntPDUObjectType::getValueNode() const
    {
        return m_pValue;
    }

    /** Returns the writeSignal node.
      */
    OpcUa::BaseMethod* IntPDUObjectType::getwriteSignal()
    {
        return m_pwriteSignalMethod;
    }

    /** Returns the writeSignal node.
      */
    const OpcUa::BaseMethod* IntPDUObjectType::getwriteSignal() const
    {
        return m_pwriteSignalMethod;
    }

    /** Uses the NodeAccessInfo of pOther and its children. */
    void IntPDUObjectType::useAccessInfoFromInstance(IntPDUObjectType* pOther)
    {
        XU::PDUObjectType::useAccessInfoFromInstance(pOther);

        // children
        m_pValue->useAccessInfoFromInstance(pOther->m_pValue);
        m_pwriteSignalMethod->useAccessInfoFrom(pOther->m_pwriteSignalMethod);
    }

    /** Set the NodeAccessInfo for this node and all of its children. */
    void IntPDUObjectType::setAccessInfoWithChildren(NodeAccessInfo* pNodeAccessInfo)
    {
        XU::PDUObjectType::setAccessInfoWithChildren(pNodeAccessInfo);

        // children
        m_pValue->setAccessInfoWithChildren(pNodeAccessInfo);
        m_pwriteSignalMethod->setAccessInfoWithChildren(pNodeAccessInfo);
    }


} // End namespace for the UA information model http://yourorganisation.org/XU_Test_ServerConfig/
