#include "stdafx.h"
#include "DiagnosisController.h"
#include "../../service/Diagnosis/DiagnoseService.h"
#include "../ApiDeclarativeServicesHelper.h"
#include "ExcelComponent.h"
#include "PdfComponent.h"
#include "domain/vo/BaseJsonVO.h"

//新增表单
StringJsonVO::Wrapper DiagnosisController::execAddDiagnose(const DiagnoseAddDTO::Wrapper& dto, const PayloadDTO& payload)
{
	// 定义返回数据对象
	auto jvo = StringJsonVO::createShared();

	// 参数校验
	std::string errmsg = dto->validate();
	if (errmsg != "")
	{
		jvo->init(errmsg, RS_PARAMS_INVALID);
		return jvo;
	}

	// 执行数据新增
	dto->setPayload(&payload);
	std::string id = DiagnoseService().saveData(dto);
	if (id != "") {
		jvo->success(id);
	}
	else
	{
		jvo->fail("");
	}
	//响应结果
	return jvo;
}

////新增分类
//StringJsonVO::Wrapper DiagnosisController::execAddDiagcategory(const UserAddDTO::Wrapper& dto)
//{
//	DiagcategoryService us;
//	auto jvo = StringJsonVO::createShared();
//	auto id = us.savecategory(dto);
//	if (id != "")
//		jvo->success(id);
//	else
//		jvo->fail({});
//	return jvo;
//}

//修改
StringJsonVO::Wrapper DiagnosisController::execModifyDiagnose(const DiagnoseDTO::Wrapper& dto, const PayloadDTO& payload)
{
	// 定义返回数据对象
	auto jvo = StringJsonVO::createShared();
	// 参数校验
	if (!dto->id)
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	// 执行数据修改
	dto->setPayload(&payload);
	if (DiagnoseService().updateData(dto)) {
		jvo->success(dto->id);
	}
	else
	{
		jvo->fail(dto->id);
	}
	// 响应结果
	return jvo;
}

//删除
ListJsonVO<String>::Wrapper DiagnosisController::execRemoveDiagnose(const List<String>& ids)
{
	// 定义返回数据对象
	auto jvo = ListJsonVO<String>::createShared();
	// 参数校验
	if (ids->empty())
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	// 执行数据删除
	if (DiagnoseService().removeData(ids)) {
		jvo->success(ids);
	}
	else
	{
		jvo->fail(nullptr);
	}
	// 响应结果
	return jvo;
}

//批量停用/启用
StringJsonVO::Wrapper DiagnosisController::execBatchChangeDiagnoseStatus(const DiagnoseBatchStatusDTO::Wrapper& dto, const PayloadDTO& payload)
{
	auto jvo = StringJsonVO::createShared();

	if (!dto->ids || !dto->status)
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	if (*dto->status != 0 && *dto->status != 1)
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	dto->setPayload(&payload);

	// 调用service批量修改状态
	if (DiagnoseService().batchChangeStatus(dto->ids, dto->status))
	{
		jvo->success(nullptr);
	}
	else
	{
		jvo->fail(nullptr);
	}

	// 响应结果
	return jvo;
}