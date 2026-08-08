#ifndef __ADD_REGISTER_DAO__
#define __ADD_REGISTER_DAO__
#include"stdafx.h"
#include "BaseDAO.h"
#include "id/UuidFacade.h"
#include"domain/do/register/AddRegisterDO.h"
class AddRegisterDAO : public BaseDAO
{
public:
    uint64_t update_slot(const SlotDO& slot)
    {
        ostringstream sql;
        sql << "UPDATE wkf_slot SET booked_count=booked_count+1 WHERE id=" <<"'" << slot.getId() << "';";
        cout << sql.str() << endl;;
        return sqlSession->executeUpdate(sql.str());
    }
};


#endif