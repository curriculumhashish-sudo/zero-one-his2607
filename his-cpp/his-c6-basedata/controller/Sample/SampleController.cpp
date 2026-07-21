
#include "stdafx.h"
#include "SampleController.h"

// 新增样本
StringJsonVO::Wrapper SampleController::executeAddSample(const SampleAddDTO::Wrapper& dto)
{
	(void)dto; // 先占位，后续接 Service/DAO 时再替换
	auto vo = StringJsonVO::createShared();
	vo->success("add sample success");
	return vo;
}

// 修改样本
StringJsonVO::Wrapper SampleController::executeEditSample(const SampleEditDTO::Wrapper& dto)
{
	(void)dto;
	auto vo = StringJsonVO::createShared();
	vo->success("edit sample success");
	return vo;
}

// 删除样本
StringJsonVO::Wrapper SampleController::executeRemoveSample(const String& id)
{
	(void)id;
	auto vo = StringJsonVO::createShared();
	vo->success("remove sample success");
	return vo;
}

// 启用/停用样本
StringJsonVO::Wrapper SampleController::executeSwitchSampleStatus(const SampleStatusDTO::Wrapper& dto)
{
	(void)dto;
	auto vo = StringJsonVO::createShared();
	vo->success("switch sample status success");
	return vo;
}