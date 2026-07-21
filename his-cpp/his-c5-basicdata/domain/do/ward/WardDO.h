
#pragma once
#include "../DoInclude.h"
class WardDO : public BaseDO
{
    MYSQL_SYNTHESIZE(std::string , id , Id );
    MYSQL_SYNTHESIZE(std::string , name ,Name);

public:
    WardDO() : BaseDO("adm_location")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id );
        MYSQL_ADD_FIELD("name", "s", name );
    }
};
typedef std::shared_ptr<WardDO> PtrWardDO;
