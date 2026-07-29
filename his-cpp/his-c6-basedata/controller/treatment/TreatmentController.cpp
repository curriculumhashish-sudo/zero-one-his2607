/*
 Copyright Zero One Star. All rights reserved.

 @Author: AI Assistant
 @Date: 2026/07/19 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#include "stdafx.h"
#include "TreatmentController.h"
#include "../../service/treatment/TreatmentService.h"
#include "../../domain/bo/PdfReportBO.h"
#include "../ApiDeclarativeServicesHelper.h"
#include "ExcelComponent.h"
#include "SimpleDateTimeFormat.h"
#include "PdfComponent.h"

// 列表分页
TreatmentPageJsonVO::Wrapper TreatmentController::execQueryTreatment(const TreatmentQuery::Wrapper& query)
{
	// 查询数据
	auto result = TreatmentService().listAll(query);
	// 响应结果
	auto jvo = TreatmentPageJsonVO::createShared();
	jvo->success(result);
	return jvo;
}

// 通过ID查询
TreatmentDetailJsonVO::Wrapper TreatmentController::execQueryById(const String& id)
{
	// 定义返回数据对象
	auto jvo = TreatmentDetailJsonVO::createShared();

	// 参数校验
	// 非空校验
	if (!id)
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	// 执行数据查询
	auto res = TreatmentService().getById(id.getValue({}));
	jvo->success(res);

	// 响应结果
	return jvo;
}

// 停/启用
StringJsonVO::Wrapper TreatmentController::execModifyStatus(const TreatmentStatusDTO::Wrapper& dto, const PayloadDTO& payload)
{
	// Validate the request and return a sample success response.
	auto vo = StringJsonVO::createShared();
	if (!dto || !dto->ids || dto->ids->size() == 0) {
		vo->fail("ids empty");
		return vo;
	}
	vo->success("OK");
	return vo;
}

// 删除（支持批量）
ListJsonVO<String>::Wrapper TreatmentController::execRemoveTreatment(const List<String>& ids)
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
	if (TreatmentService().removeData(ids)) {
		jvo->success(ids);
	}
	else
	{
		jvo->fail(nullptr);
	}
	// 响应结果
	return jvo;
}

// 修改
StringJsonVO::Wrapper TreatmentController::execModifyTreatment(
	const TreatmentModifyDTO::Wrapper& dto, const PayloadDTO& payload)
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

	// 执行数据修改
	dto->setPayload(&payload);
	if (TreatmentService().updateData(dto)) {
		jvo->success(dto->id);
	}
	else
	{
		jvo->fail(dto->id);
	}
	// 响应结果
	return jvo;
}

// 新增
StringJsonVO::Wrapper TreatmentController::execAddTreatment(
	const TreatmentAddDTO::Wrapper& dto, const PayloadDTO& payload)
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
	std::string id = TreatmentService().saveData(dto);
	if (id != "") {
		jvo->success(id);
	}
	else
	{
		jvo->fail("");
	}
	// 响应结果
	return jvo;
}

// 选择分页
TreatmentSelectPageJsonVO::Wrapper TreatmentController::execQuerySelect(const TreatmentQuery::Wrapper& query)
{
	// Query select list data
	auto result = TreatmentService().listSelect(query);
	// Build response
	auto jvo = TreatmentSelectPageJsonVO::createShared();
	jvo->success(result);
	return jvo;
}




//// 声明式服务1
//TreatmentDetailJsonVO::Wrapper TreatmentController::execQueryOne(const String& id, const PayloadDTO& payload)
//{
//	// 创建客户端对象
//	API_CLIENT_CREATE(ac, om, TreatmentApiClient, "treatment-api");
//	// 构建凭证
//	std::string token = PayloadDTO::getTokenPrefix() + payload.getToken();
//	// 返回查询结果
//	return ac->queryById(token, id)->readBodyToDto<TreatmentDetailJsonVO::Wrapper>(om);
//}
//
//// 声明式服务接口2
//TreatmentPageJsonVO::Wrapper TreatmentController::execQueryAll(const TreatmentQuery::Wrapper& query, const PayloadDTO& payload)
//{
//	// 创建客户端对象
//	API_CLIENT_CREATE(ac, om, TreatmentApiClient, "treatment-api");
//	// 构建凭证
//	std::string token = PayloadDTO::getTokenPrefix() + payload.getToken();
//	// 返回查询结果
//	return ac->queryAll(token, query->pageIndex, query->pageSize, URIUtil::urlEncode(query->keywords))->readBodyToDto<TreatmentPageJsonVO::Wrapper>(om);
//}
//
//// 导入诊疗
//StringJsonVO::Wrapper TreatmentController::execImportTreatment(std::shared_ptr<IncomingRequest> request, const PayloadDTO& payload)
//{
//	// 0 定义返回数据对象
//	auto jvo = StringJsonVO::createShared();
//	// 1 初始化
//	API_MULTIPART_INIT(container, reader);
//	// 2 配置读取器
//	API_MULTIPART_CONFIG_MEMO_DEFAULT(reader, -1);
//	// 3 读取数据
//	request->transferBody(&reader);
//	// 4 获取文件数据
//	API_MULTIPART_PARSE_FILE_FIELD(container, "excel", file);
//	if (!file)
//	{
//		jvo->init("not set excel file.", RS_PARAMS_INVALID);
//		return jvo;
//	}
//	// 5 解析与校验数据
//	std::string errmsg = "";
//	oatpp::List<TreatmentAddDTO::Wrapper> exceldata = oatpp::List<TreatmentAddDTO::Wrapper>::createShared();
//	ExcelComponent::read(file->data(), file->size(), "treatment", [exceldata, &errmsg, &payload](xlnt::worksheet* sheet) {
//		// 逐行解析数据
//		int rn = 0;
//		for (auto row : sheet->rows())
//		{
//			// 跳过表头
//			if (rn++ == 0) continue;
//			// 解析数据到DTO（列：name, categoryCode, typeEnum, packageUnitCode, pyStr, wbStr, descriptionText）
//			auto dto = TreatmentAddDTO::createShared();
//			dto->name = row[0].to_string();
//			dto->categoryCode = row[1].to_string();
//			dto->typeEnum = std::atoi(row[2].to_string().c_str());
//			// 校验数据
//			errmsg = dto->validate();
//			if (errmsg != "")
//			{
//				errmsg = "row(" + std::to_string(rn) + ") data error, " + errmsg;
//				return;
//			}
//			// 数据存储到添加列表中
//			dto->setPayload(&payload);
//			exceldata->push_back(dto);
//		}
//		});
//	if (errmsg != "")
//	{
//		jvo->init(errmsg, RS_PARAMS_INVALID);
//		return jvo;
//	}
//	if (exceldata->size() == 0)
//	{
//		jvo->init("excel file no data or not excel file.", RS_PARAMS_INVALID);
//		return jvo;
//	}
//
//	// 6 保存数据
//	bool res = TreatmentService().saveDataBatch(exceldata);
//	if (res)
//	{
//		jvo->success(nullptr);
//		return jvo;
//	}
//	jvo->fail(nullptr);
//	return jvo;
//}
//
//// 导出pdf
//StringJsonVO::Wrapper TreatmentController::execExportTreatmentPdf(const TreatmentQuery::Wrapper& query)
//{
//	// 查询数据
//	auto result = TreatmentService().listAll(query);
//	// 绘制PDF
//	PdfComponent pdf;
//	OatppAccessor<TreatmentDetailDTO::Wrapper> dataAccessor(result->rows);
//	pdf.drawWithTemplate("tpl/treatment-pdf-report.yml", "treatment-pdf-report", &dataAccessor);
//	// 保存PDF
//	auto data = pdf.saveDocToMem();
//	// 转换成base64
//	auto base64Str = jwt::base::encode<jwt::alphabet::base64>(std::string(data.begin(), data.end()));
//	// 响应数据
//	auto jvo = StringJsonVO::createShared();
//	jvo->success(base64Str);
//	return jvo;
//}
//
//// 导出Excel
//std::shared_ptr<oatpp::web::server::api::ApiController::OutgoingResponse> TreatmentController::execExportTreatment(const TreatmentQuery::Wrapper& query)
//{
//	// 查询数据
//	auto result = TreatmentService().listAll(query);
//
//	// 将数据写入到Excel中
//	auto buff = ExcelComponent().writeVectorToBuff("treatment",
//		[result](ExcelComponent* ex) {
//			// 写入表头
//			ex->addHeader({
//				ZH_WORDS_GETTER("treatment.field.id") ,
//				});
//			// 写入数据
//			int row = 2;
//			int col = 1;
//			for (auto item : *(result->rows.get())) {
//				col = 1;
//				ex->setCellValue(row, col++, item->id);
//				ex->setRowProperties(row++);
//			}
//		});
//
//	// 组装下发数据
//	const char* charData = reinterpret_cast<const char*>(buff.data());
//	auto fstring = String(charData, buff.size());
//
//	// 创建响应头
//	auto response = createResponse(Status::CODE_200, fstring);
//
//	// 设置响应头信息
//	std::string filename = "rp-treatment-" + SimpleDateTimeFormat::format() + ".xlsx";
//	response->putHeader("Content-Disposition", "attachment; filename=" + filename);
//	response->putHeader(Header::CONTENT_TYPE, " application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
//
//	// 影响成功结果
//	return response;
//}