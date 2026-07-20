#include<iostream>
#include"Mapper.h"
//class WardDO : public BaseDO
//    id : string
//    name : string
//
//    constructor :
//table = adm_location
//id 对应数据库 id
//name 对应数据库 name
class WardMapper : public Mapper<WardDO>
{
public:
    WardDO mapper(ResultSet* resultSet) const override
    {
        WardDO data;
        data.setId(resultSet->getString("id"));
        data.setName(resultSet->getString("name"));
        return data;
    }
};

