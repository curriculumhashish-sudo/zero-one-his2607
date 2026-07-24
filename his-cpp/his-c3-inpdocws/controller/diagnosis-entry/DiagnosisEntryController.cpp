#include "stdafx.h"
#include "DiagnosisEntryController.h"
//#include "../../service/diagnosis-entry/DiagnosisEntryService.h"
using namespace std;
//DiagnosisEntry
//diagnosis-entry
//DIAGNOSISENTRY
DiagnosisEntryPageJsonVO::Wrapper DiagnosisEntryController::executeQueryBrief(const DiagnosisEntryQueryNull::Wrapper& query)
{
	return {};
	//cout << "executeQueryTitle" << endl;
	//cout << "query.getPageSize() = " << query->pageSize.getValue({}) << endl;
	//cout << "query.getPageIndex() = " << query->pageIndex.getValue({}) << endl;


	////查询数据
	//auto result = MedicalRecordService().listAll(query);
	////响应结果
	//auto jvo = ByTitlePageJsonVO::createShared();
	//jvo->success(result);
	//return jvo;
}

DiagnosisEntryDetailPageJsonVO::Wrapper DiagnosisEntryController::executeQueryDetail(const DiagnosisEntryQueryById::Wrapper& query)
{
	return {};
	////查询数据
	//auto result = MedicalRecordService().listAll(query);
	////响应结果
	//auto jvo = ByTitlePageJsonVO::createShared();
	//jvo->success(result);
	//return jvo;
}