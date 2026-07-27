#include "DeptBatchStatusDAO.h"
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

    int idTotal = static_cast<int>(numIdList.size());
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
        std::cout << "affected rows: " << rows << std::endl;

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
        OATPP_LOGE("DeptDao", "batch update exception: %s", e.what());
        return 0;
    }
}

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
    // Prefer active_flag; also treat status_enum=ACTIVE as active for legacy inconsistent rows
    std::string sql =
        "SELECT COUNT(*) FROM adm_organization "
        "WHERE parent_id = ? AND is_deleted = 0 "
        "AND ( status_enum = 'ACTIVE')";
    SQLPARAMS_PUSH(params, "ll", int64_t, deptId);

    return session->executeQueryNumerical(sql, params) > 0;
}

std::vector<int64_t> DeptBatchStatusDAO::getChildDeptIds(int64_t deptId)
{
    SqlSession* session = new SqlSession();
    setSqlSession(session);
    setAutoRelease(true);

    SqlParams params;
    std::string sql = "SELECT id FROM adm_organization WHERE parent_id = ? AND is_deleted = 0";
    SQLPARAMS_PUSH(params, "ll", int64_t, deptId);

    // Multi-row query must use executeQuery, not executeQueryOne
    auto rows = session->executeQuery<int64_t>(sql, ChildIdMapper(), params);
    return std::vector<int64_t>(rows.begin(), rows.end());
}

bool DeptBatchStatusDAO::isDeptActive(int64_t deptId)
{
    SqlSession* session = new SqlSession();
    setSqlSession(session);
    setAutoRelease(true);

    SqlParams params;
    std::string sql =
        "SELECT CASE WHEN (status_enum = 'ACTIVE') THEN 1 ELSE 0 END "
        "FROM adm_organization WHERE id = ? AND is_deleted = 0";
    SQLPARAMS_PUSH(params, "ll", int64_t, deptId);

    ActiveFlagMapper mapper;
    return session->executeQueryOne<bool>(sql, mapper, params);
}
