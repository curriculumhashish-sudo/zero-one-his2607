#include "stdafx.h"
#include "EnterOrdersController.h"
//#include "../../service/diagnosis-entry/DiagnosisEntryService.h"
using namespace std;
//DiagnosisEntry
//diagnosis-entry
//DIAGNOSISENTRY
QueryPageJsonVO::Wrapper EnterOrdersController::executeQueryEnterOrders(const EnterOrdersQueryById::Wrapper& query)
{
	return {};
	////查询数据
	//auto result = MedicalRecordService().listAll(query);
	////响应结果
	//auto jvo = ByTitlePageJsonVO::createShared();
	//jvo->success(result);
	//return jvo;
}

StringJsonVO::Wrapper EnterOrdersController::execAddEnterOrders(const EnterOrdersAddDTO::Wrapper& dto, const PayloadDTO& payload)
{
	return {};
}

StringJsonVO::Wrapper EnterOrdersController::execModifyEnterOrders(const EnterOrdersModifyDTO::Wrapper& dto, const PayloadDTO& payload)
{
	return {};
}

ListJsonVO<String>::Wrapper EnterOrdersController::execRemoveEnterOrders(const List<String>& ids)
{
	return {};
}