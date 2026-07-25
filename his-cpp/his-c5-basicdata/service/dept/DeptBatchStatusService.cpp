#include "DeptBatchStatusService.h"
#include "SimpleDateTimeFormat.h"
#include "stdafx.h"

bool DeptBatchStatusService::batchToggleDeptStatus(const DeptBatchStatusDTO::Wrapper& reqDto)
{
	// 1. 参数校验已经在Controller完成，此处直接组装内部传输DTO
	auto updateDto = DeptBatchUpdateDTO::createShared();

	// 赋值ID数组
	updateDto->idList = reqDto->deptIdList;

	// 2. 根据前端数字状态，分别赋值枚举字符串、布尔标记（双字段同步）
	if (reqDto->targetStatus == 1)
	{
		// 启用
		updateDto->statusEnum = "ACTIVE";
		updateDto->activeFlag = true;
	}
	else
	{
		// 停用
		updateDto->statusEnum = "INACTIVE";
		updateDto->activeFlag = false;
	}

	// 3. 后端获取登录操作者账号（从载荷取出，前端不可篡改）
	updateDto->updateBy = reqDto->getPayload()->getUsername();
	// 4. 调用项目静态工具类获取当前标准时间（默认格式 yyyy-MM-dd HH:mm:ss）
	updateDto->updateTime = SimpleDateTimeFormat::format();

	// 5. 栈创建Dao对象，调用批量更新方法
	DeptBatchStatusDAO deptDao;
	int affectRows = deptDao.batchUpdateStatus(updateDto);

	// 修改行数等于传入ID总数 = 全部更新成功
	return affectRows == updateDto->idList->size();
}
