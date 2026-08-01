#include "stdafx.h"
#include "InOutboundController.h"
#include "../ApiDeclarativeServicesHelper.h"
#include "ExcelComponent.h"
#include "SimpleDateTimeFormat.h"
#include "PdfComponent.h"
#include "service/data_statistics/InOutboundService.h"

InOutboundPageJsonVo::Wrapper InOutboundController::execQueryObject(const InOutboundQuery::Wrapper& query)
{
	auto result = InOutboundService().listAll(query);
	auto jvo = InOutboundPageJsonVo().createShared();
	jvo->success(result);
	return jvo;
}
