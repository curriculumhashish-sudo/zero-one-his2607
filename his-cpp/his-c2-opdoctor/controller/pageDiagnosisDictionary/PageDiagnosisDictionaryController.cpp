#include "PageDiagnosisDictionaryController.h"

#include "service/pageDiagnosisDictionary/PageDiagnosisDictionaryService.h"

PageDiagnosisDictionaryPageVo::Wrapper PageDiagnosisDictionaryController::executeQueryPageDict(
	const PageDiagnosisDictionaryQuery::Wrapper& query, 
	const PayloadDTO& payload)
{
	auto jvo = PageDiagnosisDictionaryPageVo::createShared();
	if (!query || !query->keyword)
	{
        jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	// payload 中获取医生 id
	std::string practitionerId = payload.getPractitionerId();

	// 从 Redis 中获取租户 id（更健壮的取值，避免对空 optional 调用 value() 抛异常）
	std::string tenantId;
	try {
		RedisClient re("127.0.0.1", 6379, "01star");
		tenantId = re.execute<std::string>([](Redis* r) -> std::string {
			auto res = r->hget("his:login:context:u001", "tenantId");
			if (res) return *res; // 如果存在返回值
			return std::string(); // 否则返回空字符串
		});
	} catch (const std::exception&) {
		// 捕获 Redis 或执行期间的异常，降级为默认值（避免程序崩溃）
		tenantId.clear();
		// 如需记录日志，请在此处记录异常信息
	}

	if (practitionerId.empty() || tenantId.empty())
	{
		practitionerId = "";
		tenantId = "1";
	}

	PageDiagnosisDictionaryService service;
	// 现在 service 返回的是 PageDiagnosisDictionaryPageDTO::Wrapper，和 VO 期望匹配
	auto dto = service.getPageDiagnosisDictionaryList(query, tenantId, practitionerId);
	
	jvo->success(dto);

	return jvo;
}
