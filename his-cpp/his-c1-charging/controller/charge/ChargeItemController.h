#pragma once
#ifndef _CHARGE_ITEM_CONTROLLER_
#define _CHARGE_ITEM_CONTROLLER_
#include "domain/vo/BaseJsonVO.h"
#include "domain/query/charge/GetChargeItemsQuery.h"
#include "domain/dto/charge/GetChargeItemDTO.h"
#include "domain/vo/charge/GetChargeItemVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("lhfc.charging.charge.tags.t2")
/*
   缴费项控制器
   负责获取患者就诊下的待收费明细列表
*/
class ChargeItemController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(ChargeItemController);
public:
    // 获取患者缴费项
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItems.summary"),
        getChargeItems,
        GetChargeItemsQuery,
        GetChargeItemVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET, "/charging/charge/get-charge-items", getChargeItems,
        GetChargeItemsQuery, execGetChargeItems(query)
    );

private:
    GetChargeItemVO::Wrapper execGetChargeItems(const GetChargeItemsQuery::Wrapper& query);
};
#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)
#endif // !_CHARGE_ITEM_CONTROLLER_
