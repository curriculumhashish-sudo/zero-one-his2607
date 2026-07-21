#pragma once

#ifndef _PHARM_DO_
#define _PHARM_DO_

#include "../DoInclude.h"

// 数据库
class PharmDO : BaseDO
{
	// 定义受保护的字段
	// id
	MYSQL_SYNTHESIZE(string, id, Id);

public:
	PharmDO() : BaseDO("pharm")
	{
		// 添加字段域
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD();
	}

};


// 给PharmDO智能指针设定一个别名方便使用
typedef std::shared_ptr<PharmDO> PtrPharmDO;

#endif // !_PHARM_DO_