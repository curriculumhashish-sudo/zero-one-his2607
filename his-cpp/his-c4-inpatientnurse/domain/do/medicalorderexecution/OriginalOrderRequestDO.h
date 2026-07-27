#pragma once

#ifndef _ORIGINAL_ORDER_REQUEST_DO_
#define _ORIGINAL_ORDER_REQUEST_DO_

#include "../DoInclude.h"

// 药品医嘱和诊疗医嘱共用的原医嘱查询投影对象
class OriginalOrderRequestDO
{
	MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
	MYSQL_SYNTHESIZE(string, patientId, PatientId);
	MYSQL_SYNTHESIZE(string, categoryEnum, CategoryEnum);

};

typedef std::shared_ptr<OriginalOrderRequestDO> PtrOriginalOrderRequestDO;

#endif // !_ORIGINAL_ORDER_REQUEST_DO_
