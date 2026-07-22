#include "stdafx.h"
#include "GetItemService.h"
#include "id/UuidFacade.h"
#include "../../dao/refund/GetItemDAO.h"

// 放到最后导入防止编译冲突
#include "NacosClient.h"

// listAll — 根据就诊ID查询退费项目明细
// 参数: 前端传入的查询条件（encounterId + detailViewCode）
// 返回: GetItemDTO 包含 encounterId 和嵌套的 paymentGroups 结构
//   paymentGroups[].refundingLines — chargeItemStatusEnum = "REFUNDING" 的行
//   paymentGroups[].preservedLines  — chargeItemStatusEnum = "PRESERVED" 的行
GetItemDTO::Wrapper GetItemService::listAll(const GetItemQuery::Wrapper& query) {
	return GetItemDTO::Wrapper();
}