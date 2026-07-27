#include "DeptBatchStatusDao.h"
#include "stdafx.h"
#include "SqlSession.h"
#include "DeptBatchStatusMapper.h"

int DeptBatchStatusDAO::batchUpdateStatus(const std::vector<int64_t>& numIdList, const DeptBatchUpdateDTO::Wrapper& updateDto)
{
    if (numIdList.empty())
    {
        return 0;
    }

    SqlSession* session = new SqlSession();
    setSqlSession(session);
    setAutoRelease(true);

    SqlParams params;
    std::string sql = "UPDATE adm_organization SET status_enum=?,updated_by=?,updated_at=? WHERE id IN(";

    int idTotal = numIdList.size();
    for (int i = 0; i < idTotal; i++)
    {
        if (i > 0) sql += ",";
        sql += "?";
    }
    sql += ") AND is_deleted = 0";

    try
    {
        session->beginTransaction();

        std::string statusVal = *(updateDto->statusEnum.get());
        std::string updateByVal = *(updateDto->updateBy.get());
        std::string timeVal = *(updateDto->updateTime.get());

        SQLPARAMS_PUSH(params, "s", std::string, statusVal);
        SQLPARAMS_PUSH(params, "s", std::string, updateByVal);
        SQLPARAMS_PUSH(params, "s", std::string, timeVal);

        for (int64_t numId : numIdList)
        {
            SQLPARAMS_PUSH(params, "ll", int64_t, numId);
        }

        int rows = session->executeUpdate(sql, params);
        std::cout << "受影响行数：" << rows << std::endl;

        if (rows == idTotal)
        {
            session->commitTransaction();
            return rows;
        }
        else
        {
            session->rollbackTransaction();
            return 0;
        }
    }
    catch (const std::exception& e)
    {
        session->rollbackTransaction();
        OATPP_LOGE("DeptDao", "批量更新异常：%s", e.what());
        return 0;
    }
}

// ============================================================
// 新增查询方法（父子状态校验用）
// ============================================================

int64_t DeptBatchStatusDAO::getParentDeptId(int64_t deptId)
{
    SqlSession* session = new SqlSession();
    setSqlSession(session);
    setAutoRelease(true);

    SqlParams params;
    std::string sql = "SELECT parent_id FROM adm_organization WHERE id = ? AND is_deleted = 0";
    SQLPARAMS_PUSH(params, "ll", int64_t, deptId);

    ParentIdMapper mapper;
    return session->executeQueryOne<int64_t>(sql, mapper, params);
}

bool DeptBatchStatusDAO::hasActiveChildDept(int64_t deptId)
{
    SqlSession* session = new SqlSession();
    setSqlSession(session);
    setAutoRelease(true);

    SqlParams params;
    std::string sql = "SELECT COUNT(*) FROM adm_organization WHERE parent_id = ? AND active_flag = 1 AND is_deleted = 0";
    SQLPARAMS_PUSH(params, "ll", int64_t, deptId);

    ActiveCountMapper mapper;
    int count = session->executeQueryOne<int>(sql, mapper, params);
    return count > 0;
}

std::vector<int64_t> DeptBatchStatusDAO::getChildDeptIds(int64_t deptId)
{
    SqlSession* session = new SqlSession();
    setSqlSession(session);
    setAutoRelease(true);

    SqlParams params;
    std::string sql = "SELECT id FROM adm_organization WHERE parent_id = ? AND is_deleted = 0";
    SQLPARAMS_PUSH(params, "ll", int64_t, deptId);

    ChildIdListMapper mapper;
    return session->executeQueryOne<std::vector<int64_t>>(sql, mapper, params);
}

bool DeptBatchStatusDAO::isDeptActive(int64_t deptId)
{
    SqlSession* session = new SqlSession();
    setSqlSession(session);
    setAutoRelease(true);

    SqlParams params;
    std::string sql = "SELECT active_flag FROM adm_organization WHERE id = ? AND is_deleted = 0";
    SQLPARAMS_PUSH(params, "ll", int64_t, deptId);

    ActiveFlagMapper mapper;
    return session->executeQueryOne<bool>(sql, mapper, params);
}