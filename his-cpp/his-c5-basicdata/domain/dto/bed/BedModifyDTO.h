#pragma once
#ifndef _BED_MODIFY_DTO_H_
#define _BED_MODIFY_DTO_H_

#include "ApiHelper.h"
using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(DTO)

class BedModifyDTO : public oatpp::DTO
{
	DTO_INIT(BedModifyDTO, DTO);
	API_DTO_FIELD(String, id, "Bed ID", true, "322955479021522944");
	API_DTO_FIELD(String, name, "Bed name", true, "Bed 1");
	API_DTO_FIELD(String, parentId, "Parent room ID", true, "322955414072725504");
	API_DTO_FIELD(String, organizationId, "Organization ID", true, "322663308812357632");
	API_DTO_FIELD(Int32, displayOrder, "Display order", false, 1);
	API_DTO_FIELD(String, typeJson, "Bed type JSON snapshot", false, "{}");
	API_DTO_FIELD(Int32, version, "Optimistic lock version", true, 0);
};

#include OATPP_CODEGEN_END(DTO)

#endif // _BED_MODIFY_DTO_H_
