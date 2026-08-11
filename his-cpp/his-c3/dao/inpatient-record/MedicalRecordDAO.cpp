#include "stdafx.h"
#include "MedicalRecordDAO.h"
#include "MedicalRecordMapper.h"

std::string MedicalRecordDAO::queryByTitle(const MedicalRecordQueryByTitle::Wrapper& query, SqlParams& params)
{
	stringstream sqlCondition;
	sqlCondition << " WHERE 1=1";
	if (query->title) {

		sqlCondition << " AND title LIKE CONCAT('%',?,'%')";
		SQLPARAMS_PUSH(params, "s", std::string, query->title.getValue(""));
	}
	return sqlCondition.str();
}

uint64_t MedicalRecordDAO::count(const MedicalRecordQueryByTitle::Wrapper& query)
{
	SqlParams params;
	string sql = "SELECT COUNT(*) FROM emr_document ";
	// 构建查询条件
	sql += queryByTitle(query, params);
	// 执行查询
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<MedicalRecordByTitleDO> MedicalRecordDAO::selectWithPage(const MedicalRecordQueryByTitle::Wrapper& query)
{
	SqlParams params;
	string sql = R"(
	SELECT ed.id, edt.name AS document_type_name, ed.title, ed.status, ed.updated_at
	FROM emr_document ed
	LEFT JOIN emr_document_type edt ON ed.document_type_code = edt.code
	WHERE ed.title LIKE CONCAT('%',?,'%')
	)";
	// 构建分页条件
	sql += " LIMIT " + std::to_string(((query->pageIndex - 1) * query->pageSize)) + "," + std::to_string(query->pageSize);

	// 执行查询
	return sqlSession->executeQuery<MedicalRecordByTitleDO>(sql, MedicalRecordByTitleMapper(), "%s", query->title.getValue({}));
}


PtrMedicalRecordByIdDO MedicalRecordDAO::selectById(const MedicalRecordQueryById::Wrapper& query)
{
	string sql = "SELECT id,content from emr_document where id=?";
	return sqlSession->executeQueryOne<PtrMedicalRecordByIdDO>(sql, PtrMedicalRecordByIdMapper(), "%s", query->id.getValue({}));
}
