#pragma once
#ifndef _ENTERORDERSDAO_H_
#define _ENTERORDERSDAO_H_
#include "BaseDAO.h"
#include "../../domain/do/enter-orders/EnterOrdersDO.h"
#include "../../domain/query/enter-orders/EnterOrdersQuery.h"

//EnterOrders
//enter-orders
//ENTERORDERS

/**
 * 示例表数据库操作实现
 */

class EnterOrdersDAO : public BaseDAO
{
private:
	inline std::string queryById(const EnterOrdersQueryById::Wrapper& query, SqlParams& params);
public:
	// 统计数据条数
	uint64_t count(const EnterOrdersQueryById::Wrapper& query);
	// 分页查询数据
	std::list<EnterOrdersQueryDO> selectWithPage(const EnterOrdersQueryById::Wrapper& query);
	//// 批量双表插入入口
	//int insertBatchTwoTable(const std::list<EnterOrdersAddDO>& dos, int batchSize);

};
#endif // !

//MedicalRecordBriefDAO
