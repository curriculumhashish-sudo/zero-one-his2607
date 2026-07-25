#pragma once
#ifndef _DEPT_BATCH_STATUS_DAO_H_
#define _DEPT_BATCH_STATUS_DAO_H_

#include "BaseDAO.h"
#include "../../domain/dto/dept/DeptBatchStatusDTO.h"

class DeptBatchStatusDAO : public BaseDAO
{
public:
	// 删掉自定义构造、析构，完全继承BaseDAO默认构造
	~DeptBatchStatusDAO() override = default;

	int batchUpdateStatus(const DeptBatchUpdateDTO::Wrapper& updateDto);
};

#endif
