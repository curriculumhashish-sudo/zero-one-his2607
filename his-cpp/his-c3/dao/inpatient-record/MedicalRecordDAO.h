#pragma once
#ifndef _MEDICALDAO_H_
#define _MEDICALDAO_H_
#include "BaseDAO.h"
#include "../../domain/do/inpatient-record/MedicalRecordDO.h"
#include "../../domain/query/inpatient-record/MedicalRecordQuery.h"

/**
 * 示例表数据库操作实现
 */

class MedicalRecordDAO : public BaseDAO
{
private:
	inline std::string queryByTitle(const MedicalRecordQueryByTitle::Wrapper& query, SqlParams& params);
public:
	// 统计数据条数
	uint64_t count(const MedicalRecordQueryByTitle::Wrapper& query);
	// 分页查询数据
	std::list<MedicalRecordByTitleDO> selectWithPage(const MedicalRecordQueryByTitle::Wrapper& query);
	// 通过ID查询数据
	PtrMedicalRecordByIdDO selectById(const MedicalRecordQueryById::Wrapper& query);
};
#endif // !

//MedicalRecordBriefDAO
