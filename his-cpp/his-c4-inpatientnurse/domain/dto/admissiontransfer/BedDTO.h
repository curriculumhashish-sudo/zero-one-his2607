#pragma once
#ifndef _BED_DTO_
#define _BED_DTO_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// Bed display DTO (maps adm_location)
class BedDTO : public oatpp::DTO
{
    DTO_INIT(BedDTO, DTO);

    API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("bed.field.id"),true);
    API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("bed.field.name"),true);
    API_DTO_FIELD_REQUIRE(String, parent_id, ZH_WORDS_GETTER("bed.field.parentid"),true);
    API_DTO_FIELD_REQUIRE(String, form_enum, ZH_WORDS_GETTER("bed.field.formenum"),true);
    API_DTO_FIELD_REQUIRE(String, status_enum, ZH_WORDS_GETTER("bed.field.statusenum"),true);
    API_DTO_FIELD_REQUIRE(String, organization_id, ZH_WORDS_GETTER("bed.field.organizationid"),true);
	API_DTO_FIELD_REQUIRE(String, operational_enum,
		ZH_WORDS_GETTER("bed.field.operationalenum"), true);
	API_DTO_FIELD_REQUIRE(Int64, version,
		ZH_WORDS_GETTER("bed.field.version"), true);
	API_DTO_FIELD_REQUIRE(Int32, display_order,
		ZH_WORDS_GETTER("bed.field.displayorder"), true);
	API_DTO_FIELD_REQUIRE(String, house_id,
		ZH_WORDS_GETTER("bed.field.houseid"), true);
	API_DTO_FIELD_REQUIRE(String, house_name,
		ZH_WORDS_GETTER("bed.field.housename"), true);
	API_DTO_FIELD_REQUIRE(String, ward_id,
		ZH_WORDS_GETTER("bed.field.wardid"), true);
	API_DTO_FIELD_REQUIRE(String, ward_name,
		ZH_WORDS_GETTER("bed.field.wardname"), true);
public:
	BedDTO()
	{
		version = static_cast<v_int64>(0);
		display_order = 0;
	}
};

// Bed page DTO
class BedPageDTO : public PageDTO<BedDTO::Wrapper>
{
    DTO_INIT(BedPageDTO, PageDTO<BedDTO::Wrapper>);
public:
	BedPageDTO()
	{
		pageIndex = 1;
		pageSize = 10;
		total = static_cast<v_int64>(0);
		pages = static_cast<v_int64>(0);
		rows = List<BedDTO::Wrapper>::createShared();
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_BED_DTO_
