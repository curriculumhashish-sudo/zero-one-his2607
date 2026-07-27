#pragma once
/*
 Copyright Zero One Star. All rights reserved.
 @Author: awei
 @Date: 2026/07/27
*/
#ifndef _HISTORY_ORDER_LIST_SERVICE_
#define _HISTORY_ORDER_LIST_SERVICE_

#include "../../domain/query/prescription4/HistoryOrderListQuery.h"
#include "../../domain/dto/prescription4/HistoryOrderListDTO.h"
#include "../../dao/prescription4/HistoryOrderListDAO.h"

/**
 * 历史医嘱列表（就诊记录）Service 层
 * 负责查询患者历史就诊记录，并转换为 DTO
 */
class HistoryOrderListService
{
private:
    HistoryOrderListDAO dao;

    /**
     * 将 DO 列表转换为 DTO 列表
     */
    oatpp::List<HistoryOrderListDTO::Wrapper> convertToDTOList(const std::list<PtrHistoryOrderListDO>& doList);

public:
    /**
     * 分页查询就诊记录
     * @param query 查询条件（患者ID、关键词、就诊类型）
     * @return 分页结果 DTO
     */
    HistoryOrderListPageDTO::Wrapper listAll(const HistoryOrderListQuery::Wrapper& query);
};

#endif // !_HISTORY_ORDER_LIST_SERVICE_