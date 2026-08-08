#ifndef __ADD_REGISTER_DAO__
#define __ADD_REGISTER_DAO__
#include"stdafx.h"
#include "BaseDAO.h"
#include "id/UuidFacade.h"
#include"domain/do/register/AddRegisterDO.h"
class AddRegisterDAO : public BaseDAO
{
public:
    int add_register(const EncounterDO& encounter, const ParticipantDO& participant, const LocationDO& location, const PatientDO& patient,const SlotDO& slot, const AccountDO& account,bool is_insert_pa)
    {   
        sqlSession->beginTransaction();

        if (this->insert(encounter) != 1) 
        {
            sqlSession->rollbackTransaction();
            return -1; 
        }
        cout << "encounter seccuss" << endl;
        if (this->insert(participant) != 1)
        {
            sqlSession->rollbackTransaction();
            return -1;
        }
        cout << "participant seccuss" << endl;
        if (this->insert(location) != 1)
        {
            sqlSession->rollbackTransaction();
            return -1;
        }
        cout << "location seccuss" << endl;

        if (is_insert_pa) {
            if (this->insert(patient) != 1)
            {
                sqlSession->rollbackTransaction();
                return -1;
            }
            cout << "patient seccuss" << endl;
        }
        ostringstream sql;
        sql << "UPDATE wkf_slot SET booked_count=booked_count+1 WHERE id=" <<"'" << slot.getId() << "';";
        cout << sql.str() << endl;;
        if (sqlSession->executeUpdate(sql.str()) < 0) { 
            sqlSession->rollbackTransaction();
            return -1; 
        }
        cout << "slot seccuss" << endl;
        if (this->insert(account) != 1)
        {
            sqlSession->rollbackTransaction();
            return -1;
        }
        cout << "account seccuss" << endl;
        sqlSession->commitTransaction();
        return 1;
    }
};


#endif