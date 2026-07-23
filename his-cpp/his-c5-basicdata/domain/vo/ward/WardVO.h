#ifndef _WARD_VO_
#define _WARD_VO_

#include "../../GlobalInclude.h"
#include "../../dto/ward/WardDTO.h"
#include "domain/vo/JsonVO.h"
#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 单条病房VO
 */
class WardRoomJsonVO : public JsonVO<WardRoomDTO::Wrapper> {
    DTO_INIT(WardRoomJsonVO, JsonVO<WardRoomDTO::Wrapper>);
};

/**
 * 病房列表VO（用于返回多条病房简洁信息）
 */
class WardRoomListJsonVO : public JsonVO< List< WardRoomDTO::Wrapper > > {
    DTO_INIT(WardRoomListJsonVO, JsonVO< List< WardRoomDTO::Wrapper > >);
};
class WardBedJsonVO : public JsonVO<WardBedDTO::Wrapper> {
    DTO_INIT(WardBedJsonVO, JsonVO<WardBedDTO::Wrapper>);
};

/**
 * 病床列表VO
 */
class WardBedListJsonVO : public JsonVO< List< WardBedDTO::Wrapper > > {
    DTO_INIT(WardBedListJsonVO, JsonVO< List< WardBedDTO::Wrapper > >);
};
#include OATPP_CODEGEN_END(DTO)
#endif // _WARDROOM_VO_