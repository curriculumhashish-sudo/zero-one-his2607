#pragma once
#ifndef _WARD_DTO_
#define _WARD_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 病房 DTO（用于返回病房名称与唯一标识）
 */
class WardRoomDTO : public oatpp::DTO {
	DTO_INIT(WardRoomDTO, DTO);
	// 病房唯一标识
	DTO_FIELD(String, id);
	DTO_FIELD_INFO(id) { info->description = ZH_WORDS_GETTER("ward.field.roomId"); }
	// 病房名称
	DTO_FIELD(String, name);
	DTO_FIELD_INFO(name) { info->description = ZH_WORDS_GETTER("ward.field.roomName"); }
	// 可选：所属病区标识
	DTO_FIELD(String, wardAreaId);
	DTO_FIELD_INFO(wardAreaId) { info->description = ZH_WORDS_GETTER("ward.field.wardAreaId"); }
};

class WardBedDTO : public oatpp::DTO {
    DTO_INIT(WardBedDTO, DTO);
    // 病床ID
    DTO_FIELD(String, id);
    DTO_FIELD_INFO(id) { info->description = ZH_WORDS_GETTER("ward.field.bedId"); }
    // 病床名称/编号
    DTO_FIELD(String, name);
    DTO_FIELD_INFO(name) { info->description = ZH_WORDS_GETTER("ward.field.bedName"); }
    // 状态（示例：0空闲，1占用，2维护等）
    DTO_FIELD(Int32, status);
    DTO_FIELD_INFO(status) { info->description = ZH_WORDS_GETTER("ward.field.status"); }
    // 所属病房ID
    DTO_FIELD(String, wardRoomId);
    DTO_FIELD_INFO(wardRoomId) { info->description = ZH_WORDS_GETTER("ward.field.wardRoomId"); }
};
#include OATPP_CODEGEN_END(DTO)
#endif // _WARDROOM_DTO_