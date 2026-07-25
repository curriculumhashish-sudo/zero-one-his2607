#pragma once
#ifndef _DEPT_BATCH_STATUS_SERVICE_H_
#define _DEPT_BATCH_STATUS_SERVICE_H_

#include "../../domain/dto/dept/DeptBatchStatusDTO.h"
#include "../../dao/dept/DeptBatchStatusDAO.h"

class DeptBatchStatusService
{
public:
	/**
	 * 批量启停科室业务逻辑入口
	 * @param reqDto 前端提交请求DTO
	 * @return true成功 false失败
	 */
	bool batchToggleDeptStatus(const DeptBatchStatusDTO::Wrapper& reqDto);
};

#endif
