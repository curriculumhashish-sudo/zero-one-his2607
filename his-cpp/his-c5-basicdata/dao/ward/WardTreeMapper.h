#pragma once 
#ifndef _WARDTREE_MAPPER_H_
#define _WARDTREE_MAPPER_H_
#include"Mapper.h"
#include"domain/do/ward/WardTreeDO.h"
class WardTreeMapper :public  Mapper<WardTreeDO> {
public:
	WardTreeDO mapper(ResultSet* resultSet) const override {
		WardTreeDO temp;
		temp.setId(resultSet->getString("id"));
		temp.setName(resultSet->getString("name"));
		temp.setFormEnum(resultSet->getString("form_enum"));
		temp.setParentId(resultSet->getString("parent_id"));
		temp.setStatusEnum(resultSet->getString("status_enum"));
		temp.setOperationalEnum(resultSet->getString("operational_enum"));
		temp.setDisplayOrder(resultSet->getInt("display_order"));
		return temp;
	}

};


#endif