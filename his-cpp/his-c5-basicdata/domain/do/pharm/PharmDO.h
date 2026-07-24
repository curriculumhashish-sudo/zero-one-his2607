#pragma once

#ifndef _PHARM_DO_
#define _PHARM_DO_

#include "../DoInclude.h"

// 数据库
class PharmDO : BaseDO
{
	// 定义受保护的字段
	// id
	MYSQL_SYNTHESIZE(UInt64, id, Id);
	// name: 名称
	MYSQL_SYNTHESIZE(string, name, Name);
	// typeEnum: 类型枚举值
	MYSQL_SYNTHESIZE(string, typeEnum, TypeEnum);
	// classRoom: 上级科室
	MYSQL_SYNTHESIZE(string, classRoom, ClassRoom);
	// statusEnum: 状态
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);

public:
	PharmDO() : BaseDO("pharm")
	{
		// 添加字段域
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("name", "s", name);
		MYSQL_ADD_FIELD("typeEnum", "s", typeEnum);
		MYSQL_ADD_FIELD("classRoom", "s", classRoom);
		MYSQL_ADD_FIELD("statusEnum", "s", statusEnum);
	}

};


// 给PharmDO智能指针设定一个别名方便使用
typedef std::shared_ptr<PharmDO> PtrPharmDO;

#endif // !_PHARM_DO_