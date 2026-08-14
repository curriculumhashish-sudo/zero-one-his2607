#include "stdafx.h"
#include "EnterOrdersController.h"
#include "../../service/enter-orders/EnterOrdersService.h"
#include "ExcelComponent.h"
using namespace std;
//EnterOrders
//enter-orders
//ENTERORDERS
QueryPageJsonVO::Wrapper EnterOrdersController::executeQueryEnterOrders(const EnterOrdersQueryById::Wrapper& query)
{
	//响应结果
	auto jvo = QueryPageJsonVO::createShared();
	if (!query)
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	//查询数据
	auto result = EnterOrdersService().select(query);

	jvo->success(result);
	return jvo;
}

//StringJsonVO::Wrapper EnterOrdersController::execAddEnterOrders(const EnterOrdersAddDTO::Wrapper& dto, const PayloadDTO& payload)
//{
//	// 定义返回数据对象
//	auto jvo = StringJsonVO::createShared();
//
//	// 参数校验
//	std::string errmsg = dto->validate();
//	if (errmsg != "")
//	{
//		jvo->init(errmsg, RS_PARAMS_INVALID);
//		return jvo;
//	}
//
//	// 执行数据新增
//	dto->setPayload(&payload);
//	std::string id = EnterOrdersService().add(dto);
//	if (id != "") {
//		jvo->success(id);
//	}
//	else
//	{
//		jvo->fail("");
//	}
//	//响应结果
//	return jvo;
//	return {};
//}

StringJsonVO::Wrapper EnterOrdersController::execAddEnterOrders(std::shared_ptr<IncomingRequest> request, const PayloadDTO& payload)
{
    // 0 定义返回数据对象
    auto jvo = StringJsonVO::createShared();
    // 1 初始化
    API_MULTIPART_INIT(container, reader);
    // 2 配置读取器
    API_MULTIPART_CONFIG_MEMO_DEFAULT(reader, -1);
    // 3 读取数据
    request->transferBody(&reader);
    // 4 获取文件数据
    API_MULTIPART_PARSE_FILE_FIELD(container, "excel", file);
    if (!file)
    {
        jvo->init("not set excel file.", RS_PARAMS_INVALID);
        return jvo;
    }

    // 5 解析与校验数据
    std::string errmsg = "";
    std::string errmsg1 = "";
    oatpp::List<EnterOrdersAddDTO::Wrapper> exceldata = oatpp::List<EnterOrdersAddDTO::Wrapper>::createShared();
    oatpp::List<EnterOrdersAddDTO1::Wrapper> exceldata1 = oatpp::List<EnterOrdersAddDTO1::Wrapper>::createShared();

    ExcelComponent::read(file->data(), file->size(), "diagnosis-entry", [exceldata, exceldata1, &errmsg, &errmsg1, &payload](xlnt::worksheet* sheet) {
        // 逐行解析解析数据
        int rn = 0;
        for (auto row : sheet->rows())
        {
            // 跳过表头
            if (rn++ == 0) continue;
            // 解析数据到DTO
            auto dto = EnterOrdersAddDTO::createShared();
            auto dto1 = EnterOrdersAddDTO1::createShared();

            dto->tenant_id = row[0].to_string();
            dto->encounter_id = row[1].to_string();
            dto->patient_id = row[2].to_string();
            dto->group_id = row[3].to_string();
            dto->status = row[4].to_string();
            dto->therapy_type = row[5].to_string();
            dto->dose = row[6].to_string();
            dto->dose_unit = row[7].to_string();
            dto->method = row[8].to_string();
            dto->rate = row[9].to_string();
            dto->created_at = row[10].to_string();
            dto->created_by = row[11].to_string();
            dto->skin_text = row[12].to_string();
            dto->medication_id = row[13].to_string();
            dto->condition_definition_id = row[14].to_string();
            dto->bus_no = row[15].to_string();
            dto->quantity = row[16].to_string();
            dto->unit_code = row[17].to_string();
            dto1->total_price = row[18].to_string(); 

            // 校验主表行
            errmsg = dto->validate();
            if (!errmsg.empty())
            {
                errmsg = "row(" + std::to_string(rn) + ") main table data error, " + errmsg;
                return;
            }
            // 校验子表行
            errmsg1 = dto1->validate();
            if (!errmsg1.empty())
            {
                errmsg1 = "row(" + std::to_string(rn) + ") sub table data error, " + errmsg1;
                return;
            }

            // 赋值操作人信息
            dto->setPayload(&payload);
            exceldata->push_back(dto);
            dto1->setPayload(&payload);
            exceldata1->push_back(dto1);
        }
        });

    // 校验解析异常信息
    if (!errmsg.empty())
    {
        jvo->init(errmsg, RS_PARAMS_INVALID);
        return jvo;
    }
    if (exceldata->empty())
    {
        jvo->init("excel file main table no valid data.", RS_PARAMS_INVALID);
        return jvo;
    }
    if (!errmsg1.empty())
    {
        jvo->init(errmsg1, RS_PARAMS_INVALID);
        return jvo;
    }
    if (exceldata1->empty())
    {
        jvo->init("excel file sub table no valid data.", RS_PARAMS_INVALID);
        return jvo;
    }

    // 6 统一使用同一个Service对象，保证事务一致性
    EnterOrdersService service;
    // 同时传入主表、子表数据，在Service内部开启事务双表插入
    bool saveResult = service.batchAddTwoTable(exceldata, exceldata1);

    if (saveResult)
    {
        jvo->success("Excel医嘱批量导入成功");
    }
    else
    {
        jvo->fail("Excel医嘱批量导入失败，请检查数据或联系管理员");
    }

    return jvo;
}





StringJsonVO::Wrapper EnterOrdersController::execModifyEnterOrders(const EnterOrdersModifyDTO::Wrapper& dto, const PayloadDTO& payload)
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
    if (EnterOrdersService().updateData(dto)) {
        jvo->success(dto->id);
    }
    else
    {
        jvo->fail(dto->id);
    }
    // 响应结果
    return jvo;
}

ListJsonVO<String>::Wrapper EnterOrdersController::execRemoveEnterOrders(const List<String>& ids)
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
    if (EnterOrdersService().removeData(ids)) {
        jvo->success(ids);
    }
    else
    {
        jvo->fail(nullptr);
    }
    // 响应结果
    return jvo;
}