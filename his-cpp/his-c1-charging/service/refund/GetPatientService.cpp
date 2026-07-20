#include "stdafx.h"
#include "GetPatientService.h"
#include "id/UuidFacade.h"
#include "../../dao/refund/GetPatientDAO.h"

// 放到最后导入防止编译冲突
#include "NacosClient.h"

GetPatientPageDTO::Wrapper GetPatientService::listAll(const GetPatientQuery::Wrapper& query)
{
	return GetPatientPageDTO::Wrapper();
}
