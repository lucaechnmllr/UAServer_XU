
#ifndef _XU_NodeManagerXUNamespace_H__
#define _XU_NodeManagerXUNamespace_H__

#pragma warning(push,0)
#include "uamutex.h"
#include "uabasenodes.h"
#include "nodemanagerbase.h"
#include "uaobjecttypes.h"
#include "opcua_foldertype.h"
#pragma warning(pop)

#include "xu_identifiers.h"
#include "xu_xuobjecttype.h"
#include "xu_floatpduobjecttype.h"
#include "xu_intpduobjecttype.h"
#include <unordered_map>
#include <vector>
#include <string>


/** Namespace for the UA information model http://yourorganisation.org/UAServer/
*/
namespace XU {

    /** Base class for NodeManager for XUNamespace information model.
     *
     *  This class is used to create the nodes defined for the namespace http://yourorganisation.org/UAServer/.
     *
     */
    class NodeManagerXUNamespace : public NodeManagerBase, public UaThread
    {
        NodeManagerXUNamespace();
    public:
        /* construction / destruction */
        NodeManagerXUNamespace(OpcUa_Boolean firesEvents, OpcUa_Int32 nHashTableSize = 10000019);
        virtual ~NodeManagerXUNamespace() {};

        //- Interface NodeManagerUaNode -------------------------------------------------------
        virtual UaStatus   afterStartUp();
        virtual UaStatus   beforeShutDown();
        //virtual XmlUaNodeFactoryNamespace* getUaNodeFactory(OpcUa_UInt16 namespaceIndex) const;
        //- Interface NodeManagerUaNode -------------------------------------------------------

        //- Interface UaThread
        void run();
        //- Interface UaThread

        
        static OpcUa_UInt16 getTypeNamespace();
        static OpcUa_UInt16 getServerMode();
        static void setServerMode(OpcUa_UInt16 mode);

        //- Read the write Permisson file
        //- Safe KKS of write permitted files to vector m_pWrite_perm_list
        void readWritePermissionFile();
        


    private:
        UaStatus createDataTypeNodes();
        UaStatus createReferenceTypeNodes();
        UaStatus addAdditionalHierarchicalReferences();
        UaStatus addAdditionalNonHierarchicalReferences();

        bool m_stopThread;
        bool m_signalimage_complete;

        XU::XUObjectType* xu_object_ref;
        std::vector<std::string> m_pWrite_perm_list;



    protected:
        UaMutex             m_mutex;
        static OpcUa_UInt16 s_namespaceIndex;
        static OpcUa_UInt16 s_server_mode;
        
        
#if SUPPORT_Event_Subscription_Server_Facet
        UaStatus recursiveRegisterEventSources(const UaNodeId& startingNode, std::set<UaNodeId>& setBrowsedNodes);
#endif // SUPPORT_Event_Subscription_Server_Facet
    };

} // End namespace for the UA information model http://yourorganisation.org/XU_Test_ServerConfig/

#endif // #ifndef __NodeManagerXUNamespace_H__