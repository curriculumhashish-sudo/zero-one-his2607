#pragma once
#ifndef _WARD_MAPPER_H_
#define _WARD_MAPPER_H_
#include"../../../lib-mysql/include/Mapper.h"
class WardMapper :public Mapper<WardDO>
{
public:
	WardDO mapper(ResultSet* resultSet) const override {
		WardDO data;
		data.setId(resultSet->getString("id"));
		data.setName(resultSet->getString("name"));
		return data;
	}


};

#endif