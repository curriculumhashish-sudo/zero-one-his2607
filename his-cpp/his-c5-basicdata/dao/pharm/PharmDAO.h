#ifndef _PHARMDAO_H_
#define _PHARMDAO_H_

#include "BaseDAO.h"
#include "domain/do/pharm/PharmDO.h"
#include "domain/query/pharm/PharmQuery.h"
#include <list>

class PharmDAO : public BaseDAO
{
public:
    // 分页查询库房药房列表
    std::list<PtrPharmDO> selectPage(const PharmQuery::Wrapper& query);
    // 统计符合条件的记录总数
    uint64_t count(const PharmQuery::Wrapper& query);
    // 新增库房药房
    int insert(const PharmDO& data);
    // 修改库房药房信息
    int update(const PharmDO& data);
    // 逻辑删除库房药房
    int remove(uint64_t id);
    // 更新库房药房状态
    int updateStatus(uint64_t id, int status);

private:
    // 动态构建查询条件与参数集合
    std::string queryConditionBuilder(const PharmQuery::Wrapper& query, SqlParams& params);
};

#endif // !_PHARMDAO_H_