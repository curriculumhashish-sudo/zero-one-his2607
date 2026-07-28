#include "SupplierDAO.h"
#include <sstream>
#include "stdafx.h"

int SupplierDAO::batchUpdateStatus(const std::list<std::string>& ids, const std::string& statusEnum) {
	if (ids.empty()) return 0;

	std::stringstream sql;
	sql << "UPDATE cus_supplier SET status_enum = ? WHERE id IN (";

	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, statusEnum);

	bool first = true;
	for (const auto& id : ids) {
		if (first) first = false;
		else sql << ",";
		sql << "?";
		SQLPARAMS_PUSH(params, "s", std::string, id);
	}
	sql << ") AND is_deleted = 0";

	//返回修改的行数
	return sqlSession->executeUpdate(sql.str(), params);
}


int SupplierDAO::batchSoftDelete(const std::list<std::string>& ids,const std::string& deletedBy,const std::string& deletedAt)
{
    if (ids.empty()) return 0;

    std::stringstream sql;
    sql << "UPDATE cus_supplier SET is_deleted = 1, deleted_at = ?, deleted_by = ? WHERE id IN (";

    SqlParams params;
    SQLPARAMS_PUSH(params, "dt", std::string, deletedAt);
    SQLPARAMS_PUSH(params, "s", std::string, deletedBy);

    bool first = true;
    for (const auto& id : ids)
    {
        if (first) first = false;
        else sql << ",";
        sql << "?";
        SQLPARAMS_PUSH(params, "s", std::string, id);
    }
    sql << ") AND is_deleted = 0";

    //返回删除的行数
    return sqlSession->executeUpdate(sql.str(), params);
}


