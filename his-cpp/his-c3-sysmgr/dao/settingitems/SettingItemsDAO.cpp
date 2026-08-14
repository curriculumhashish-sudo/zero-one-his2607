#include "stdafx.h"
#include "SettingItemsDAO.h"
#include "SimpleDateTimeFormat.h"

//软删除
//
int SettingItemsDAO::softDelete(const std::string& id, const std::string& deletedBy) {
	std::string now = SimpleDateTimeFormat::format();    //获取现在的时间
	int totalAffected = 0;								 //被影响的行数
	std::string sql = "UPDATE whale_settings "
		"SET is_deleted=1, deleted_at=?, deleted_by=? "
		"WHERE id=? AND is_deleted=0";
	totalAffected += sqlSession->executeUpdate(sql, "%s%s%s", now, deletedBy, id);
	return totalAffected;
}

