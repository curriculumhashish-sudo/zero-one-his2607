

#ifndef _WARD_QUERY_
#define _WARD_QUERY_

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
    DTO_FIELD_INFO(wardAreaId) { info->description = ZH_WORDS_GETTER("ward.field.wardAreaId"); }
    // 可选名称模糊搜索
    DTO_FIELD(String, name);
    DTO_FIELD_INFO(name) { info->description = ZH_WORDS_GETTER("ward.field.name"); }
};

class WardBedQuery : public PageQuery {
    DTO_INIT(WardBedQuery, PageQuery);
    // 所属病房ID（必填）
    DTO_FIELD(String, wardRoomId);
    DTO_FIELD_INFO(wardRoomId) { info->description = ZH_WORDS_GETTER("ward.field.wardRoomId"); }
    // 可选：状态过滤
    DTO_FIELD(Int32, status);
    DTO_FIELD_INFO(status) { info->description = ZH_WORDS_GETTER("ward.field.status"); }
};
#include OATPP_CODEGEN_END(DTO)
#endif // _WARDROOM_QUERY_