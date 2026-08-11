#include "stdafx.h"
#include "DiagnosisEntryController.h"
#include "../../service/diagnosis-entry/DiagnosisEntryService.h"
using namespace std;
//DiagnosisEntry
//diagnosis-entry
//DIAGNOSISENTRY


DiagnosisEntryPageJsonVO::Wrapper DiagnosisEntryController::executeQueryBrief(const DiagnosisEntryQueryNull::Wrapper& query)
{
	//响应结果
	auto jvo = DiagnosisEntryPageJsonVO::createShared();
	if (!query)
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	//查询数据
	auto result = DiagnosisEntryService().listAllBrief(query);

	jvo->success(result);
	return jvo;
}

DiagnosisEntryDetailPageJsonVO::Wrapper DiagnosisEntryController::executeQueryDetail(const DiagnosisEntryQueryById::Wrapper& query)
{
	//响应结果
	auto jvo = DiagnosisEntryDetailPageJsonVO::createShared();
	if (!query)
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	//查询数据
	auto result = DiagnosisEntryService().listAllDetail(query);

	jvo->success(result);
	return jvo;
}