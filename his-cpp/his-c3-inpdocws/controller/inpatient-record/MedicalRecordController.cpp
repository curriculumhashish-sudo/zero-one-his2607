#include "stdafx.h"
#include "MedicalRecordController.h"
#include "../../service/inpatient-record/MedicalRecordService.h"
using namespace std;

ByTitlePageJsonVO::Wrapper MedicalRecordController::executeQueryByTitle(const MedicalRecordQueryByTitle::Wrapper& query)
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

ByIdJsonVO::Wrapper MedicalRecordController::executeQueryById(const MedicalRecordQueryById::Wrapper& query)
{
	return {};
	////查询数据
	//auto result = MedicalRecordService().listAll(query);
	////响应结果
	//auto jvo = ByTitlePageJsonVO::createShared();
	//jvo->success(result);
	//return jvo;
}

StringJsonVO::Wrapper MedicalRecordController::execAddMedicalRecord(const MedicalRecordAddDTO::Wrapper& dto, const PayloadDTO& payload)
{
	return {};
}

//StringJsonVO::Wrapper MedicalRecordController::execModifyMedicalRecord(const MedicalRecordModifyDTO::Wrapper& dto, const PayloadDTO& payload)
//{
//	return {  };
//}