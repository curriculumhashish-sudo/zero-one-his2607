/*
 Copyright Zero One Star. All rights reserved.
 @Author: awei
 @Date: 2026/07/27
*/
#include "stdafx.h"
#include "HistoryOrderDetailService.h"

// ========== 私有方法：DO → DTO 转换 ==========
oatpp::List<HistoryOrderDetailDTO::Wrapper> HistoryOrderDetailService::convertToDTOList(const std::list<PtrHistoryOrderDetailDO>& doList)
{
    auto dtoList = oatpp::List<HistoryOrderDetailDTO::Wrapper>::createShared();

    for (auto& doItem : doList) {
        auto dto = HistoryOrderDetailDTO::createShared();
        dto->orderId = doItem->getOrderId();
        dto->orderType = doItem->getOrderType();
        dto->orderName = doItem->getOrderName();
        dto->quantity = doItem->getQuantity();
        dto->executedCount = doItem->getExecutedCount();
        dto->sampleType = doItem->getSampleType();
        dto->executeHospital = doItem->getExecuteHospital();
        dto->createTime = doItem->getCreateTime();
        dto->diagnosis = doItem->getDiagnosis();
        dto->doctorName = doItem->getDoctorName();
        dto->isSkinTest = doItem->getIsSkinTest();
        dtoList->push_back(dto);
    }

    return dtoList;
}

// ========== 分页查询 ==========
HistoryOrderDetailPageDTO::Wrapper HistoryOrderDetailService::listAll(const HistoryOrderDetailQuery::Wrapper& query)
{
    // 1. 查询数据
    auto doList = dao.selectWithPage(query);
    auto total = dao.count(query);

    // 2. DO → DTO
    auto dtoList = convertToDTOList(doList);

    // 3. 组装分页结果
    auto pageDto = HistoryOrderDetailPageDTO::createShared();
    pageDto->list = dtoList;
    pageDto->total = total;
    pageDto->pageNum = query->pageIndex ? query->pageIndex.getValue(1) : 1;
    pageDto->pageSize = query->pageSize ? query->pageSize.getValue(10) : 10;
    // 若 PageDTO 有 pages 字段可在此赋值

    return pageDto;
}