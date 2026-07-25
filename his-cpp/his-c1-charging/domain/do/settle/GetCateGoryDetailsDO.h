#ifndef _GET_CATEGORY_DETAIL_DO_
#define _GET_CATEGORY_DETAIL_DO_
#include "../Dolnclude.h"

/**
 * 费用大类明细实体类
 */
class GetCategoryDetailDO : public BaseDO
{
    MYSQL_SYNTHESIZE(long long, encounter_id, Encounter_id);
    // 序号
    MYSQL_SYNTHESIZE(int, sort_num, Sort_num);
    // 费用大类名称
    MYSQL_SYNTHESIZE(string, charge_type_name, Charge_type_name);
    // 大类费用金额
    MYSQL_SYNTHESIZE(double, type_total_fee, Type_total_fee);

public:
    GetCategoryDetailDO() : BaseDO("GetCategoryDetail")
    {
        MYSQL_ADD_FIELD_PK("encounter_id", "ll", encounter_id);
        MYSQL_ADD_FIELD("sort_num", "i", sort_num);
        MYSQL_ADD_FIELD("charge_type_name", "s", charge_type_name);
        MYSQL_ADD_FIELD("type_total_fee", "d", type_total_fee);
    }
};

typedef std::shared_ptr<GetCategoryDetailDO> PtrGetCategoryDetailDO;

#endif // !_GET_CATEGORY_DETAIL_DO_