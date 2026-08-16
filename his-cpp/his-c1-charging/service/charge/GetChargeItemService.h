#pragma once

#ifndef _GET_CHARGE_ITEM_SERVICE_H_
#define _GET_CHARGE_ITEM_SERVICE_H_

#include "domain/vo/charge/GetChargeItemVO.h"
#include "domain/query/charge/GetChargeItemsQuery.h"
#include "domain/dto/charge/GetChargeItemDTO.h"

/**
 * 患者缴费项查询服务（接口2）
 */
class GetChargeItemService
{
public:
    /**
     * 按就诊ID查询待收费明细列表
     * @param query 包含 encounterId 和 includePriceExtension
     * @return 缴费项 DTO，含 encounterId 和 items 子列表
     */
    GetChargeItemDTO::Wrapper listAll(const GetChargeItemsQuery::Wrapper& query);
};

#endif // !_GET_CHARGE_ITEM_SERVICE_H_
