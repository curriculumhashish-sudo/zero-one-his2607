#pragma once
#ifndef _BED_VO_H_
#define _BED_VO_H_

#include "ApiHelper.h"
using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(DTO)

class BedVO : public oatpp::DTO
{
	DTO_INIT(BedVO, DTO);
	API_DTO_FIELD(String, id, "Bed ID", true, "322955479021522944");
	API_DTO_FIELD(String, name, "Bed name", true, "Bed 1");
	API_DTO_FIELD(String, busNo, "Business number", true, "LO0028.LO0029.LO0032");
	API_DTO_FIELD(String, statusEnum, "Status enum", true, "ACTIVE");
	API_DTO_FIELD(String, operationalEnum, "Operational enum", false, "NOT_IN_USE");
	API_DTO_FIELD(String, formEnum, "Location form enum", true, "BED");
	API_DTO_FIELD(String, modeEnum, "Location mode enum", true, "INSTANCE");
	API_DTO_FIELD(String, typeJson, "Type JSON snapshot", false, "{}");
	API_DTO_FIELD(String, organizationId, "Organization ID", true, "322663308812357632");
	API_DTO_FIELD(Int32, displayOrder, "Display order", false, 1);
	API_DTO_FIELD(String, pyStr, "Pinyin code", false, "1hc");
	API_DTO_FIELD(String, wbStr, "Wubi code", false, "1yo");
	API_DTO_FIELD(String, parentId, "Parent room ID", true, "322955414072725504");
	API_DTO_FIELD(Int32, version, "Optimistic lock version", true, 0);
};

class BedJsonVO : public JsonVO<BedVO::Wrapper>
{
	DTO_INIT(BedJsonVO, JsonVO<BedVO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // _BED_VO_H_
