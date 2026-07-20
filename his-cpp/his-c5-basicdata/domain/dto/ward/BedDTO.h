#pragma once
#ifndef _BED_DTO_
#define _BED_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 病床 DTO（名称、唯一标识、状态）
 */
class BedDTO : public oatpp::DTO {
    DTO_INIT(BedDTO, DTO);
    // 病床ID
    DTO_FIELD(String, id);
    DTO_FIELD_INFO(id) { info->description = ZH_WORDS_GETTER("bed.field.id"); }
    // 病床名称/编号
    DTO_FIELD(String, name);
    DTO_FIELD_INFO(name) { info->description = ZH_WORDS_GETTER("bed.field.name"); }
    // 状态（示例：0空闲，1占用，2维护等）
    DTO_FIELD(Int32, status);
    DTO_FIELD_INFO(status) { info->description = ZH_WORDS_GETTER("bed.field.status"); }
    // 所属病房ID
    DTO_FIELD(String, wardRoomId);
    DTO_FIELD_INFO(wardRoomId) { info->description = ZH_WORDS_GETTER("bed.field.wardRoomId"); }
};

#include OATPP_CODEGEN_END(DTO)
#endif // _BED_DTO_