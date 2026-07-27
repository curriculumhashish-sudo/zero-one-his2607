#include "stdafx.h"
#include "PatientDAO.h"
#include "PatientMapper.h"
#include "SqlSession.h"

// 弹窗选择患者分页列表
std::vector<PtrPatientDO> PatientDAO::selectPatList(const PatSelectPageQuery::Wrapper& query, int offset, int limit)
{
    SqlSession* session = new SqlSession();
    setSqlSession(session);
    setAutoRelease(true);

    SqlParams params;
    std::string sql = R"(
        SELECT bus_no, name, gender_enum, birth_date, phone, id_card
        FROM adm_patient
        WHERE is_deleted = 0 AND active_flag = 1
    )";

    // 关键词模糊检索
    if (query->keyword && !query->keyword->empty())
    {
        sql += " AND (name LIKE ? OR bus_no LIKE ? OR id_card LIKE ?)";
        std::string likeStr = "%" + *(query->keyword) + "%";
        SQLPARAMS_PUSH(params, "s", std::string, likeStr);
        SQLPARAMS_PUSH(params, "s", std::string, likeStr);
        SQLPARAMS_PUSH(params, "s", std::string, likeStr);
    }

    // 分页排序
    sql += " ORDER BY created_at DESC LIMIT ? OFFSET ?";
    SQLPARAMS_PUSH(params, "i", int, limit);
    SQLPARAMS_PUSH(params, "i", int, offset);

    // ? 正确调用：指定返回实体类型 + 传入自定义Mapper，框架自动处理参数与结果映射
    PtrPatientSelectMapper mapper;
    std::list<PtrPatientDO> listData = session->executeQuery<PtrPatientDO>(sql, mapper);

    // list 转 vector 返回（适配上层Service接收格式）
    std::vector<PtrPatientDO> resultList(listData.begin(), listData.end());
    return resultList;
}

// 列表总数统计
int PatientDAO::countPatList(const PatSelectPageQuery::Wrapper& query)
{
    SqlSession* session = new SqlSession();
    setSqlSession(session);
    setAutoRelease(true);

    SqlParams params;
    std::string sql = R"(
        SELECT COUNT(*)
        FROM adm_patient
        WHERE is_deleted = 0 AND active_flag = 1
    )";

    if (query->keyword && !query->keyword->empty())
    {
        sql += " AND (name LIKE ? OR bus_no LIKE ? OR id_card LIKE ?)";
        std::string likeStr = "%" + *(query->keyword) + "%";
        SQLPARAMS_PUSH(params, "s", std::string, likeStr);
        SQLPARAMS_PUSH(params, "s", std::string, likeStr);
        SQLPARAMS_PUSH(params, "s", std::string, likeStr);
    }

    // 总数查询绑定计数Mapper
    PatientCountMapper countMapper;
    std::list<int> countList = session->executeQuery<int>(sql, countMapper);

    if (!countList.empty())
    {
        return countList.front();
    }
    return 0;
}
