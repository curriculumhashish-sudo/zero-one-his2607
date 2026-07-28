#pragma once

#ifndef _TREATMENT_DAO_
#define _TREATMENT_DAO_
#include "BaseDAO.h"
#include "../../domain/do/treatment/WkfActivityDefinitionDo.h"
#include "../../domain/do/treatment/WkfActivityDefinitionChildrenDo.h"
#include "../../domain/do/treatment/AdmEpisodeOfCareEncounterDO.h"
#include "../../domain/do/treatment/AdmEpisodeOfCareDO.h"
#include "../../domain/query/treatment/TreatmentQuery.h"

/**
 * 示例表数据库操作实现
 */
class TreatmentDAO : public BaseDAO
{
private:
	//************************************
	// Method:      queryConditionBuilder
	// FullName:    SampleDAO::queryConditionBuilder
	// Access:      private 
	// Returns:     std::string 返回条件SQL语句，即WHERE部分的语句
	// Parameter:   const SampleQuery::Wrapper& query 查询数据对象
	// Parameter:   SqlParams& params 存放查询数据的参数对象
	// Description: 查询条件构建器，根据SampleQuery对象动态构建查询条件相关参数
	//************************************
	inline std::string queryConditionBuilder(const TreatmentQuery::Wrapper& query, SqlParams& params);
public:
	// 统计数据条数
	uint64_t count(const TreatmentQuery::Wrapper& query);
	// 分页查询数据
	std::list<TreatmentDO> selectWithPage(const TreatmentQuery::Wrapper& query);
	// 通过姓名查询数据
	std::list<TreatmentDO> selectByName(const string& name);
	// 通过ID查询数据
	PtrTreatmentDO selectById(std::string id);
	// 批量ID软删除：标准容器std::list嵌套std::string
	int32_t removeData(std::list<std::string>& ids);
};
#endif // !_TREATMENT_DAO_
