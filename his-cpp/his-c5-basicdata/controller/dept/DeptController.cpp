
#include "stdafx.h"
#include "DeptController.h"

DeptAddJsonVO::Wrapper DeptController::execAddDept(const DeptAddDTO::Wrapper& dto)
{
	auto jvo = DeptAddJsonVO::createShared();
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

	jvo->init(nullptr, RS_API_UN_IMPL);
	return jvo;
}

DeptModifyJsonVO::Wrapper DeptController::execModifyDept(const DeptModifyDTO::Wrapper& dto)
{
	auto jvo = DeptModifyJsonVO::createShared();
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

	jvo->init(nullptr, RS_API_UN_IMPL);
	return jvo;
}

DeptDeleteJsonVO::Wrapper DeptController::execRemoveDept(const DeptDeleteQuery::Wrapper& query)
{
	auto jvo = DeptDeleteJsonVO::createShared();
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

	jvo->init(nullptr, RS_API_UN_IMPL);
	return jvo;
}
