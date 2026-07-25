#pragma once

#ifndef _BED_QUERY_
#define _BED_QUERY_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class BedQuery : public PageQuery
{
	DTO_INIT(BedQuery, PageQuery);

	API_DTO_FIELD_REQUIRE(String, parent_id, ZH_WORDS_GETTER("bed.field.parentid"), false);
	API_DTO_FIELD_REQUIRE(String, organization_id, ZH_WORDS_GETTER("bed.field.organizationid"), false);
	API_DTO_FIELD_REQUIRE(String, intended_admission_date, ZH_WORDS_GETTER("bed.field.intended_admission_date"), true);
	API_DTO_FIELD_REQUIRE(String, status_enum, ZH_WORDS_GETTER("bed.field.statusenum"), false);

public:
	BedQuery()
	{
		parent_id = "WARD301";
		organization_id = "334526752078102528";
		intended_admission_date = "2026-07-24";
		status_enum = "ENABLE";
	}
};


#include OATPP_CODEGEN_END(DTO)
#endif // _BED_QUERY_
