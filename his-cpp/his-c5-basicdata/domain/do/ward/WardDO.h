
#pragma once
#include "../DoInclude.h"
class WardDO : public BaseDO
{
    MYSQL_SYNTHESIZE(std::string, id, Id);
    MYSQL_SYNTHESIZE(std::string, name, Name);
    MYSQL_SYNTHESIZE(std::string, statusEnum, StatusEnum);
    MYSQL_SYNTHESIZE(int, displayOrder, DisplayOrder);

public:
    WardDO() : BaseDO("adm_location")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("name", "s", name);
        MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
        MYSQL_ADD_FIELD("display_order", "i", displayOrder);
    }
};
typedef std::shared_ptr<WardDO> PtrWardDO;
