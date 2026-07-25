
#include "stdafx.h"
#include "DeptController.h"

DeptAddJsonVO::Wrapper DeptController::execAddDept(const DeptAddDTO::Wrapper& dto, const PayloadDTO& payload)
{
	// 定义返回数据对象
	auto jvo = DeptAddJsonVO::createShared();

	// 参数校验
	if (!dto)
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	const std::string error = dto->validate();
	if (!error.empty())
	{
		jvo->init(error, RS_PARAMS_INVALID);
		return jvo;
	}

	// 执行数据新增
	dto->setPayload(&payload);
	// ...
	
	// 返回结果
	jvo->init(nullptr, RS_API_UN_IMPL);
	return jvo;
}

DeptModifyJsonVO::Wrapper DeptController::execModifyDept(const DeptModifyDTO::Wrapper& dto, const PayloadDTO& payload)
{
	// 定义返回数据对象
	auto jvo = DeptModifyJsonVO::createShared();

	// 参数校验
	if (!dto)
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	const std::string error = dto->validate();
	if (!error.empty())
	{
		jvo->init(error, RS_PARAMS_INVALID);
		return jvo;
	}

	// 执行数据修改
	dto->setPayload(&payload);
	// ...
	
	// 返回结果
	jvo->init(nullptr, RS_API_UN_IMPL);
	return jvo;
}

DeptDeleteJsonVO::Wrapper DeptController::execRemoveDept(const DeptDeleteQuery::Wrapper& query)
{
	// 定义返回数据对象
	auto jvo = DeptDeleteJsonVO::createShared();

	// 参数校验
	if (!query)
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	const std::string error = query->validate();
	if (!error.empty())
	{
		jvo->init(error, RS_PARAMS_INVALID);
		return jvo;
	}

	// 执行数据删除
	// ...

	// 返回结果
	jvo->init(nullptr, RS_API_UN_IMPL);
	return jvo;
}
