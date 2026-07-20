#ifndef _BED_VO_
#define _BED_VO_

#include "../../GlobalInclude.h"
#include "../../dto/ward/BedDTO.h"
#include "domain/vo/JsonVO.h"
#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 病床单条VO
 */
class BedJsonVO : public JsonVO<BedDTO::Wrapper> {
    DTO_INIT(BedJsonVO, JsonVO<BedDTO::Wrapper>);
};

/**
 * 病床列表VO
 */
class BedListJsonVO : public JsonVO< List< BedDTO::Wrapper > > {
    DTO_INIT(BedListJsonVO, JsonVO< List< BedDTO::Wrapper > >);
};

#include OATPP_CODEGEN_END(DTO)
#endif // _BED_VO_