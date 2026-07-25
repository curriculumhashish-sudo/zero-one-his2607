#pragma once

#ifndef _FREQUENCYDO_H_
#define _FREQUENCYDO_H_

#include "../DoInclude.h"

/**
 * 频次定义DO
 *
 * 对应数据库：
 * cus_frequency
 *
 * 作用：
 * 查询用药频次
 *
 * 用于：
 * 处方签发时频次下拉选择
 */
class FrequencyDO : public BaseDO
{
	// 频次ID
	MYSQL_SYNTHESIZE(string, id, Id);
	// 频次编码
	MYSQL_SYNTHESIZE(string, rateCode, RateCode);
	// 频次名称
	// 例如：每日一次
	MYSQL_SYNTHESIZE(string, name, Name);
	// 周期类型
	MYSQL_SYNTHESIZE(string, cycleType, CycleType);
	// 每日执行时间
	MYSQL_SYNTHESIZE(string, dayTimes, DayTimes);
	// 是否启用
	MYSQL_SYNTHESIZE(bool, activeFlag, ActiveFlag);

public:
	FrequencyDO()
		: BaseDO("cus_frequency")
	{
		// 主键
		MYSQL_ADD_FIELD_PK("id", "s", id);
		// 频次编码
		MYSQL_ADD_FIELD_NULLABLE("rate_code", "s", rateCode, true);
		// 频次名称
		MYSQL_ADD_FIELD("name", "s", name);
		// 周期类型
		MYSQL_ADD_FIELD("cycle_type", "s", cycleType);
		// 执行时间
		MYSQL_ADD_FIELD_NULLABLE("day_times", "s", dayTimes, true);
		// 是否启用
		MYSQL_ADD_FIELD("active_flag", "b", activeFlag);
	}
};

typedef std::shared_ptr<FrequencyDO> PtrFrequencyDO;

#endif