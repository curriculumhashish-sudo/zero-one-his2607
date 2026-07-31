#ifndef FREQUENCY_DO_H
#define FREQUENCY_DO_H

#include <string>
#include <cstdint>
#include "../DoInclude.h"

// 频次数据库实体对象
class CusFrequencyDO : public BaseDO
{
	// 主键
	MYSQL_SYNTHESIZE(string, id, Id);
	// 租户标识
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);
	// 创建时间
	MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
	// 最后更新时间
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
	// 创建人
	MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
	// 最后更新人
	MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
	// 软删标记 0-未删 1-已删
	MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
	// 删除时间
	MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
	// 删除人
	MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
	// 乐观锁版本号
	MYSQL_SYNTHESIZE(int, version, Version);
	// 频次编码
	MYSQL_SYNTHESIZE(string, rateCode, RateCode);
	// 频次名称
	MYSQL_SYNTHESIZE(string, name, Name);
	// 周期类型
	MYSQL_SYNTHESIZE(string, cycleType, CycleType);
	// 日计数
	MYSQL_SYNTHESIZE(int, dayCount, DayCount);
	// 日间隔
	MYSQL_SYNTHESIZE(int, dayInterval, DayInterval);
	// 日执行时点串
	MYSQL_SYNTHESIZE(string, dayTimes, DayTimes);
	// 周周期标记
	MYSQL_SYNTHESIZE(int, weekCycleFlag, WeekCycleFlag);
	// 总执行次数
	MYSQL_SYNTHESIZE(int, totalExecutionCount, TotalExecutionCount);
	// 执行周期
	MYSQL_SYNTHESIZE(int, executionPeriod, ExecutionPeriod);
	// 执行周期单位
	MYSQL_SYNTHESIZE(string, executionPeriodUnit, ExecutionPeriodUnit);
	// 适用场景
	MYSQL_SYNTHESIZE(string, applicableScenes, ApplicableScenes);
	// 允许跨日
	MYSQL_SYNTHESIZE(string, allowCrossDay, AllowCrossDay);
	// 启用标记 1-启用 0-停用
	MYSQL_SYNTHESIZE(int, activeFlag, ActiveFlag);
	// 排序号
	MYSQL_SYNTHESIZE(int, sortNo, SortNo);
public:
	CusFrequencyDO() : BaseDO("cus_frequency")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, true);
		MYSQL_ADD_FIELD_NULLABLE("created_at", "dt", createdAt, false);
		MYSQL_ADD_FIELD_NULLABLE("updated_at", "dt", updatedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy, true);
		MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy, true);
		MYSQL_ADD_FIELD_NULLABLE("is_deleted", "i", isDeleted, false);
		MYSQL_ADD_FIELD_NULLABLE("deleted_at", "dt", deletedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy, true);
		MYSQL_ADD_FIELD_NULLABLE("version", "i", version, false);
		MYSQL_ADD_FIELD_NULLABLE("rate_code", "s", rateCode, true);
		MYSQL_ADD_FIELD_NULLABLE("name", "s", name, false);
		MYSQL_ADD_FIELD_NULLABLE("cycle_type", "s", cycleType, false);
		MYSQL_ADD_FIELD_NULLABLE("day_count", "i", dayCount, true);
		MYSQL_ADD_FIELD_NULLABLE("day_interval", "i", dayInterval, true);
		MYSQL_ADD_FIELD_NULLABLE("day_times", "s", dayTimes, true);
		MYSQL_ADD_FIELD_NULLABLE("week_cycle_flag", "i", weekCycleFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("total_execution_count", "i", totalExecutionCount, true);
		MYSQL_ADD_FIELD_NULLABLE("execution_period", "i", executionPeriod, true);
		MYSQL_ADD_FIELD_NULLABLE("execution_period_unit", "s", executionPeriodUnit, true);
		MYSQL_ADD_FIELD_NULLABLE("applicable_scenes", "s", applicableScenes, true);
		MYSQL_ADD_FIELD_NULLABLE("allow_cross_day", "s", allowCrossDay, true);
		MYSQL_ADD_FIELD_NULLABLE("active_flag", "i", activeFlag, false);
		MYSQL_ADD_FIELD_NULLABLE("sort_no", "i", sortNo, true);
	}
};
typedef std::shared_ptr<CusFrequencyDO> PtrCusFrequencyDO;
#endif // FREQUENCY_DO_H