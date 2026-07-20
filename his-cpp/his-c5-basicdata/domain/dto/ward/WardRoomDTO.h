#pragma once
#ifndef _WARDROOM_DTO_
#define _WARDROOM_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 病房 DTO（用于返回病房名称与唯一标识）
 */
class WardRoomDTO : public oatpp::DTO {
	DTO_INIT(WardRoomDTO, DTO);
	// 病房唯一标识
	DTO_FIELD(String, id);
	DTO_FIELD_INFO(id) { info->description = ZH_WORDS_GETTER("wardroom.field.id"); }
	// 病房名称
	DTO_FIELD(String, name);
	DTO_FIELD_INFO(name) { info->description = ZH_WORDS_GETTER("wardroom.field.name"); }
	// 可选：所属病区标识
	DTO_FIELD(String, wardAreaId);
	DTO_FIELD_INFO(wardAreaId) { info->description = ZH_WORDS_GETTER("wardroom.field.wardAreaId"); }
};

#include OATPP_CODEGEN_END(DTO)
#endif // _WARDROOM_DTO_