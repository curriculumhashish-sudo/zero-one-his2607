#include "GetCategoryDetailsDAO.h"
#include "GetCategoryDetailsMapper.h"

static std::string buildCondition(const GetCategoryDetailsQuery::Wrapper& query, SqlParams& params)
{
    std::ostringstream oss;
    oss << " WHERE fee.is_deleted = 0 ";
    // 先判断智能指针有效，再访问内部数据
    if (query->encounter_id && !query->encounter_id->empty())
    {
        oss << " AND fee.encounter_id = ? ";
        // oatpp::String -> std::string 标准转换方式
        std::string encId = std::string(query->encounter_id->c_str());
        //params.push_back(SqlParam(encId));
    }
    return oss.str();
}

uint64_t GetCategoryDetailsDAO::count(const GetCategoryDetailsQuery::Wrapper& query)
{
    SqlParams params;
    std::string sql = "SELECT COUNT(*) FROM yb_inpatient_fee_upload fee " + buildCondition(query, params);
    return sqlSession->executeQueryNumerical(sql, params);
}

std::list<GetCategoryDetailsDO> GetCategoryDetailsDAO::selectWithPage(const GetCategoryDetailsQuery::Wrapper& query)
{
    SqlParams params;
    std::ostringstream sql;
    sql << " SELECT "
        << " ROW_NUMBER() OVER (ORDER BY fee.created_at ASC) AS sort_num, "
        << " fee.med_chrgitm_type AS charge_type_name, "
        << " fee.det_item_fee_sumamt AS type_total_fee "
        << " FROM yb_inpatient_fee_upload fee ";
    sql << buildCondition(query, params);
    sql << " ORDER BY fee.created_at ASC ";

    GetCategoryDetailsMapper mapper;
    return sqlSession->executeQuery<GetCategoryDetailsDO>(sql.str(), mapper, params);
}