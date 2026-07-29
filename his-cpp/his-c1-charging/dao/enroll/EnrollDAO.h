#pragma once

#include "BaseDAO.h"
#include "../../domain/do/enroll/EnrollListDO.h"
#include "../../domain/query/enroll/EnrollQuery.h"
#include "../../domain/do/enroll/EnrollDetailDO.h"

class EnrollDAO : public BaseDAO
{
private:
    // 构建列表和总数查询共用的筛选条件。
    std::string buildCondition(const EnrollQuery::Wrapper& query, SqlParams& params);

public:
    // 查询符合条件的住院登记总数。
    uint64_t count(const EnrollQuery::Wrapper& query);
    
    // 查询当前页的住院登记列表。
    std::list<EnrollListDO> selectWithPage(const EnrollQuery::Wrapper& query);

/**
* @brief 根据住院登记主键查询一条住院登记详情。
*
* @param encounterId 住院登记主键，对应 adm_encounter.id。
* @return 查询到时返回详情对象；未查询到时返回 nullptr。
*
* @details
* 该方法只负责从数据库读取原始详情数据。
* 状态文字、性别文字、年龄等展示与业务处理由 Service 层完成。
*/
    PtrEnrollDetailDO selectDetailById(const std::string& encounterId);
};