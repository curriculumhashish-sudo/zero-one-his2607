
#ifndef _BED_QUERY_
#define _BED_QUERY_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 根据病房获取病床列表查询
 */
class BedQuery : public PageQuery {
    DTO_INIT(BedQuery, PageQuery);
    // 所属病房ID（必填）
    DTO_FIELD(String, wardRoomId);
    DTO_FIELD_INFO(wardRoomId) { info->description = ZH_WORDS_GETTER("bed.field.wardRoomId"); }
    // 可选：状态过滤
    DTO_FIELD(Int32, status);
    DTO_FIELD_INFO(status) { info->description = ZH_WORDS_GETTER("bed.field.status"); }
};

#include OATPP_CODEGEN_END(DTO)
#endif // _BED_QUERY_