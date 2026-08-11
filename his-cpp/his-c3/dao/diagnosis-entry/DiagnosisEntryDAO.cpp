#include "stdafx.h"
#include "DiagnosisEntryDAO.h"
#include "DiagnosisEntryMapper.h"
//DiagnosisEntry
//diagnosis-entry
//DIAGNOSISENTRY
//std::string DiagnosisEntryDAO::queryById(const DiagnosisEntryQueryById::Wrapper& query, SqlParams& params)
//{
//	stringstream sqlCondition;
//	sqlCondition << " WHERE 1=1";
//	if (query->tenant_id) {
//
//		sqlCondition << " AND `ed.title` LIKE CONCAT('%',?,'%')";
//		SQLPARAMS_PUSH(params, "s", std::string, query->tenant_id.getValue(""));
//	}
//	if (query->patient_id) {
//
//		sqlCondition << " AND `ed.title` LIKE CONCAT('%',?,'%')";
//		SQLPARAMS_PUSH(params, "s", std::string, query->patient_id.getValue(""));
//	}
//	return sqlCondition.str();
//}

//uint64_t MedicalRecordDAO::count(const MedicalRecordQueryByTitle::Wrapper& query)
//{
//	SqlParams params;
//	string sql = "SELECT COUNT(*) FROM emr_document ";
//	// 构建查询条件
//	sql += queryByTitle(query, params);
//	// 执行查询
//	return sqlSession->executeQueryNumerical(sql, params);
//}
//
//std::list<MedicalRecordByTitleDO> MedicalRecordDAO::selectWithPage(const MedicalRecordQueryByTitle::Wrapper& query)
//{
//	SqlParams params;
//	string sql = R"(
//	SELECT ed.id, edt.name AS document_type_name, ed.title, ed.status, ed.updated_at
//	FROM emr_document ed
//	LEFT JOIN emr_document_type edt ON ed.document_type_code = edt.code
//	)";
//	// 构建查询条件
//	sql += queryByTitle(query, params);
//	//// 构建排序语句
//	//sql += " ORDER BY IFNULL(`ed.updated_at`, `ed.created_at`) DESC, `ed.id` DESC ";
//	// 构建分页条件
//	sql += " LIMIT " + std::to_string(((query->pageIndex - 1) * query->pageSize)) + "," + std::to_string(query->pageSize);
//
//	// 执行查询
//	return sqlSession->executeQuery<MedicalRecordByTitleDO>(sql, MedicalRecordByTitleMapper(), params);
//}
//
//std::list<MedicalRecordByTitleDO> MedicalRecordDAO::selectByTitle(const string& title)
//{
//	string sql = "SELECT ed.id, edt.name AS document_type_name, ed.title, ed.status, ed.updated_at FROM emr_document ed LEFT JOIN emr_document_type edt ON ed.document_type_code = edt.code WHERE `ed.title` LIKE CONCAT('%',?,'%')";
//	return sqlSession->executeQuery<MedicalRecordByTitleDO>(sql, MedicalRecordByTitleMapper(), "%s", title);
//}

//DiagnosisEntryBriefDO selectNull(const DiagnosisEntryQueryNull::Wrapper& query);
std::list<DiagnosisEntryBriefDO> DiagnosisEntryDAO::selectBrief(const DiagnosisEntryQueryNull::Wrapper& query)
{
	string sql = R"(
SELECT DISTINCT ccd.name
from cln_condition cc
left join cln_condition_definition ccd on ccd.id = cc.definition_id
where cc.tenant_id = ? 
)";
	return sqlSession->executeQuery<DiagnosisEntryBriefDO>(sql, MedicalRecordBriefMapper(), "%s", query->tenant_id.getValue({}));
}

std::string DiagnosisEntryDAO::queryById(const DiagnosisEntryQueryById::Wrapper& query, SqlParams& params)
{
	stringstream sqlCondition;
	sqlCondition << " WHERE 1=1";
	if (query->tenant_id) {

		sqlCondition << " AND `tenant_id` LIKE CONCAT('%',?,'%')";
		SQLPARAMS_PUSH(params, "s", std::string, query->tenant_id.getValue(""));
	}
	if (query->patient_id) {

		sqlCondition << " AND `patient_id` LIKE CONCAT('%',?,'%')";
		SQLPARAMS_PUSH(params, "s", std::string, query->patient_id.getValue(""));
	}
	return sqlCondition.str();
}

uint64_t DiagnosisEntryDAO::count(const DiagnosisEntryQueryById::Wrapper& query)
{
	SqlParams params;
	string sql = "SELECT COUNT(*) FROM cln_condition ";
	// 构建查询条件
	sql += queryById(query, params);
	// 执行查询
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<DiagnosisEntryDetailDO> DiagnosisEntryDAO::selectDetail(const DiagnosisEntryQueryById::Wrapper& query)
{
//    string sql = R"(SELECT DISTINCT
//    cnd.id,
//    def.condition_code,
//    cnd.description,
//    CASE cnd.tcm_flag
//        WHEN 1 THEN '中医'
//        ELSE '西医'
//    END AS tcm_flag,
//    tc.display,
//    CASE aed.primary_flag
//        WHEN 1 THEN '是'
//        ELSE '否'
//FROM cln_condition cnd
//LEFT JOIN cln_condition_definition def 
//    ON cnd.definition_id = def.definition_id
//LEFT JOIN adm_encounter_diagnosis aed 
//    ON aed.condition_id = cnd.id
//LEFT JOIN ter_concept tc 
//    ON tc.code = aed.diag_type_code 
//    AND tc.code_system_id = 384
//WHERE cnd.tenant_id = ? 
//  AND cnd.patient_id = ?;)";

	string sql = R"(SELECT DISTINCT
    cnd.id,
    def.condition_code,
    cnd.description,
    CASE cnd.tcm_flag
        WHEN 1 THEN '中医'
        ELSE '西医'
    END AS tcm_flag,
    tc.display,
    CASE aed.primary_flag
        WHEN 1 THEN '是'
        ELSE '否'
    END AS primary_flag
FROM cln_condition cnd
LEFT JOIN cln_condition_definition def 
    ON cnd.definition_id = def.id
LEFT JOIN adm_encounter_diagnosis aed 
    ON aed.condition_id = cnd.id
LEFT JOIN ter_concept tc 
    ON tc.code = aed.diag_type_code 
    AND tc.code_system_id = 384
WHERE cnd.tenant_id = ? 
  AND cnd.patient_id = ?)";
    sql += " LIMIT " + std::to_string(((query->pageIndex - 1) * query->pageSize)) + "," + std::to_string(query->pageSize);
    return sqlSession->executeQuery<DiagnosisEntryDetailDO>(sql, MedicalRecordDetailMapper(), "%s%s", query->tenant_id.getValue({}),query->patient_id.getValue({}));
}
//PtrMedicalRecordByIdDO DiagnosisEntryDAO::selectBrief(const MedicalRecordById::Wrapper& query)
//{
//	string sql = R"(
//SELECT cc.id, ccd.name
//from cln_condition cc
//left join cln_condition_definition ccd on ccd.id = cc.definition_id
//where cc.tenant_id = ? and cc.patient_id = ?
//)";
//	// 模板调用格式原样保留，不改动调用结构
//	return sqlSession->executeQueryOne<PtrMedicalRecordByIdDO>(sql, PtrMedicalRecordByIdMapper(), "%s",
//		query->cln_condition.tenant_id.getValue({}),
//		query->cln_condition.patient_id.getValue({})
//	);
//}