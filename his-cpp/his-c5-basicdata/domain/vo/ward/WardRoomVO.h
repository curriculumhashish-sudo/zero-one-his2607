#ifndef _WARDROOM_VO_
#define _WARDROOM_VO_

#include "../../GlobalInclude.h"
#include "../../dto/ward/WardRoomDTO.h"

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
class WardRoomListJsonVO : public JsonVO<ListDTO<WardRoomDTO::Wrapper>::Wrapper> {
    DTO_INIT(WardRoomListJsonVO, JsonVO<ListDTO<WardRoomDTO::Wrapper>::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // _WARDROOM_VO_