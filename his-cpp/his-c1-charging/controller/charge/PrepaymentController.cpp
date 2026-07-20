/*
 Copyright Zero One Star. All rights reserved.

 @Author: Jack
 @Date: 2026/07/20

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/

#include "stdafx.h"
#include "PrepaymentController.h"

// 收预交金
PrepaymentAddVO::Wrapper PrepaymentController::execAddPrepayment(const PrepaymentAddDTO::Wrapper& dto)
{
    auto vo = PrepaymentAddVO::createShared();
    return vo;
}

// 退预交金
PrepaymentRefundVO::Wrapper PrepaymentController::execRefundPrepayment(const PrepaymentRefundDTO::Wrapper& dto)
{
    auto vo = PrepaymentRefundVO::createShared();
    return vo;
}

// 查询预交金流水
PrepaymentPageJsonVO::Wrapper PrepaymentController::execQueryPrepayment(const PrepaymentQuery::Wrapper& query)
{
    // 构建示例列表项用于API文档
    auto item = PrepaymentRecordDTO::createShared();
    item->prepaymentId = "demo-prepay-001";
    item->inpatientNo = query->inpatientNo;
    item->patientName = ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRecordDTO.patientName.value");
    item->amount = 1000.00;
    item->prepayType = "ADD";
    item->method = ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentRecordDTO.method.value");
    item->operatorName = "admin";
    item->createTime = "2026-07-20 10:30:00";

    // 构建分页示例数据
    auto rows = oatpp::List<PrepaymentRecordDTO::Wrapper>::createShared();
    rows->push_back(item);

    auto page = PrepaymentPageDTO::createShared();
    page->pageIndex = query->pageIndex;
    page->pageSize = query->pageSize;
    page->total = 1;
    page->rows = rows;
    page->calcPages();

    // 构建统一JSON响应
    auto result = PrepaymentPageJsonVO::createShared();
    result->success(page);
    return result;
}
