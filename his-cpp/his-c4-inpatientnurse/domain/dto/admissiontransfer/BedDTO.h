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
public:
    BedDTO() {
		this->id = "BED30101";
		this->name = ZH_WORDS_GETTER("business-text.sample.bed-name");
		this->parent_id = "WARD301";
        this->form_enum = "BED";
        this->status_enum = "ENABLE";
		this->organization_id = "334526752078102528";
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
		total = 1;
		pages = 1;
		rows = {BedDTO::createShared()};
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_BED_DTO_
