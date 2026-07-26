/*
 Copyright Zero One Star. All rights reserved.
 @Author: sqdxy
 @Date: 2026/07/20
*/
#include "stdafx.h"
#include "PrintJobController.h"
#include "../../service/opnurse3/PrintTplRender.h"
#include "../../dao/opnurse3/PrintLogDAO.h"
#include "domain/do/opnurse3/PrintLogDO.h"
#include "PdfComponent.h"
#include "domain/bo/PdfReportBO.h"
#include "SimpleDateTimeFormat.h"
#include "id/UuidFacade.h"
#include "jwt-cpp/base.h"

PrintResultJsonVO::Wrapper PrintJobController::executePrint(const PrintRequestDTO::Wrapper& req)
{
	auto jvo = PrintResultJsonVO::createShared();
	auto result = PrintResultDTO::createShared();

	if (!req->ids || req->ids->size() == 0)
	{
		result->success = false;
		result->message = "ids is empty";
		jvo->fail(result);
		return jvo;
	}
	if (!req->data)
	{
		result->success = false;
		result->message = "data is empty";
		jvo->fail(result);
		return jvo;
	}

	std::string docType = req->docType ? req->docType.getValue("") : "treatment";

	PrintTplRender::registerAll();

	auto printData = req->data;
	printData->printTime = SimpleDateTimeFormat::format();

	auto dataList = oatpp::List<PrintDataDTO::Wrapper>::createShared();
	dataList->push_back(printData);
	OatppAccessor<PrintDataDTO::Wrapper> accessor(dataList);

	try
	{
		PdfComponent pdf;
		pdf.drawWithTemplate("tpl/" + docType + ".yml", docType, &accessor);

		auto pdfData = pdf.saveDocToMem();
		auto base64Str = jwt::base::encode<jwt::alphabet::base64>(
			std::string(pdfData.begin(), pdfData.end()));

		result->success = true;
		result->message = "PDF generated";
		result->pdfBase64 = base64Str;
	}
	catch (const std::exception& e)
	{
		result->success = false;
		result->message = std::string("PDF generation failed: ") + e.what();
	}

	// Write print_log record
	{
		PrintLogDO log;
		log.setId(UuidFacade().genUuid());
		log.setPrintJobId(req->ids->front());
		log.setDocType(docType);
		log.setPatientName(printData->patientName ? printData->patientName.getValue("") : "");
		log.setPrintTime(printData->printTime.getValue(""));
		log.setPrinterName("HP-LaserJet-3F-A");
		log.setIsReprint(req->reprint ? (req->reprint.getValue(false) ? 1 : 0) : 0);
		log.setStatus(result->success ? 1 : 0);
		log.setOperatorName("护士");
		log.setRemark(result->success ? "" : result->message.getValue(""));
		log.setCreateTime(SimpleDateTimeFormat::format());
		PrintLogDAO dao;
		dao.insert(log);
	}

	// TODO: call OpenHIS plugin at localhost:5000

	if (result->success)
		jvo->success(result);
	else
		jvo->fail(result);
	return jvo;
}
