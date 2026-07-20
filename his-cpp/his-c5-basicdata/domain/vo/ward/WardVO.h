#pragma once
#ifndef _WARD_VO_H_
#define _WARD_VO_H_

#include "ApiHelper.h"
using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(DTO)

class WardVO : public oatpp::DTO
{
	DTO_INIT(WardVO, DTO);
	API_DTO_FIELD(String, id, "Ward ID", true, "322955305339588608");
	API_DTO_FIELD(String, name, "Ward name", true, "Internal Medicine Ward");
	API_DTO_FIELD(String, busNo, "Business number", true, "LO0028");
	API_DTO_FIELD(String, statusEnum, "Status enum", true, "ACTIVE");
	API_DTO_FIELD(String, operationalEnum, "Operational enum", false, "NOT_IN_USE");
	API_DTO_FIELD(String, formEnum, "Location form enum", true, "WARD");
	API_DTO_FIELD(String, modeEnum, "Location mode enum", true, "INSTANCE");
	API_DTO_FIELD(String, typeJson, "Type JSON snapshot", false, "{}");
	API_DTO_FIELD(String, organizationId, "Organization ID", true, "322663308812357632");
	API_DTO_FIELD(Int32, displayOrder, "Display order", false, 1);
	API_DTO_FIELD(String, pyStr, "Pinyin code", false, "nklq");
	API_DTO_FIELD(String, wbStr, "Wubi code", false, "maui");
	API_DTO_FIELD(String, parentId, "Parent location ID", false, "");
	API_DTO_FIELD(Int32, version, "Optimistic lock version", true, 0);
};

class WardJsonVO : public JsonVO<WardVO::Wrapper>
{
	DTO_INIT(WardJsonVO, JsonVO<WardVO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // _WARD_VO_H_
