#include "stdafx.h"
#include "EquipmentController.h"
#include "../../service/equipment/EquipmentService.h"
#include "../../domain/bo/PdfReportBO.h"
#include "../ApiDeclarativeServicesHelper.h"
#include "ExcelComponent.h"
#include "SimpleDateTimeFormat.h"
#include "PdfComponent.h"

// 器材列表分页查询
EquipmentPageJsonVO::Wrapper EquipmentController::execQueryEquipment(const EquipmentQuery::Wrapper& query)
{
	// 查询数据
	auto result = EquipmentService().listAll(query);
	// 响应结果
	auto jvo = EquipmentDetailPageJsonVO::createShared();
	jvo->success(result);
	return jvo;
}

// 器材详情
EquipmentJsonVO::Wrapper EquipmentController::execQueryById(const String& id)
{
	// 定义返回数据对象
	auto jvo = EquipmentJsonVO::createShared();
	// 参数校验
	// 非空校验
	if (!id)
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	// 执行数据新增
	auto res = EquipmentService().getById(id.getValue({}));
	jvo->success(res);

	//响应结果
	return jvo;
}


// 新增器材
StringJsonVO::Wrapper EquipmentController::execAddEquipment(const EquipmentAddDTO::Wrapper& dto, const PayloadDTO& payload)
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
	std::string id = EquipmentService().saveData(dto);
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

//修改设备
StringJsonVO::Wrapper EquipmentController::execModifyEquipment(const EquipmentModifyDTO::Wrapper& dto, const PayloadDTO& payload)
{
	return {};
}

//批量批量删除器材
StringJsonVO::Wrapper EquipmentController::execDeleteEquipment(const EquipmentDeleteDTO::Wrapper& dto, const PayloadDTO& payload)
{
	return {};
}

//批量启停用器材
StringJsonVO::Wrapper EquipmentController::execChangeEquipmentStatus(const EquipmentStatusDTO::Wrapper& dto, const PayloadDTO& payload)
{
	return {};
}

//选择器材
EquipmentSelectPageJsonVO::Wrapper EquipmentController::execSelectEquipment(const EquipmentQuery::Wrapper& query)
{
	return {};
}

// 声明式服务接口1
EquipmentJsonVO::Wrapper EquipmentController::execQueryOne(const String& id, const PayloadDTO& payload)
{
	// 创建客户端对象
	API_CLIENT_CREATE(ac, om, EquipmentApiClient, "equipment-api");
	// 构建凭证
	std::string token = PayloadDTO::getTokenPrefix() + payload.getToken();
	// 返回查询结果
	return ac->queryById(token, id)->readBodyToDto<EquipmentJsonVO::Wrapper>(om);
}

// 声明式服务接口2
EquipmentPageJsonVO::Wrapper EquipmentController::execQueryAll(const EquipmentQuery::Wrapper& query, const PayloadDTO& payload)
{
	// 创建客户端对象
	API_CLIENT_CREATE(ac, om, EquipmentApiClient, "equipment-api");
	// 构建凭证
	std::string token = PayloadDTO::getTokenPrefix() + payload.getToken();
	// 返回查询结果
	return ac->queryAll(token, query->pageIndex, query->pageSize, URIUtil::urlEncode(query->name))->readBodyToDto<EquipmentPageJsonVO::Wrapper>(om);
}