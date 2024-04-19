
#ifndef _XU_NodeManagerXUNamespace_H__
#define _XU_NodeManagerXUNamespace_H__

#pragma warning(push,2)
#include "uamutex.h"
#include "uabasenodes.h"
#include "nodemanagerbase.h"
#include "uaobjecttypes.h"
#pragma warning(pop)

#include "xu_identifiers.h"
#include "xu_xuobjecttype.h"
#include "xu_floatpduobjecttype.h"
#include "xu_intpduobjecttype.h"
#include "opcua_foldertype.h"
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
        NodeManagerXUNamespace(OpcUa_Boolean firesEvents, OpcUa_Int32 nHashTableSize = 1000003);
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
        


    private:
        UaStatus createDataTypeNodes();
        UaStatus createReferenceTypeNodes();
        UaStatus addAdditionalHierarchicalReferences();
        UaStatus addAdditionalNonHierarchicalReferences();

        bool m_stopThread;

        XU::XUObjectType* xu_object_ref;


        //std::unordered_map<std::string, IntPDUObjectType*> object_list_int; //List of all "IntPDU" objects
        //std::unordered_map<std::string, FloatPDUObjectType*> object_list_float; //List of all "FloatPDU" objects
        //std::unordered_map<std::string, OpcUa::FolderType*> folder_list; //List of all Folders


    protected:
        UaMutex             m_mutex;
        static OpcUa_UInt16 s_namespaceIndex;
        
#if SUPPORT_Event_Subscription_Server_Facet
        UaStatus recursiveRegisterEventSources(const UaNodeId& startingNode, std::set<UaNodeId>& setBrowsedNodes);
#endif // SUPPORT_Event_Subscription_Server_Facet
    };

} // End namespace for the UA information model http://yourorganisation.org/XU_Test_ServerConfig/

#endif // #ifndef __NodeManagerXUNamespace_H__