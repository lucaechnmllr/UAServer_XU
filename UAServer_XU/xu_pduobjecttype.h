

#ifndef __XU_PDUObjectType_H__
#define __XU_PDUObjectType_H__

#pragma warning(push,2)
#include "opcua_baseobjecttype.h"
#include "basenodes.h"
#include "opcua_foldertype.h"
#include "opcua_propertytype.h"
#pragma warning(pop)

#include <vector>
#include <string>
#include "PDU.h"


// Namespace for the UA information model http://yourorganisation.org/XU_Test_ServerConfig/
namespace XU {


    /** Generated base class for a PDUObjectType.
     *
     *  This class contains the generated base code for the object type PDUObjectType
     *  representing an OPC UA ObjectType. This class is used to create the object type and to
     *  create and represent instances of the object type in the server address space.
     *
     *  **Object members of the PDUObjectType:**
     *
     *  Browse Name | TypeDefinition | Modelling Rule | See Also
     *  ------------|----------------|----------------|---------------
     *  States      | FolderType     | Mandatory      | \ref getStates
     *
     *  **Variable members of the PDUObjectType:**
     *
     *  Browse Name | DataType | TypeDefinition | Modelling Rule | See Also
     *  ------------|----------|----------------|----------------|-------------------------------------
     *  Timestamp   | DateTime | PropertyType   | Mandatory      | \ref getTimestamp, \ref setTimestamp
     *
     */
    class PDUObjectType :
        public OpcUa::BaseObjectType
    {

    protected:
        virtual ~PDUObjectType();
    public:

        //! Data type of the PDU value
        enum TYPE {
            NONE, //!< No type defined, cannot be returned by any function
            INT,  //!< PDU contains an integer value
            FLOAT //!< PDU contains a float value
        };


        // construction / destruction
        PDUObjectType(const UaNodeId& nodeId, UaObject* pInstanceDeclarationObject, NodeManagerConfig* pNodeConfig, UaMutexRefCounted* pSharedMutex = NULL);
        PDUObjectType(const UaNodeId& nodeId, const UaString& name, OpcUa_UInt16 browseNameNameSpaceIndex, NodeManagerConfig* pNodeConfig, const PDU::timeval time, UaMutexRefCounted* pSharedMutex = NULL);
        PDUObjectType(
            UaBase::Object* pBaseNode,
            XmlUaNodeFactoryManager* pFactory,
            NodeManagerConfig* pNodeConfig,
            UaMutexRefCounted* pSharedMutex = NULL);
        static void createTypes();
        static void clearStaticMembers();

        virtual TYPE getType() const { return NONE; };
        virtual int getInt() const;
        virtual float getFloat() const;
        int getStateBitwise() const { return this->state_bitwise; }
        void setStateBitwise(int state);
        void setStates(XU::PDUObjectType* node, int state);
        void resetStates();

        virtual UaNodeId       typeDefinitionId() const;


        virtual void setTimestamp(const UaDateTime& Timestamp);
        virtual UaDateTime getTimestamp() const;

        // state_doubt defined at States
        virtual void setStates_state_doubt(OpcUa_Boolean state_doubt);
        virtual OpcUa_Boolean getStates_state_doubt() const;

        // state_hwfail defined at States
        virtual void setStates_state_hwfail(OpcUa_Boolean state_hwfail);
        virtual OpcUa_Boolean getStates_state_hwfail() const;

        // state_invalid defined at States
        virtual void setStates_state_invalid(OpcUa_Boolean state_invalid);
        virtual OpcUa_Boolean getStates_state_invalid() const;

        // state_repval defined at States
        virtual void setStates_state_repval(OpcUa_Boolean state_repval);
        virtual OpcUa_Boolean getStates_state_repval() const;

        // state_service defined at States
        virtual void setStates_state_service(OpcUa_Boolean state_service);
        virtual OpcUa_Boolean getStates_state_service() const;

        // state_simulat defined at States
        virtual void setStates_state_simulat(OpcUa_Boolean state_simulat);
        virtual OpcUa_Boolean getStates_state_simulat() const;

        // state_timeadj defined at States
        virtual void setStates_state_timeadj(OpcUa_Boolean state_timeadj);
        virtual OpcUa_Boolean getStates_state_timeadj() const;

        // state_uninit defined at States
        virtual void setStates_state_uninit(OpcUa_Boolean state_uninit);
        virtual OpcUa_Boolean getStates_state_uninit() const;

        // state_valid defined at States
        virtual void setStates_state_valid(OpcUa_Boolean state_valid);
        virtual OpcUa_Boolean getStates_state_valid() const;

        virtual OpcUa::FolderType* getStates();
        virtual const OpcUa::FolderType* getStates() const;
        virtual OpcUa::PropertyType* getTimestampNode();
        virtual const OpcUa::PropertyType* getTimestampNode() const;
        virtual OpcUa::PropertyType* getStates_state_doubtNode();
        virtual const OpcUa::PropertyType* getStates_state_doubtNode() const;
        virtual OpcUa::PropertyType* getStates_state_hwfailNode();
        virtual const OpcUa::PropertyType* getStates_state_hwfailNode() const;
        virtual OpcUa::PropertyType* getStates_state_invalidNode();
        virtual const OpcUa::PropertyType* getStates_state_invalidNode() const;
        virtual OpcUa::PropertyType* getStates_state_repvalNode();
        virtual const OpcUa::PropertyType* getStates_state_repvalNode() const;
        virtual OpcUa::PropertyType* getStates_state_serviceNode();
        virtual const OpcUa::PropertyType* getStates_state_serviceNode() const;
        virtual OpcUa::PropertyType* getStates_state_simulatNode();
        virtual const OpcUa::PropertyType* getStates_state_simulatNode() const;
        virtual OpcUa::PropertyType* getStates_state_timeadjNode();
        virtual const OpcUa::PropertyType* getStates_state_timeadjNode() const;
        virtual OpcUa::PropertyType* getStates_state_uninitNode();
        virtual const OpcUa::PropertyType* getStates_state_uninitNode() const;
        virtual OpcUa::PropertyType* getStates_state_validNode();
        virtual const OpcUa::PropertyType* getStates_state_validNode() const;

        // NodeAccessInfo management
        void useAccessInfoFromInstance(PDUObjectType* pInstance);
        virtual void setAccessInfoWithChildren(NodeAccessInfo* pNodeAccessInfo);

    protected:
        // Object nodes
        // Object States
        static OpcUa::FolderType* s_pStates;
        OpcUa::FolderType* m_pStates;

        // Variable nodes
        // Variable Timestamp
        static OpcUa::PropertyType* s_pTimestamp;
        OpcUa::PropertyType* m_pTimestamp;

        // Variable States state_doubt
        static OpcUa::PropertyType* s_pStates_state_doubt;
        OpcUa::PropertyType* m_pStates_state_doubt;
        // Variable States state_hwfail
        static OpcUa::PropertyType* s_pStates_state_hwfail;
        OpcUa::PropertyType* m_pStates_state_hwfail;
        // Variable States state_invalid
        static OpcUa::PropertyType* s_pStates_state_invalid;
        OpcUa::PropertyType* m_pStates_state_invalid;
        // Variable States state_repval
        static OpcUa::PropertyType* s_pStates_state_repval;
        OpcUa::PropertyType* m_pStates_state_repval;
        // Variable States state_service
        static OpcUa::PropertyType* s_pStates_state_service;
        OpcUa::PropertyType* m_pStates_state_service;
        // Variable States state_simulat
        static OpcUa::PropertyType* s_pStates_state_simulat;
        OpcUa::PropertyType* m_pStates_state_simulat;
        // Variable States state_timeadj
        static OpcUa::PropertyType* s_pStates_state_timeadj;
        OpcUa::PropertyType* m_pStates_state_timeadj;
        // Variable States state_uninit
        static OpcUa::PropertyType* s_pStates_state_uninit;
        OpcUa::PropertyType* m_pStates_state_uninit;
        // Variable States state_valid
        static OpcUa::PropertyType* s_pStates_state_valid;
        OpcUa::PropertyType* m_pStates_state_valid;


    private:
        void initialize(const PDU::timeval time);

    private:
        static bool s_typeNodesCreated;
        int state_bitwise;
        std::vector<std::string> m_pWrite_perm_list;
    };

} // End namespace for the UA information model http://yourorganisation.org/XU_Test_ServerConfig/

#endif // #ifndef __XUPDUObjectType_H__


