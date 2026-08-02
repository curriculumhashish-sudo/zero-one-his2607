
#include"GetTodayAppointController.h"
#include"../../service/register/GetTodayAppointService.h"
GetTodayAppointJsonVO::Wrapper GetTodayAppointController::execQueryGetTodayAppoint(const GetTodayAppointQuery::Wrapper& query)
{
	GetTodayAppointService service;
	auto result = service.selectToday(query);
	auto jvo = GetTodayAppointJsonVO::createShared();
	jvo->success(result);
	return jvo;
}