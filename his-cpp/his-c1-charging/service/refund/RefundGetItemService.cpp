#include "stdafx.h"
#include "RefundGetItemService.h"
#include "id/UuidFacade.h"
#include "../../dao/refund/RefundGetItemDAO.h"

// 放到最后导入防止编译冲突
#include "NacosClient.h"

// listAll — 根据就诊ID查询退费项目明细
// 参数: 前端传入的查询条件（encounterId + detailViewCode）
// 返回: RefundGetItemDTO 包含 encounterId 和嵌套的 paymentGroups 结构
//   paymentGroups[].refundingLines — chargeItemStatusEnum = "REFUNDING" 的行
//   paymentGroups[].preservedLines  — chargeItemStatusEnum = "PRESERVED" 的行
RefundGetItemDTO::Wrapper RefundGetItemService::listAll(const RefundGetItemQuery::Wrapper& query) {
	return RefundGetItemDTO::Wrapper();
}