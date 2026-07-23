#pragma once

#ifndef _GET_YXY_LIST_QUERY_
#define _GET_YXY_LIST_QUERY_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class GetExpenseDetailsListQuery : public PageQuery
{
	DTO_INIT(GetExpenseDetailsListQuery, PageQuery);

	DTO_FIELD(String, inpatient_no);
	DTO_FIELD_INFO(inpatient_no) {
		info->description = ZH_WORDS_GETTER("qi.charging.settle.inpatient_no");
	}

	DTO_FIELD(String, patient_name);
	DTO_FIELD_INFO(patient_name) {
		info->description = ZH_WORDS_GETTER("qi.charging.settle.patient_name");
	}

	DTO_FIELD(UInt8, encounter_status);
	DTO_FIELD_INFO(encounter_status) {
		info->description = ZH_WORDS_GETTER("qi.charging.settle.encounter_status");
	}


};

#include OATPP_CODEGEN_END(DTO)
#endif