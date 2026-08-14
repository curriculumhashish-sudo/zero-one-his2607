#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: PKQ
 @Date: 2026/07/24 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 ...
*/
#ifndef _ORDERSPLITSERVICE_H_
#define _ORDERSPLITSERVICE_H_
#include "domain/dto/medicalorder_2/OrderSplitDTO.h"
#include "domain/vo/medicalorder_2/MedicalOrderVO.h"


class OrderSplitService
{
public:
    SplitResultDTO::Wrapper splitOrder(const OrderSplitDTO::Wrapper& dto);
};

#endif