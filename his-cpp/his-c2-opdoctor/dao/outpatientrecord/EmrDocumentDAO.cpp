#include "stdafx.h"
#include "EmrDocumentDAO.h"
#include "EmrDocumentMapper.h"
#include "SqlSession.h"
#include <sstream>

#define QUERY_HEAD "SELECT id,patient_id,encounter_id,document_type_code,title,content,status," \
                    "source_template_id,created_at,updated_at,created_by,updated_by," \
                    "is_deleted,tenant_id FROM emr_document "

PtrEmrDocumentDO EmrDocumentDAO::selectById(const std::string& id)
{
	std::ostringstream sql;
	sql << QUERY_HEAD;
	sql << "WHERE id=? AND is_deleted=0";
	EmrDocumentMapper mapper;
	SqlParams params = { SqlParam("s", std::make_shared<std::string>(id)) };
	auto list = sqlSession->executeQuery<PtrEmrDocumentDO>(sql.str(), mapper, params);
	if (list.empty())
		return nullptr;
	return list.front();
}
