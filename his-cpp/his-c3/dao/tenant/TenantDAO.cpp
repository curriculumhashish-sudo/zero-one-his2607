#include "stdafx.h"
#include "TenantDAO.h"
#include "SimpleDateTimeFormat.h"

//软删除
int TenantDAO::softDelete(const std::string& id, const std::string& deletedBy)
{
	//获取当前时间
	std::string now = SimpleDateTimeFormat::format();
	int totalAffected = 0;
	std::string sql = "UPDATE whale_tenants "
		"SET is_deleted=1, deleted_at=?, deleted_by=?, "
		"code=CONCAT(code,'#deleted_',?) "
		"WHERE id=? AND is_deleted=0";
	//参数: 删除时间, 删除者, 用作code后缀的id, WHERE 条件里的id
	//删除后，code会变成原来的code + '#deleted_' + id,释放唯一约束,可以添加相同code的租户
	totalAffected += sqlSession->executeUpdate(sql, "%s%s%s%s", now, deletedBy, id, id);
	return totalAffected;
}
