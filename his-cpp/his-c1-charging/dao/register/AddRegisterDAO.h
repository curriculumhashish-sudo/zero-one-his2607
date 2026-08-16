#ifndef __ADD_REGISTER_DAO__
#define __ADD_REGISTER_DAO__
#include"stdafx.h"
#include "BaseDAO.h"
#include "id/UuidFacade.h"
#include"domain/do/register/AddRegisterDO.h"
class DefinitionMaaper : public Mapper<DefinitionDO>
{
public:
    DefinitionDO mapper(ResultSet* resultSet) const override
    {
        DefinitionDO data;
        data.setId(resultSet->getString(1));
        data.setPrice_value(resultSet->getDouble(2));
        return data;
    }
};

class AddRegisterDAO : public BaseDAO
{
public:

    DefinitionDO getCost(const string service_id)
    {
        stringstream sql;
        sql << "SELECT id, price_value FROM adm_charge_item_definition ";
        sql << " WHERE 1=1 AND";
        sql << " instance_id=" <<"'"<<service_id<<"'";
        sql << " AND instance_table='adm_healthcare_service'";
        cout << sql.str() << endl;

       return sqlSession->executeQueryOne<DefinitionDO>(sql.str(), DefinitionMaaper());

    
    }

    int add_register(const ChargItemDO& chargItem,const PaymentDO& pay,
        const EncounterDO& encounter, const ParticipantDO& participant, const LocationDO& location, const PatientDO& patient,const SlotDO& slot, const AccountDO& account,bool is_insert_pa)
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

        if (this->insert(chargItem) != 1)
        {
            sqlSession->rollbackTransaction();
            return -1;
        }
        cout << "chargItem seccuss" << endl;

        if (this->insert(pay) != 1)
        {
            sqlSession->rollbackTransaction();
            return -1;
        }
        cout << "chargItem pay" << endl;

        sqlSession->commitTransaction();
        return 1;
    }
};


#endif