#pragma once
#ifndef _BED_VO_
#define _BED_VO_
#include "../../GlobalInclude.h"
#include "../../dto/admissiontransfer/BedDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class BedPageJsonVO : public JsonVO<BedPageDTO::Wrapper> {
    DTO_INIT(BedPageJsonVO, JsonVO<BedPageDTO::Wrapper>);
public:
	BedPageJsonVO()
	{
		data = BedPageDTO::createShared();
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_BED_VO_
