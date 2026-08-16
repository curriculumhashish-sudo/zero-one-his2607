#ifndef _GET_TODAY_MAPPER_
#define _GET_TODAY_MAPPER_

#include "Mapper.h"
#include "../../domain/do/register/GetTodayAppointDO.h"

class GetTodayAppointMapper : public Mapper<GetTodayAppointDO>
{
public:
	GetTodayAppointDO mapper(ResultSet* resultSet) const override
	{
		GetTodayAppointDO data;
		data.setId(resultSet->getString(1));
		data.setStatus(resultSet->getString(2));
		data.setCapacity(resultSet->getInt(3));
		data.setBooked_count(resultSet->getInt(4));
		data.setRegister_type(resultSet->getString(5));
		data.setOrganization(resultSet->getString(6));
		data.setStart_time(resultSet->getString(7));
		data.setEnd_time(resultSet->getString(8));
		return data;
	}
};



#endif