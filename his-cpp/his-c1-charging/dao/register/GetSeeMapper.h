#ifndef _GET_SEE_MAPPER_
#define _GET_SEE_MAPPER_

#include "Mapper.h"
#include "../../domain/do/register/GetSeeDO.h"

class GetSeeMapper : public Mapper<GetSeeDO>
{
public:
	GetSeeDO mapper(ResultSet* resultSet) const override
	{
		GetSeeDO data;
		data.setId(resultSet->getString(1));
		data.setBus_no(resultSet->getString(2));
		data.setName(resultSet->getString(3));
		data.setGender_enum(resultSet->getString(4));
		data.setPhone(resultSet->getString(5));
		data.setOrg_name(resultSet->getString(6));
		data.setDoctor(resultSet->getString(7));
		data.setCost_type(resultSet->getString(8));
		data.setAddr(resultSet->getString(9));
		data.setRegister_type(resultSet->getString(10));
		data.setCost(resultSet->getDouble(11));
		data.setTime(resultSet->getString(12));
		data.setServiceTypeId(resultSet->getString(13));
		data.setStatusEnum(resultSet->getString(14));
		return data;
	}
};



#endif