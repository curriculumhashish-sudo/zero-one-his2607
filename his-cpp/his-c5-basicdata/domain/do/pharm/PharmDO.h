#pragma once
#ifndef _PHARM_DO_
#define _PHARM_DO_

#include "../DoInclude.h"

/**
 * PharmDO - Database object for sys_pharm table (c5-pharm implementation).
 *
 * Also compatible with the c5 (mengsuibizhi) pharm table schema
 * through the typeEnum / statusEnum alias fields.
 */
class PharmDO : public BaseDO
{
    // c5-pharm fields
    MYSQL_SYNTHESIZE(uint64_t, id, Id);
    MYSQL_SYNTHESIZE(std::string, name, Name);
    MYSQL_SYNTHESIZE(std::string, type, Type);
    MYSQL_SYNTHESIZE(int, status, Status);
    MYSQL_SYNTHESIZE(int, sort, Sort);
    MYSQL_SYNTHESIZE(std::string, remark, Remark);
    MYSQL_SYNTHESIZE(std::string, create_by, Create_By);
    MYSQL_SYNTHESIZE(std::string, update_by, Update_By);
    MYSQL_SYNTHESIZE(std::string, create_time, Create_Time);
    MYSQL_SYNTHESIZE(std::string, update_time, Update_Time);

public:
    PharmDO() : BaseDO("sys_pharm")
    {
        MYSQL_ADD_FIELD_PK("id", "ull", id);
        MYSQL_ADD_FIELD("name", "s", name);
        MYSQL_ADD_FIELD("status", "i", status);
        MYSQL_ADD_FIELD("sort", "i", sort);
        MYSQL_ADD_FIELD_NULLABLE("type", "s", type, true);
        MYSQL_ADD_FIELD_NULLABLE("remark", "s", remark, true);
        MYSQL_ADD_FIELD_NULLABLE("create_by", "s", create_by, true);
        MYSQL_ADD_FIELD_NULLABLE("update_by", "s", update_by, true);
        MYSQL_ADD_FIELD_NULLABLE("create_time", "s", create_time, true);
        MYSQL_ADD_FIELD_NULLABLE("update_time", "s", update_time, true);
    }
};

typedef std::shared_ptr<PharmDO> PtrPharmDO;

#endif // !_PHARM_DO_
