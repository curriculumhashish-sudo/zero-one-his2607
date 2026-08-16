#pragma once
#ifndef _WARD_MODIFY_DTO_H_
#define _WARD_MODIFY_DTO_H_

#include "ApiHelper.h"
using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(DTO)

class WardModifyDTO : public oatpp::DTO
{
	DTO_INIT(WardModifyDTO, DTO);
	API_DTO_FIELD(String, id, "Ward ID", true, "322955305339588608");
	API_DTO_FIELD(String, name, "Ward name", true, "Internal Medicine Ward");
	API_DTO_FIELD(String, organizationId, "Organization ID", true, "322663308812357632");
	API_DTO_FIELD(Int32, displayOrder, "Display order", false, 1);
	API_DTO_FIELD(String, pyStr, "Pinyin code", false, "nklq");
	API_DTO_FIELD(String, wbStr, "Wubi code", false, "maui");
	API_DTO_FIELD(Int32, version, "Optimistic lock version", true, 0);
};

#include OATPP_CODEGEN_END(DTO)

#endif // _WARD_MODIFY_DTO_H_
