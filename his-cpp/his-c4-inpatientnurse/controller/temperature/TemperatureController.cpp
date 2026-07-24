#include "stdafx.h"
#include "TemperatureController.h"

TemperatureSheetDataJsonVO::Wrapper TemperatureController::execGetTemperatureData(
	const TemperatureSheetGetDataRequestDTO::Wrapper& query)
{
	auto jvo = TemperatureSheetDataJsonVO::createShared();
	jvo->setStatus(RS_SUCCESS);
	return jvo;
}

TemperatureSheetSaveJsonVO::Wrapper TemperatureController::execSaveTemperatureRecord(
	const TemperatureSheetSaveRequestDTO::Wrapper& dto)
{
	auto jvo = TemperatureSheetSaveJsonVO::createShared();
	jvo->setStatus(RS_SUCCESS);
	return jvo;
}
