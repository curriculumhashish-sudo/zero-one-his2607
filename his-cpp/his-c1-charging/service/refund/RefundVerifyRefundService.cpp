#include "stdafx.h"
#include "RefundVerifyRefundService.h"
#include "id/UuidFacade.h"
#include "../../dao/refund/RefundVerifyRefundDAO.h"

// 放到最后导入防止编译冲突
#include "NacosClient.h"


// 校验退费是否可执行
RefundVerifyRefundDTO::Wrapper RefundVerifyRefundService::verify(const RefundVerifyRefundQuery::Wrapper& query) {
	return RefundVerifyRefundDTO::Wrapper();
}