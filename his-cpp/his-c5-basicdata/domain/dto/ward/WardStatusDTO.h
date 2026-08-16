#pragma once
#ifndef _WARD_STATUS_DTO_H_
#define _WARD_STATUS_DTO_H_

#include "ApiHelper.h"
using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(DTO)

class WardStatusDTO : public oatpp::DTO
{
	DTO_INIT(WardStatusDTO, DTO);
	API_DTO_FIELD(String, id, "Ward ID", true, "322955305339588608");
	API_DTO_FIELD(String, statusEnum, "Status enum: ACTIVE or DISABLED", true, "ACTIVE");
	API_DTO_FIELD(Int32, version, "Optimistic lock version", true, 0);
};

#include OATPP_CODEGEN_END(DTO)

#endif // _WARD_STATUS_DTO_H_
