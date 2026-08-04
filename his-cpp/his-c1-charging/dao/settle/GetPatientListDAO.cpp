#include "stdafx.h"
#include "GetPatientListDAO.h"
#include "GetPatientListMapper.h"

// 构建查询条件
inline std::string GetPatientListDAO::queryConditionBuilder(const GetPatientListQuery::Wrapper & query, SqlParams & params)
{
    std::ostringstream sqlCondition;
    sqlCondition << " WHERE 1=1 ";

    // 就诊ID
    if (query->encounter_id)
    {
        sqlCondition << " AND enc.id = ? ";
        SQLPARAMS_PUSH(params, "ll", uint64_t, query->encounter_id);
    }
    // 住院号 精确匹配 (对应 enc.bus_no)
    if (query->inpatient_no && !query->inpatient_no->empty())
    {
        sqlCondition << " AND enc.bus_no = ? ";
        SQLPARAMS_PUSH(params, "s", std::string, *query->inpatient_no);
    }
    // 患者姓名 模糊查询 (对应 pat.name)
    if (query->patient_name && !query->patient_name->empty())
    {
        sqlCondition << " AND pat.name LIKE ? ";
        SQLPARAMS_PUSH(params, "s", std::string, "%" + (*query->patient_name) + "%");
    }
    // 性别 (对应 pat.gender_enum)
    if (query->gender)
    {
        sqlCondition << " AND pat.gender_enum = ? ";
        SQLPARAMS_PUSH(params, "i", uint8_t, query->gender);
    }
    // 科室名称 (对应 loc_dept.name)
    if (query->dept_name && !query->dept_name->empty())
    {
        sqlCondition << " AND loc_dept.name LIKE ? ";
        SQLPARAMS_PUSH(params, "s", std::string, "%" + (*query->dept_name) + "%");
    }
    // 病区名称 (对应 loc_ward.name)
    if (query->ward_name && !query->ward_name->empty())
    {
        sqlCondition << " AND loc_ward.name LIKE ? ";
        SQLPARAMS_PUSH(params, "s", std::string, "%" + (*query->ward_name) + "%");
    }
    // 床号 (对应 loc_bed.name)
    if (query->bed_no && !query->bed_no->empty())
    {
        sqlCondition << " AND loc_bed.name LIKE ? ";
        SQLPARAMS_PUSH(params, "s", std::string, "%" + (*query->bed_no) + "%");
    }
    // 入院时间 ⚠️重要修复：参数类型由数字i改成字符串s
   /* if (query->admit_date && !query->admit_date->empty())
    {
        sqlCondition << " AND DATE(enc.start_time) = ? ";
        SQLPARAMS_PUSH(params, "i", uint64_t, *query->admit_date);
    }*/
    // 医保类型 (对应 enc.yb_class_enum)
    if (query->insurance_type)
    {
        sqlCondition << " AND enc.yb_class_enum = ? ";
        SQLPARAMS_PUSH(params, "i", uint8_t, query->insurance_type);
    }
    //【临时注释】医生姓名，待找到正确关联表再启用
    /*
    if (query->doctor_name && !query->doctor_name->empty())
    {
        sqlCondition << " AND prac.name LIKE ? ";
        SQLPARAMS_PUSH(params, "s", std::string, "%" + (*query->doctor_name) + "%");
    }
    */

    return sqlCondition.str();
}

// Count matching rows
uint64_t GetPatientListDAO::count(const GetPatientListQuery::Wrapper& query)
{
    SqlParams params;
    std::ostringstream sql;

    sql << " SELECT COUNT(DISTINCT enc.id) " // DISTINCT 解决多条JOIN造成计数虚高
        << " FROM adm_encounter enc "
        << " LEFT JOIN adm_patient pat ON enc.patient_id = pat.id "
        << " LEFT JOIN bkg_bed_reservation_ext bed ON enc.id = bed.appointment_id "
        << " LEFT JOIN adm_location loc_bed ON bed.location_id = loc_bed.id "
        << " LEFT JOIN adm_encounter_location enc_loc ON enc.id = enc_loc.encounter_id "
        << " LEFT JOIN adm_location loc_dept ON enc_loc.location_id = loc_dept.id "
        << " LEFT JOIN adm_location loc_ward ON bed.ward_location_id = loc_ward.id ";

    sql << queryConditionBuilder(query, params);

    return sqlSession->executeQueryNumerical(sql.str(), params);
}

// Paginated query - multi-table JOIN
std::list<GetPatientListDO> GetPatientListDAO::selectWithPage(const GetPatientListQuery::Wrapper& query)
{
    SqlParams params;
    std::ostringstream sql;

    sql << "SELECT DISTINCT " // DISTINCT 消除重复就诊记录
        << " enc.id AS id, "
        << " enc.bus_no AS bus_no, "
        << " pat.name AS name, "
        << " pat.gender_enum AS gender_enum, "
        << " TIMESTAMPDIFF(YEAR, pat.birth_date, CURDATE()) AS age, "
        << " loc_dept.name AS org_name, "
        << " loc_ward.name AS ward_name, "
        << " loc_bed.name AS bed_no, "
        << " enc.start_time, "
        << " DATE_FORMAT(enc.start_time, '%Y-%m-%d') AS admit_date, "
        << " enc.yb_class_enum AS insurance_type, "
        << " NULL AS doctor_name "
        << " FROM adm_encounter enc "
        << " LEFT JOIN adm_patient pat ON enc.patient_id = pat.id "
        << " LEFT JOIN bkg_bed_reservation_ext bed ON enc.id = bed.appointment_id "
        << " LEFT JOIN adm_location loc_bed ON bed.location_id = loc_bed.id "
        << " LEFT JOIN adm_encounter_location enc_loc ON enc.id = enc_loc.encounter_id "
        << " LEFT JOIN adm_location loc_dept ON enc_loc.location_id = loc_dept.id "
        << " LEFT JOIN adm_location loc_ward ON bed.ward_location_id = loc_ward.id ";

    sql << queryConditionBuilder(query, params);

    sql << " ORDER BY admit_date DESC "
        << " LIMIT " << std::to_string((int64_t)(query->pageIndex.getValue(1) - 1) * query->pageSize.getValue(10))
        << ", " << std::to_string((int64_t)query->pageSize.getValue(10));

    GetPatientListMapper mapper;
    return sqlSession->executeQuery<GetPatientListDO>(
        sql.str(), GetPatientListMapper(), params);
}



