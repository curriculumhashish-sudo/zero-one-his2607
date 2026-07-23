#pragma once
#ifndef _WARD_QUERY_
#define _WARD_QUERY_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)
class WardQuery : public oatpp::DTO
{
	DTO_INIT(WardQuery,DTO);
	DTO_FIELD(String, organizationId);
	DTO_FIELD_INFO(organizationId) {
		info->description = ZH_WORDS_GETTER("ward.field.organization-id");
		info->required = true;
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif