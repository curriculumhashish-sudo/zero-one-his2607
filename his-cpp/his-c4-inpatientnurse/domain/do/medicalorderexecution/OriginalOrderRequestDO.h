#pragma once

#ifndef _ORIGINAL_ORDER_REQUEST_DO_
#define _ORIGINAL_ORDER_REQUEST_DO_

#include "../DoInclude.h"

// 药品医嘱和诊疗医嘱共用的原医嘱查询投影对象
class OriginalOrderRequestDO : public BaseDO
{
	MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
	MYSQL_SYNTHESIZE(string, patientId, PatientId);
	MYSQL_SYNTHESIZE(string, categoryEnum, CategoryEnum);

public:
	OriginalOrderRequestDO() : BaseDO("original_order_request")
	{
		MYSQL_ADD_FIELD_NULLABLE("encounter_id", "s", encounterId, false);
		MYSQL_ADD_FIELD_NULLABLE("patient_id", "s", patientId, false);
		MYSQL_ADD_FIELD_NULLABLE("category_enum", "s", categoryEnum, false);
	}
};

typedef std::shared_ptr<OriginalOrderRequestDO> PtrOriginalOrderRequestDO;

#endif // !_ORIGINAL_ORDER_REQUEST_DO_
