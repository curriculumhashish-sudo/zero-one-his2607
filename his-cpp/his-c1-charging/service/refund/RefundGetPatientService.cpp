#include "stdafx.h"
#include "RefundGetPatientService.h"
#include "id/UuidFacade.h"
#include "../../dao/refund/RefundGetPatientDAO.h"

// 放到最后导入防止编译冲突
#include "NacosClient.h"

RefundGetPatientPageDTO::Wrapper RefundGetPatientService::listAll(const RefundGetPatientQuery::Wrapper& query)
{
	return RefundGetPatientPageDTO::Wrapper();
}
