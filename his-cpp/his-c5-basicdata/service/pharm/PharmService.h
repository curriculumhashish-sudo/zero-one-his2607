#ifndef _PHARMSERVICE_H_
#define _PHARMSERVICE_H_

#include "domain/query/pharm/PharmQuery.h"
#include "domain/dto/pharm/PharmAddDTO.h"
#include "domain/dto/pharm/PharmModifyDTO.h"
#include "domain/vo/pharm/PharmPageVO.h"
#include "domain/GlobalInclude.h"
#include "domain/vo/BaseJsonVO.h"

class PharmService
{
public:
    // 分页查询库房药房列表
    PharmPageJsonVO::Wrapper getPage(const PharmQuery::Wrapper& query);
    // 新增库房药房
    StringJsonVO::Wrapper add(const PharmAddDTO::Wrapper& dto);
    // 修改库房药房信息
    StringJsonVO::Wrapper modify(const PharmModifyDTO::Wrapper& dto);
    // 逻辑删除库房药房
    StringJsonVO::Wrapper remove(uint64_t id);
    // 更新库房药房状态
    StringJsonVO::Wrapper updateStatus(uint64_t id, int status);
};

#endif // !_PHARMSERVICE_H_