#include "DeptBatchStatusDao.h"
#include "stdafx.h"
#include "SqlSession.h"

int DeptBatchStatusDAO::batchUpdateStatus(const DeptBatchUpdateDTO::Wrapper& updateDto)
{
    if (!updateDto->idList || updateDto->idList->empty())
    {
        return 0;
    }

    SqlSession* session = new SqlSession();
    setSqlSession(session);
    setAutoRelease(true);

    SqlParams params;
    std::string sql = "UPDATE adm_organization SET status_enum=?,active_flag=?,updated_by=?,updated_at=?,version=version+1 WHERE id IN(";

    int idTotal = updateDto->idList->size();
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
        int flagNum = updateDto->activeFlag.get() ? 1 : 0;
        std::string updateByVal = *(updateDto->updateBy.get());
        std::string timeVal = *(updateDto->updateTime.get());

        SQLPARAMS_PUSH(params, "s", std::string, statusVal);
        SQLPARAMS_PUSH(params, "i", int, flagNum);
        SQLPARAMS_PUSH(params, "s", std::string, updateByVal);
        SQLPARAMS_PUSH(params, "s", std::string, timeVal);

        // ✅ 修复：使用 "ll" 类型，直接传递 int64_t
        for (int64_t id : *(updateDto->idList))
        {
            SQLPARAMS_PUSH(params, "ll", int64_t, id);
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