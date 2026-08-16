#include "stdafx.h"
#include "DiagnosisDAO.h"
#include "DiagnosisMapper.h"




DiagnosisDO DiagnosisDAO::selectById(std::string id)
{
    string sql = "SELECT id, encounter_id, condition_id, primary_flag, `rank`, description, diag_type_code, condition_code, tcm_syndrome_group, tcm_flag FROM adm_encounter_diagnosis WHERE `id`=?";
    return sqlSession->executeQueryOne<DiagnosisDO>(sql, DiagnosisMapper(), "%s", id);
}
