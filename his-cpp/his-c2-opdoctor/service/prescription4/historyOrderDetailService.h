#pragma once
/*
 Copyright Zero One Star. All rights reserved.
 @Author: awei
 @Date: 2026/07/27
*/
#ifndef _HISTORY_ORDER_DETAIL_SERVICE_
#define _HISTORY_ORDER_DETAIL_SERVICE_

#include "../../domain/query/prescription4/HistoryOrderDetailQuery.h"
#include "../../domain/dto/prescription4/HistoryOrderDetailDTO.h"
#include "../../dao/prescription4/HistoryOrderDetailDAO.h"

/**
 * 历史医嘱明细 Service 层
 * 负责查询某次就诊下的所有医嘱（药品、检验检查、耗材），并转换为 DTO
 */
class HistoryOrderDetailService
{
private:
    HistoryOrderDetailDAO dao;

    /**
     * 将 DO 列表转换为 DTO 列表
     */
    oatpp::List<HistoryOrderDetailDTO::Wrapper> convertToDTOList(const std::list<PtrHistoryOrderDetailDO>& doList);

public:
    /**
     * 分页查询医嘱明细
     * @param query 查询条件（就诊ID、医嘱类型、关键词）
     * @return 分页结果 DTO
     */
    HistoryOrderDetailPageDTO::Wrapper listAll(const HistoryOrderDetailQuery::Wrapper& query);
};

#endif // !_HISTORY_ORDER_DETAIL_SERVICE_