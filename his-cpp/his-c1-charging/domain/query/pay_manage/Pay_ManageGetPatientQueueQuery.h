#pragma once

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class Pay_ManageGetPatientQueueQuery :public PageQuery
{
	DTO_INIT(Pay_ManageGetPatientQueueQuery, PageQuery);

	
	DTO_FIELD(String, hospitalized_number);
	DTO_FIELD_INFO(hospitalized_number) {
		info->description = ZH_WORDS_GETTER("byj.charging.pay_manage.hospitalized_number");
	}

	
	DTO_FIELD(String, name);
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("byj.charging.pay_manage.name");
	}

	DTO_FIELD(String, bed_number);
	DTO_FIELD_INFO(bed_number) {
		info->description = ZH_WORDS_GETTER("byj.charging.pay_manage.bed_number");
	}

	DTO_FIELD(UInt64, hospitalized_time);
	DTO_FIELD_INFO(hospitalized_time) {
		info->description = ZH_WORDS_GETTER("byj.charging.pay_manage.hospitalized_time");
	}

};

#include OATPP_CODEGEN_END(DTO)