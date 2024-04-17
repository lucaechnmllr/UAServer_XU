#include "xu_nodemanagerxunamespace.h"
//#include "xu_browsenames.h"
#include "xu_floatpduobjecttype.h"
#include "xu_intpduobjecttype.h"
#include "xu_pduobjecttype.h"
#include "xu_xuobjecttype.h"

#pragma warning(push,2)
#include <uabasenodes.h>
#include "instancefactory.h"
#include "nodemanagerroot.h"
#include "opcua_foldertype.h"
#pragma warning(pop)

#include "PDU.h"
#include "PDU_String.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include "shutdown.h"

// Namespace for the UA information model http://yourorganisation.org/XU_Test_ServerConfig/
namespace XU {
    OpcUa_UInt16 NodeManagerXUNamespace::s_namespaceIndex = 0;

    /** Construction of the class NodeManagerXUNamespace.
    */
    NodeManagerXUNamespace::NodeManagerXUNamespace(
        OpcUa_Boolean firesEvents,  //!< [in] Flag indicating if the NodeManager should activate the EventManager
        OpcUa_Int32 nHashTableSize) //!< [in] Size of the hash table. The default value is 10.007.<br>
        //   The size can be smaller than the total number of nodes managed
        //   by the node manager but it is better for the performance to avoid
        //   collisions with a large size.
        //   Prefered sizes are 1.009, 10.007, 100.003, 1.000.003, 10.000.019.
        : NodeManagerBase("http://yourorganisation.org/UaServer/", firesEvents, nHashTableSize), m_stopThread(false), xu_object_ref(NULL)
    {
        m_defaultLocaleId = "en";
    }

    UaStatus NodeManagerXUNamespace::afterStartUp()
    {
        UaStatus ret;

        s_namespaceIndex = getNameSpaceIndex();

        ret = createDataTypeNodes();
        UA_ASSERT(ret.isGood());
        ret = createReferenceTypeNodes();
        UA_ASSERT(ret.isGood());

        // ------------------------------------------------------------
        // Create object type nodes
        // ------------------------------------------------------------
        PDUObjectType::createTypes();
        FloatPDUObjectType::createTypes();
        IntPDUObjectType::createTypes();
        XUObjectType::createTypes();
        // ------------------------------------------------------------

        // ------------------------------------------------------------
        // Create variable type nodes
        // ------------------------------------------------------------
        // ------------------------------------------------------------

        ret = addAdditionalHierarchicalReferences();
        UA_ASSERT(ret.isGood());
        if (ret.isBad()) return ret;
        ret = addAdditionalNonHierarchicalReferences();
        UA_ASSERT(ret.isGood());
        if (ret.isBad()) return ret;


#if SUPPORT_Event_Subscription_Server_Facet
        std::set<UaNodeId> setBrowsedNodes;
        recursiveRegisterEventSources(OpcUaId_Server, setBrowsedNodes);
        setBrowsedNodes.clear();
#endif // SUPPORT_Event_Subscription_Server_Facet


        //---------------------------- Create XU Object to reference the Signals -----------------------------------
        XU::XUObjectType* pObject = new XU::XUObjectType(
            UaNodeId("XU", getNameSpaceIndex()), // NodeId
            "XU", // Name
            getNameSpaceIndex(), // Name space index
            this); // Interface for adding nodes
        addNodeAndReference(OpcUaId_ObjectsFolder, pObject, OpcUaId_Organizes);
        //----------------------------------------------------------------------------------------------------------

        xu_object_ref = pObject;

        start();

        return ret;
    }


    /** Start shut down in derived class before shutting down base class.
    */
    UaStatus NodeManagerXUNamespace::beforeShutDown()
    {
        UaStatus ret;

        m_stopThread = true;
        wait();

        return ret;
    }


    /** Static method to get the namespace index for the type namespace managed by this class.
 */
    OpcUa_UInt16 NodeManagerXUNamespace::getTypeNamespace()
    {
        return s_namespaceIndex;
    }

    /** Helper function to create data type related nodes.
    */
    UaStatus NodeManagerXUNamespace::createDataTypeNodes()
    {
        UaStatus ret;



        return ret;
    }

    /** Helper function to create reference type related nodes.
    */
    UaStatus NodeManagerXUNamespace::createReferenceTypeNodes()
    {
        UaStatus                ret;

        return ret;
    }


    UaStatus NodeManagerXUNamespace::addAdditionalHierarchicalReferences()
    {
        UaStatus ret;

        return ret;
    }

    UaStatus NodeManagerXUNamespace::addAdditionalNonHierarchicalReferences()
    {
        UaStatus ret;

        return ret;
    }

#if SUPPORT_Event_Subscription_Server_Facet
    // browse from server object following HasEventSource references
    UaStatus NodeManagerXUNamespace::recursiveRegisterEventSources(const UaNodeId& startingNode, std::set<UaNodeId>& setBrowsedNodes)
    {
        UaStatus ret;

        if (setBrowsedNodes.find(startingNode) != setBrowsedNodes.end())
        {
            return OpcUa_Good;
        }

        ContinuationPointWrapper continuationPoint;
        UaReferenceDescriptions references;
        ret = m_pServerManager->browse(
            startingNode,
            OpcUa_False,
            UaNodeId(OpcUaId_HasEventSource),
            OpcUa_NodeClass_Object | OpcUa_NodeClass_Variable,
            continuationPoint,
            references);

        if (ret.isGood())
        {
            setBrowsedNodes.insert(startingNode);
            // only use NodeIds of this namespace
            for (OpcUa_UInt32 i = 0; i < references.length(); i++)
            {
                if (references[i].NodeId.NodeId.NamespaceIndex == getNameSpaceIndex())
                {
                    // ToDo - right now this only works for HasEventSource and HasNotifier not for any subtypes of those reference types
                    if (references[i].ReferenceTypeId.NamespaceIndex == 0 && references[i].ReferenceTypeId.Identifier.Numeric == OpcUaId_HasEventSource)
                    {
                        registerEventSource(startingNode, UaNodeId(references[i].NodeId.NodeId));
                        recursiveRegisterEventSources(references[i].NodeId.NodeId, setBrowsedNodes);
                    }
                    else if (references[i].ReferenceTypeId.NamespaceIndex == 0 && references[i].ReferenceTypeId.Identifier.Numeric == OpcUaId_HasNotifier)
                    {
                        registerEventNotifier(startingNode, UaNodeId(references[i].NodeId.NodeId));
                        recursiveRegisterEventSources(references[i].NodeId.NodeId, setBrowsedNodes);
                    }
                }
            }
        }
        return ret;
    }
#endif // SUPPORT_Event_Subscription_Server_Facet

	void NodeManagerXUNamespace::run()

	{
		using std::string;
		using std::unordered_map;
		using std::endl;

		typedef std::shared_ptr<PDU::basePDU> pdu_ptr_t;

		string line;
		//std::ifstream ifs;

		static OpcUa_Int32 signal_count = 0;
		OpcUa_UInt16 namespaceindex = this->getNameSpaceIndex();


		while (m_stopThread == OpcUa_False && ShutDownFlag() == 0)
		{
			std::getline(std::cin, line); // Wait for input stream
			try {
				auto temppdu = pdu_ptr_t(PDU::StringtoPDU(line, ':')); //Create shared pointer

				std::stringstream sstr;
				std::string folder_system = temppdu->getFolderName();
				sstr << folder_system << temppdu->getSignalName();
				std::string folder_signaltype = sstr.str();

				switch (temppdu->getType())
				{
				case PDU::TYPE::FLOAT:
				{
					//if (!object_list_float.contains(temppdu->getKKS()))
					if (!this->getNode(UaNodeId(temppdu->getKKS().c_str(), namespaceindex)))
					{
						//if (!folder_list.contains(folder_system)) // If folder for specific object doesn't already exist
						if (!this->getNode(UaNodeId(folder_system.c_str(), namespaceindex)))
						{
							//------------------ Create new folder Object --------------------------------------
							OpcUa::FolderType* pFolderSystem = new OpcUa::FolderType(
								UaNodeId(folder_system.c_str(), namespaceindex), // NodeId
								folder_system.c_str(), // Name
								namespaceindex, // Name space index
								this); // Interface for adding nodes
							addNodeAndReference(xu_object_ref, pFolderSystem, OpcUaId_Organizes);
							//-----------------------------------------------------------------------------------
							//------------------ Add folder to folder List --------------------------------------
							//folder_list[folder_system] = pFolderSystem;
							//-----------------------------------------------------------------------------------

							//------------------ Create new folder Object --------------------------------------
							OpcUa::FolderType* pFolderSignal = new OpcUa::FolderType(
								UaNodeId(folder_signaltype.c_str(), namespaceindex), // NodeId
								folder_signaltype.c_str(), // Name
								namespaceindex, // Name space index
								this); // Interface for adding nodes
							addNodeAndReference(UaNodeId(folder_system.c_str(), namespaceindex), pFolderSignal, OpcUaId_Organizes);
							//-----------------------------------------------------------------------------------
							//------------------ Add folder to folder List --------------------------------------
							//folder_list[folder_signaltype] = pFolderSignal;
							//-----------------------------------------------------------------------------------
						}
						//else if (!folder_list.contains(folder_signaltype))
						else if (!this->getNode(UaNodeId(folder_signaltype.c_str(), namespaceindex)))
						{
							//------------------ Create new folder Object --------------------------------------
							OpcUa::FolderType* pFolderSignal = new OpcUa::FolderType(
								UaNodeId(folder_signaltype.c_str(), namespaceindex), // NodeId
								temppdu->getSignalName().c_str(), // Name
								namespaceindex, // Name space index
								this); // Interface for adding nodes
							addNodeAndReference(UaNodeId(folder_system.c_str(), namespaceindex), pFolderSignal, OpcUaId_Organizes);
							//-----------------------------------------------------------------------------------
							//------------------ Add folder to folder List --------------------------------------
							//folder_list[folder_signaltype] = pFolderSignal;
							//-----------------------------------------------------------------------------------
						}
						//-------------------- Create new FloatPDU Object -----------------------------------
						XU::FloatPDUObjectType* pObjectFloat = new XU::FloatPDUObjectType(
							UaNodeId(temppdu->getKKS().c_str(), namespaceindex),
							temppdu->getKKS().c_str(),
							namespaceindex,
							this,
							temppdu->getFloat(),
							temppdu->getState(),
							temppdu->getTime());

						addNodeAndReference(UaNodeId(folder_signaltype.c_str(), namespaceindex), pObjectFloat, OpcUaId_HasComponent); //Add to folder

						//-------------------- Add to object list -------------------------------------------
						object_list_float[temppdu->getKKS()] = pObjectFloat;
						//-----------------------------------------------------------------------------------
						signal_count++;
					}
					else // PDU does already exist
					{
						//Check if the value has changed
						if (object_list_float.at(temppdu->getKKS())->getValue() != temppdu->getFloat())
						{
							object_list_float[temppdu->getKKS()]->setValue(temppdu->getFloat()); // Set new value
						}
						//Check if the state has changed
						if (object_list_float.at(temppdu->getKKS())->getStateBitwise() != temppdu->getState())
						{
							object_list_float[temppdu->getKKS()]->setStates(object_list_float[temppdu->getKKS()], temppdu->getState()); // Set new state
						}
						object_list_float[temppdu->getKKS()]->setTimestamp(UaDateTime::fromTime_t(static_cast<time_t>(temppdu->getTime().tv_sec) + (temppdu->getTime().tv_usec) / 1000000)); // Set new timestamp
					}
					break;
				}
				case PDU::TYPE::INT:
				{
					//if (!object_list_int.contains(temppdu->getKKS()))
					if (!this->getNode(UaNodeId(temppdu->getKKS().c_str(), namespaceindex)))
					{
						//if (!folder_list.contains(folder_system)) // If folder for specific object doesn't already exist
						if (!this->getNode(UaNodeId(folder_system.c_str(), namespaceindex)))
						{
							//------------------ Create new folder Object --------------------------------------
							OpcUa::FolderType* pFolderSystem = new OpcUa::FolderType(
								UaNodeId(folder_system.c_str(), namespaceindex), // NodeId
								folder_system.c_str(), // Name
								namespaceindex, // Name space index
								this); // Interface for adding nodes
							addNodeAndReference(xu_object_ref, pFolderSystem, OpcUaId_Organizes);
							//-----------------------------------------------------------------------------------
							//------------------ Add folder to folder List --------------------------------------
							//folder_list[folder_system] = pFolderSystem;
							//-----------------------------------------------------------------------------------
						}
						//if (!folder_list.contains(folder_signaltype))
						if (!this->getNode(UaNodeId(folder_signaltype.c_str(), namespaceindex)))
						{
							//------------------ Create new folder Object --------------------------------------
							OpcUa::FolderType* pFolderSignal = new OpcUa::FolderType(
								UaNodeId(folder_signaltype.c_str(), namespaceindex), // NodeId
								temppdu->getSignalName().c_str(), // Name
								namespaceindex, // Name space index
								this); // Interface for adding nodes

							addNodeAndReference(UaNodeId(folder_system.c_str(), namespaceindex), pFolderSignal, OpcUaId_Organizes);
							//-----------------------------------------------------------------------------------
							//------------------ Add folder to folder List --------------------------------------
							//folder_list[folder_signaltype] = pFolderSignal;
							//-----------------------------------------------------------------------------------
						}
						//-------------------- Create new IntPDU Object -------------------------------------
						XU::IntPDUObjectType* pObjectInt = new XU::IntPDUObjectType(
							UaNodeId(temppdu->getKKS().c_str(), namespaceindex),
							temppdu->getKKS().c_str(),
							namespaceindex,
							this,
							temppdu->getInt(),
							temppdu->getState(),
							temppdu->getTime());

						addNodeAndReference(UaNodeId(folder_signaltype.c_str(), namespaceindex), pObjectInt, OpcUaId_HasComponent); //Add to folder

						//--------------------------- Add to object list ------------------------------------
						object_list_int[temppdu->getKKS()] = pObjectInt;
						//-----------------------------------------------------------------------------------
						signal_count++;
					}
					else
					{
						//Check if the value has changed
						if (object_list_int.at(temppdu->getKKS())->getValue() != temppdu->getInt())
						{
							object_list_int[temppdu->getKKS()]->setValue(temppdu->getInt()); //Set new value
						}
						//Check if state has changed
						if (object_list_int.at(temppdu->getKKS())->getStateBitwise() != temppdu->getState())
						{
							object_list_int[temppdu->getKKS()]->setStates(object_list_int[temppdu->getKKS()], temppdu->getState()); //Set new state
						}
						object_list_int[temppdu->getKKS()]->setTimestamp(UaDateTime::fromTime_t(static_cast<time_t>(temppdu->getTime().tv_sec) + temppdu->getTime().tv_usec)); // Set new timestamp
					}
					break;
				}
				default:
					throw std::runtime_error("Fehler bei Erstellung des Namespaces: Server wird gestoppt!");
					break;
				}
			}
			catch (std::exception e) {
				if (ShutDownFlag() == 0)
					std::cerr << "Fehler bei Umwandlung in PDU: " << e.what() << endl;
			}
			// update signal count
			xu_object_ref->setSignals(signal_count);
		}
	}


} // End namespace for the UA information model http://yourorganisation.org/UAServer/
