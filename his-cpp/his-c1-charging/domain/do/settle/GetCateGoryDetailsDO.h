#pragma once
#ifndef _GET_CATEGORY_DETAILS_DO_
#define _GET_CATEGORY_DETAILS_DO_

#include "../DoInclude.h"

/**
 * 费用大类明细实体类
 */
class GetCategoryDetailsDO : public BaseDO
{
    MYSQL_SYNTHESIZE(string, encounter_id, Encounter_id);
    // 序号
    MYSQL_SYNTHESIZE(int, sort_num, Sort_num);
    // 费用大类名称
    MYSQL_SYNTHESIZE(string, charge_type_name, Charge_type_name);
    // 大类费用金额
    MYSQL_SYNTHESIZE(double, type_total_fee, Type_total_fee);

public:
    GetCategoryDetailsDO() : BaseDO("GetCategoryDetails")
    {
        MYSQL_ADD_FIELD_PK("encounter_id", "s", encounter_id);
        MYSQL_ADD_FIELD("sort_num", "i", sort_num);
        MYSQL_ADD_FIELD("charge_type_name", "s", charge_type_name);
        MYSQL_ADD_FIELD("type_total_fee", "d", type_total_fee);
    }
};

typedef std::shared_ptr<GetCategoryDetailsDO> PtrGetCategoryDetailsDO;

#endif // !_GET_CATEGORY_DETAILS_DO_