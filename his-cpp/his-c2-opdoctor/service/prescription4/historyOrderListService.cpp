/*
 Copyright Zero One Star. All rights reserved.
 @Author: awei
 @Date: 2026/07/27
*/
#include "stdafx.h"
#include "historyOrderListService.h"

// ========== 私有方法：DO → DTO 转换 ==========
oatpp::List<HistoryOrderListDTO::Wrapper> HistoryOrderListService::convertToDTOList(const std::list<PtrHistoryOrderListDO>& doList)
{
    auto dtoList = oatpp::List<HistoryOrderListDTO::Wrapper>::createShared();

    for (auto& doItem : doList) {
        auto dto = HistoryOrderListDTO::createShared();
        dto->visitId = doItem->getVisitId();
        dto->visitTime = doItem->getVisitTime();
        dto->visitType = doItem->getVisitType();
        dto->mainDiagnosis = doItem->getMainDiagnosis();
        dto->doctorName = doItem->getDoctorName();
        dto->drugCount = doItem->getDrugCount();
        dto->examCount = doItem->getExamCount();
        dto->materialCount = doItem->getMaterialCount();
        dtoList->push_back(dto);
    }

    return dtoList;
}

// ========== 分页查询 ==========
HistoryOrderListPageDTO::Wrapper HistoryOrderListService::listAll(const HistoryOrderListQuery::Wrapper& query)
{
    // 1. 调用 DAO 查询数据
    auto doList = dao.selectWithPage(query);
    auto total = dao.count(query);

    // 2. DO → DTO 转换
    auto dtoList = convertToDTOList(doList);

    // 3. 组装分页结果
    auto pageDto = HistoryOrderListPageDTO::createShared();
    pageDto->list = dtoList;
    pageDto->total = total;
    pageDto->pageNum = query->pageIndex ? query->pageIndex.getValue(1) : 1;
    pageDto->pageSize = query->pageSize ? query->pageSize.getValue(10) : 10;
    // 若 PageDTO 有 pages 字段可在此赋值，示例中未添加

    return pageDto;
}