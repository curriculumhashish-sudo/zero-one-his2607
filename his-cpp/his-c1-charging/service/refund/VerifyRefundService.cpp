#include "stdafx.h"
#include "VerifyRefundService.h"
#include "id/UuidFacade.h"
#include "../../dao/refund/VerifyRefundDAO.h"

// 放到最后导入防止编译冲突
#include "NacosClient.h"


// 校验退费是否可执行
VerifyRefundDTO::Wrapper VerifyRefundService::verify(const VerifyRefundQuery::Wrapper& query) {
	return VerifyRefundDTO::Wrapper();
}