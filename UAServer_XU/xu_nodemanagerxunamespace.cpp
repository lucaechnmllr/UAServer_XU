#pragma warning(push,0)
#include <uabasenodes.h>
#include "instancefactory.h"
#include "nodemanagerroot.h"
#include "opcua_foldertype.h"
#pragma warning(pop)

#include "xu_nodemanagerxunamespace.h"
#include "xu_floatpduobjecttype.h"
#include "xu_intpduobjecttype.h"
#include "xu_pduobjecttype.h"
#include "xu_xuobjecttype.h"
#include "shutdown.h"

#include "PDU.h"
#include "PDU_String.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <chrono>

constexpr auto TIMEZONE_MESZ = 2*60*60;

// Namespace for the UA information model http://yourorganisation.org/XU_Test_ServerConfig/
namespace XU {
	// Definition of the static variables
	OpcUa_UInt16 NodeManagerXUNamespace::s_namespaceIndex = 0;
	OpcUa_UInt16 NodeManagerXUNamespace::s_server_mode = 0;

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

		readWritePermissionFile();

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

	/** Static method to get the mode (0: normal, 1: rapid) of the server.
	*/
	OpcUa_UInt16 NodeManagerXUNamespace::getServerMode()
	{
		return s_server_mode;
	}

	
	void NodeManagerXUNamespace::setServerMode(OpcUa_UInt16 mode)
	{
		s_server_mode = mode;
		//std::cerr << "setServerMode called: " << s_server_mode << std::endl;
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

	void NodeManagerXUNamespace::readWritePermissionFile()
	{
		std::string line;
		std::ifstream file("C:/C++SDK_UA_OPC_gekauft/uasdkcppbundle-bin-windows-vs2015_x64-v1.8.3-628/bin/signals_write_permission.txt");



		if (file.is_open())
		{
			while (std::getline(file, line))
			{
				m_pWrite_perm_list.insert(line.c_str());
			}

			file.close();
		}
		else {
			perror("Open");
			std::cerr << "Error trying to open write permission file, check if file is available!\n";
			exit(-1);
		}
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

		typedef std::unique_ptr<PDU::basePDU> pdu_ptr_t;
		auto start_time = std::chrono::high_resolution_clock::now();

		string line;
		UaStatus addResult;

		static OpcUa_UInt64 signal_count = 0;
		UaDateTime datetime = 0;
		OpcUa_UInt16 namespaceindex = this->getNameSpaceIndex();

		// Short delay to connect the two interface programs correctly in advance. 
		UaThread::msleep(1000);

		while (m_stopThread == OpcUa_False && ShutDownFlag() == 0)
		{
			// Error/abort handling
			if (!std::getline(std::cin, line)) { // Get input stream
				if (ShutDownFlag() != 0)
					break;
				// if failure, check the reason
				if (std::cin.eof()) {
					std::cerr << "End of input reached. Connection to STDINtoXU might be closed." << std::endl;
				}
				else if (std::cin.fail()) {
					std::cerr << "Input stream error. Connection to STDINtoXU might be broken." << std::endl;
				}
				else {
					std::cerr << "Unknown error occurred. Connection to STDINtoXU might be broken." << std::endl;
				}
				break; // Schleife beenden
			}
			

			try {
				// XUtoSTDOUT prints empty strings -> if _STDOUT_DUMMY_TIME exceeds 
				// Catch this "empty" transfer
				if (line.empty())
					continue;

				auto temppdu = pdu_ptr_t(PDU::StringtoPDU(line, ':')); // Create shared pointer

				std::stringstream sstr;
				std::string folder_system = temppdu->getFolderName(); // Get system folder name

				//! We need to use systemfolder + signalfolder name for the nodeids of the 
				//! signal folder because there are some of the same signal names but in different systems.
				//! Otherwise we would add signals to the right signal folders but under the wrong system folder.
				sstr << folder_system << temppdu->getSignalName();
				std::string folder_signaltype = sstr.str();


				//! After the signal image has been sent, a flag is set to allow client connections
				//! The first signal which has the 5th bit set in the Origin part is the first signal
				//! which was generated by an event and not by the signal image
				if (temppdu->getOrigin() & (1 << 4) && !m_signalimage_complete)
				{
					m_signalimage_complete = true;
					std::cerr << "All signals detected. Server ready for connection.\n";
					// TODO Allow Client connections now!
				}


				switch (temppdu->getType())
				{
				case PDU::TYPE::FLOAT:
				{
					if (!this->findNode(UaNodeId(temppdu->getKKS().c_str(), namespaceindex)))
					{
						if (!this->findNode(UaNodeId(folder_system.c_str(), namespaceindex)))
						{
							//------------------ Create new system folder Object --------------------------------------
							OpcUa::FolderType* pFolderSystem = new OpcUa::FolderType(
								UaNodeId(folder_system.c_str(), namespaceindex), // NodeId
								folder_system.c_str(), // Name
								namespaceindex, // Name space index
								this); // Interface for adding nodes
							addResult = addNodeAndReference(xu_object_ref, pFolderSystem, OpcUaId_HasComponent);
							if (addResult.isNotGood())
							{   // Release reference if adding node to NodeManager failed
								pFolderSystem->releaseReference();
							}
						}
						if (!this->findNode(UaNodeId(folder_signaltype.c_str(), namespaceindex)))
						{
							//------------------ Create new signal folder Object --------------------------------------
							OpcUa::FolderType* pFolderSignal = new OpcUa::FolderType(
								UaNodeId(folder_signaltype.c_str(), namespaceindex), // NodeId
								temppdu->getSignalName().c_str(), // Name
								namespaceindex, // Name space index
								this); // Interface for adding nodes
							addResult = addNodeAndReference(UaNodeId(folder_system.c_str(), namespaceindex), pFolderSignal, OpcUaId_HasComponent);
							if (addResult.isNotGood())
							{   // Release reference if adding node to NodeManager failed
								pFolderSignal->releaseReference();
							}
						}

						bool temp_write_permission = false;
						if(m_pWrite_perm_list.find(temppdu->getKKS().c_str()) != m_pWrite_perm_list.end())
						{
							temp_write_permission = true;
						}

						//-------------------- Create new FloatPDU Object -----------------------------------
						XU::FloatPDUObjectType* pObjectFloat = new XU::FloatPDUObjectType(
							UaNodeId(temppdu->getKKS().c_str(), namespaceindex),
							temppdu->getKKS().c_str(),
							namespaceindex,
							this,
							temppdu->getFloat(),
							temppdu->getState(),
							temppdu->getTime(),
							temp_write_permission);

						addResult = addNodeAndReference(UaNodeId(folder_signaltype.c_str(), namespaceindex), pObjectFloat, OpcUaId_HasComponent); //Add to folder
						if (addResult.isNotGood())
						{   // Release reference if adding node to NodeManager failed
							pObjectFloat->releaseReference();
						}

						signal_count++;
					}
					else // PDU does already exist
					{
						XU::FloatPDUObjectType* pObjectFloat = (XU::FloatPDUObjectType*)this->findNode(UaNodeId(temppdu->getKKS().c_str(), namespaceindex));
						if (pObjectFloat == NULL)
						{
							// Exit if nullptr
							perror("findNode()");
							std::cerr << "findNode() returned nullptr" << '\n';
							exit(-1);
						}

						pObjectFloat->setValue((OpcUa_Double)temppdu->getFloat()); //Set new value

						pObjectFloat->setStates(pObjectFloat, temppdu->getState()); //Set new state

						pObjectFloat->setTimestamp(UaDateTime::fromTime_t(static_cast<time_t>(temppdu->getTime().tv_sec + TIMEZONE_MESZ))); // Set new timestamp

						if (temppdu->getKKS() == "3AHP 712MP               XQ01     ")
						{
							auto end = std::chrono::high_resolution_clock::now();

							auto now = std::chrono::system_clock::now();
							std::time_t now_time = std::chrono::system_clock::to_time_t(now);
							auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

							// Zeit formatieren und ausgeben
							std::tm* now_tm = std::localtime(&now_time);
							std::ostringstream oss;
							oss << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S");
							oss << '.' << std::setfill('0') << std::setw(3) << milliseconds.count();

							// Ausgabe
							std::cerr << oss.str() << " -> Signalaenderung veroeffentlicht (UAServer_XU)\n";
						}
					}
					break;
				}
				case PDU::TYPE::INT:
				{
					if (!this->findNode(UaNodeId(temppdu->getKKS().c_str(), namespaceindex)))
					{
						if (!this->findNode(UaNodeId(folder_system.c_str(), namespaceindex)))
						{
							//------------------ Create new folder Object --------------------------------------
							OpcUa::FolderType* pFolderSystem = new OpcUa::FolderType(
								UaNodeId(folder_system.c_str(), namespaceindex), // NodeId
								folder_system.c_str(), // Name
								namespaceindex, // Name space index
								this); // Interface for adding nodes
							addResult = addNodeAndReference(xu_object_ref, pFolderSystem, OpcUaId_HasComponent);
							if (addResult.isNotGood())
							{   // Release reference if adding node to NodeManager failed
								pFolderSystem->releaseReference();
							}
						}
						if (!this->findNode(UaNodeId(folder_signaltype.c_str(), namespaceindex)))
						{
							//------------------ Create new folder Object --------------------------------------
							OpcUa::FolderType* pFolderSignal = new OpcUa::FolderType(
								UaNodeId(folder_signaltype.c_str(), namespaceindex), // NodeId
								temppdu->getSignalName().c_str(), // Name
								namespaceindex, // Name space index
								this); // Interface for adding nodes
							addResult = addNodeAndReference(UaNodeId(folder_system.c_str(), namespaceindex), pFolderSignal, OpcUaId_HasComponent);
							if (addResult.isNotGood())
							{   // Release reference if adding node to NodeManager failed
								pFolderSignal->releaseReference();
							}
						}

						bool temp_write_permission = false;
						if (m_pWrite_perm_list.find(temppdu->getKKS().c_str()) != m_pWrite_perm_list.end())
						{
							temp_write_permission = true;
						}

						//-------------------- Create new IntPDU Object -------------------------------------
						XU::IntPDUObjectType* pObjectInt = new XU::IntPDUObjectType(
							UaNodeId(temppdu->getKKS().c_str(), namespaceindex),
							temppdu->getKKS().c_str(),
							namespaceindex,
							this,
							temppdu->getInt(),
							temppdu->getState(),
							temppdu->getTime(),
							temp_write_permission);

						addResult = addNodeAndReference(UaNodeId(folder_signaltype.c_str(), namespaceindex), pObjectInt, OpcUaId_HasComponent); //Add to folder
						if (addResult.isNotGood())
						{   // Release reference if adding node to NodeManager failed
							pObjectInt->releaseReference();
						}

						signal_count++;
					}
					else
					{
						XU::IntPDUObjectType* pObjectInt = (XU::IntPDUObjectType*)this->findNode(UaNodeId(temppdu->getKKS().c_str(), namespaceindex));
						if ((pObjectInt == NULL))
						{
							// Exit if nullptr
							perror("findNode()");
							std::cerr << "findNode() returned nullptr" << '\n';
							exit(-1);
						}
						
						pObjectInt->setValue((OpcUa_Int16)temppdu->getInt()); //Set new value
						
						pObjectInt->setStates(pObjectInt, temppdu->getState()); //Set new state

						pObjectInt->setTimestamp(UaDateTime::fromTime_t(static_cast<time_t>(temppdu->getTime().tv_sec + TIMEZONE_MESZ))); // Set new timestamp

						//if (temppdu->getKKS() == "3AHP 712MP               XQ01     ")
						//{
							/*auto end_time = std::chrono::high_resolution_clock::now();
							auto int_s = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
							std::cerr << int_s.count() << "us" << std::endl;*/
						//}
					}
					break;
				}
				case PDU::TYPE::NONE:
					// exception would have been thrown earlier
					break;
				default:
					throw std::runtime_error("Fehler bei Erstellung des Namespaces: Server wird gestoppt!");
					break;
				}
			}
			catch (std::exception e) {
				if (ShutDownFlag() == 0)
					std::cerr << "Fehler bei Umwandlung in PDU: " << e.what() << '\n';
			}
			// update signal count
			xu_object_ref->setSignals(signal_count);
		}
	}


} // End namespace for the UA information model http://yourorganisation.org/UAServer/
