

#ifndef _WARDROOM_QUERY_
#define _WARDROOM_QUERY_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 根据病区获取病房列表的查询参数
 */
class WardRoomQuery : public PageQuery {
    DTO_INIT(WardRoomQuery, PageQuery);
    // 病区ID（用于按病区过滤）
    DTO_FIELD(String, wardAreaId);
    DTO_FIELD_INFO(wardAreaId) { info->description = ZH_WORDS_GETTER("wardroom.field.wardAreaId"); }
    // 可选名称模糊搜索
    DTO_FIELD(String, name);
    DTO_FIELD_INFO(name) { info->description = ZH_WORDS_GETTER("wardroom.field.name"); }
};

#include OATPP_CODEGEN_END(DTO)
#endif // _WARDROOM_QUERY_