#pragma once
#ifndef _ORDER_SERVICE_1_H_
#define _ORDER_SERVICE_1_H_

#include "domain/vo/order/OrderVO1.h"
#include "domain/query/order/OrderQuery1.h"
#include "domain/dto/order/OrderAddDTO1.h"
#include "domain/dto/order/OrderModifyDTO1.h"
#include "domain/dto/order/OrderDetailDTO1.h"

/**
 * 医嘱服务实现
 */
class OrderService1
{
public:
    /**
     * 分页查询患者医嘱列表
     */
    OrderDetailPageDTO1::Wrapper queryOrderList(const OrderQuery1::Wrapper& query);

    /**
     * 新增医嘱
     */
    std::string addOrder(const OrderAddDTO1::Wrapper& dto);

    /**
     * 修改医嘱
     */
    bool modifyOrder(const OrderModifyDTO1::Wrapper& dto);
};

#endif // !_ORDER_SERVICE_1_H_