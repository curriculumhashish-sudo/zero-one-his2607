#include "stdafx.h"
#include "SampleController.h"
#include "service/SampleService.h"

SampleListJsonVO::Wrapper SampleController::executeListSampleName(const SampleNameQuery::Wrapper& query)
{
	SampleService service;
	auto vo = SampleListJsonVO::createShared();
	for (const auto& row : service.listSampleNames(query))
	{
		vo->addData(row);
	}
	vo->setStatus(RS_SUCCESS);
	return vo;
}

SamplePageJsonVO::Wrapper SampleController::executeListSamplePage(const SampleQuery::Wrapper& query)
{
	SampleService service;
	auto vo = SamplePageJsonVO::createShared();
	vo->success(service.listSamplePage(query));
	return vo;
}

StringJsonVO::Wrapper SampleController::executeAddSample(const SampleAddDTO::Wrapper& dto)
{
	SampleService service;
	service.addSample(dto);
	auto vo = StringJsonVO::createShared();
	vo->success("add sample success");
	return vo;
}

StringJsonVO::Wrapper SampleController::executeEditSample(const SampleEditDTO::Wrapper& dto)
{
	SampleService service;
	service.editSample(dto);
	auto vo = StringJsonVO::createShared();
	vo->success("edit sample success");
	return vo;
}

StringJsonVO::Wrapper SampleController::executeRemoveSample(const SampleRemoveQuery::Wrapper& dto)
{
	SampleService service;
	service.removeSample(dto->id.getValue(""));
	auto vo = StringJsonVO::createShared();
	vo->success("remove sample success");
	return vo;
}

StringJsonVO::Wrapper SampleController::executeSwitchSampleStatus(const SampleStatusDTO::Wrapper& dto)
{
	SampleService service;
	service.switchSampleStatus(dto);
	auto vo = StringJsonVO::createShared();
	vo->success("switch sample status success");
	return vo;
}
