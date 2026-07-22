#ifndef _PHARMDO_H_
#define _PHARMDO_H_
#include "../DoInclude.h"

class PharmDO : public BaseDO
{
    // 字段声明：一键生成智能指针成员 + getter/setter
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
        // 主键字段
        MYSQL_ADD_FIELD_PK("id", "ull", id);
        // 非空普通字段
        MYSQL_ADD_FIELD("name", "s", name);
        MYSQL_ADD_FIELD("status", "i", status);
        MYSQL_ADD_FIELD("sort", "i", sort);
        // 可空字段
        MYSQL_ADD_FIELD_NULLABLE("type", "s", type, true);
        MYSQL_ADD_FIELD_NULLABLE("remark", "s", remark, true);
        MYSQL_ADD_FIELD_NULLABLE("create_by", "s", create_by, true);
        MYSQL_ADD_FIELD_NULLABLE("update_by", "s", update_by, true);
        MYSQL_ADD_FIELD_NULLABLE("create_time", "s", create_time, true);
        MYSQL_ADD_FIELD_NULLABLE("update_time", "s", update_time, true);
    }
};

// 智能指针别名，全链路统一使用
typedef std::shared_ptr<PharmDO> PtrPharmDO;

#endif // !_PHARMDO_H_