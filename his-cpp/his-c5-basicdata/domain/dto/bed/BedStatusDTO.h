#pragma once
#ifndef _BED_STATUS_DTO_H_
#define _BED_STATUS_DTO_H_

#include "ApiHelper.h"
using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(DTO)

class BedStatusDTO : public oatpp::DTO
{
	DTO_INIT(BedStatusDTO, DTO);
	API_DTO_FIELD(String, id, "Bed ID", true, "322955479021522944");
	API_DTO_FIELD(String, statusEnum, "Status enum: ACTIVE or DISABLED", true, "ACTIVE");
	API_DTO_FIELD(Int32, version, "Optimistic lock version", true, 0);
};

#include OATPP_CODEGEN_END(DTO)

#endif // _BED_STATUS_DTO_H_
