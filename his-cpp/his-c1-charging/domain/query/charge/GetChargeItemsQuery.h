#pragma once

#ifndef _GET_CHARGE_ITEMS_QUERY_
#define _GET_CHARGE_ITEMS_QUERY_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)
/*
   获取患者缴费项查询参数
*/
class GetChargeItemsQuery : public oatpp::DTO {
    DTO_INIT(GetChargeItemsQuery, DTO);

    // 就诊主键
    API_DTO_FIELD(String, encounterId,
        ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemsQuery.encounterId.description"),
        true, "337209205188464640");

    // 是否加载定价扩展字段（成本/折扣）；true 时额外关联 adm_charge_item_definition 及产品读模型
    API_DTO_FIELD(Boolean, includePriceExtension,
        ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemsQuery.includePriceExtension.description"),
        false, false);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_GET_CHARGE_ITEMS_QUERY_
