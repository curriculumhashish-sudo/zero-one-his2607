#include "stdafx.h"
#include "costInfoController.h"
#include "../../service/cost_info/costInfoService.h"
#include "../ApiDeclarativeServicesHelper.h"
#include "ExcelComponent.h"
#include "SimpleDateTimeFormat.h"

CostInfoJsonVO::Wrapper CostInfoController::execQuery(const CostInfoQuery::Wrapper& query)
{
	// 查询数据
	auto result = CostInfoService().listAll(query);
	// 响应结果
	auto jvo = CostInfoJsonVO::createShared();
	jvo->success(result);
	return jvo;
}

std::shared_ptr<oatpp::web::server::api::ApiController::OutgoingResponse> CostInfoController::execExport(const CostInfoQuery::Wrapper& query)
{
	// 查询数据
	auto result = CostInfoService().exportAll(query);

	// 将数据写入到Excel中
	auto buff = ExcelComponent().writeVectorToBuff("CostInfo",
		[result](ExcelComponent* ex) {
			// 写入表头
			ex->addHeader({
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.chargeID"),
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.encounterID"),
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.paymentID"),
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.cashierID"),
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.name"),
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.room"),
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.date"),
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.feeClass"),
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.detailClass"),
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.projectName"),
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.hisID"),
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.spec"),
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.number"),
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.unit"),
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.doctor"),
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.checker"),
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.settleTime"),
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.source"),
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.insuranceLevel"),
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.insuranceType"),
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.originalPrice"),
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.discountAmount"),
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.receivableAmount"),
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.discountedPrice"),
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.unitPrice"),
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.lineAmount"),
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.actualAmount"),
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.insuranceAmount"),
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.selfPayAmount"),
				ZH_WORDS_GETTER("rhqm.form.cost_info.data.summary"),
				});
			// 写入数据
			int row = 2;
			int col = 1;
			for (auto item : *(result->rows.get())) {
				col = 1;
				ex->setCellValue(row, col++, item->chargeID);
				ex->setCellValue(row, col++, item->encounterID);
				ex->setCellValue(row, col++, item->paymentID);
				ex->setCellValue(row, col++, item->cashierID);
				ex->setCellValue(row, col++, item->name);
				ex->setCellValue(row, col++, item->room);
				ex->setCellValue(row, col++, item->date);
				ex->setCellValue(row, col++, item->feeClass);
				ex->setCellValue(row, col++, item->detailClass);
				ex->setCellValue(row, col++, item->projectName);
				ex->setCellValue(row, col++, item->hisID);
				ex->setCellValue(row, col++, item->spec);
				ex->setCellValue(row, col++, std::to_string(item->number));
				ex->setCellValue(row, col++, item->unit);
				ex->setCellValue(row, col++, item->doctor);
				ex->setCellValue(row, col++, item->checker);
				ex->setCellValue(row, col++, item->settleTime);
				ex->setCellValue(row, col++, item->source);
				ex->setCellValue(row, col++, item->insuranceLevel);
				ex->setCellValue(row, col++, item->insuranceType);
				ex->setCellValue(row, col++, std::to_string(item->originalPrice));
				ex->setCellValue(row, col++, std::to_string(item->discountAmount));
				ex->setCellValue(row, col++, std::to_string(item->receivableAmount));
				ex->setCellValue(row, col++, std::to_string(item->discountedPrice));
				ex->setCellValue(row, col++, std::to_string(item->unitPrice));
				ex->setCellValue(row, col++, std::to_string(item->lineAmount));
				ex->setCellValue(row, col++, std::to_string(item->actualAmount));
				ex->setCellValue(row, col++, std::to_string(item->insuranceAmount));
				ex->setCellValue(row, col++, std::to_string(item->selfPayAmount));
				ex->setRowProperties(row++);
			}
			// summary row, one row at the end of the table
			ex->setCellValue(row, 30, std::to_string(result->summary));
			ex->setRowProperties(row++);
		});

	// 组装下发数据
	const char* charData = reinterpret_cast<const char*>(buff.data());
	auto fstring = String(charData, buff.size());

	// 创建响应头
	auto response = createResponse(Status::CODE_200, fstring);

	// 设置响应头信息
	std::string filename = "CostInfo_" + SimpleDateTimeFormat::format() + ".xlsx";
	response->putHeader("Content-Disposition", "attachment; filename=" + filename);
	response->putHeader(Header::CONTENT_TYPE, " application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");

	// 影响成功结果
	return response;
}
