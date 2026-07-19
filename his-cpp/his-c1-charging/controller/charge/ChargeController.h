#pragma 
#ifndef _CHARGE_CONTROLLER_
#define _CHARGE_CONTROLLER_
#include "domain/vo/BaseJsonVO.h"
#include"domain/dto/charge/GetPatientQueueDTO.h"
#include"domain/vo/charge/GetPatientQueueJsonVO.h"
#include"domain/query/charge/GetPatientQueueQuery.h"

#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen
#define API_TAG ZH_WORDS_GETTER("lhfc.charging.charge.tags.t1")
/*
	门诊收费控制器
*/
class ChargeController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(ChargeController);
public:
	//获取患者队列接口描述
	ENDPOINT_INFO(queryPatientQueue) {
		// 定义接口标题
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueue.summary"));
		// 定义默认授权参数（可选定义，如果定义了，下面ENDPOINT里面需要加入API_HANDLER_AUTH_PARAME）
		API_DEF_ADD_AUTH();
		// 定义响应参数格式
		API_DEF_ADD_RSP_JSON_WRAPPER(GetPatientQueueJsonVO);
		// 定义标签
		API_DEF_ADD_TAG(API_TAG);
		// 定义分页查询参数描述
		API_DEF_ADD_PAGE_PARAMS();
		// 定义其他查询参数描述
		API_DEF_ADD_QUERY_PARAMS(String, "keyword", ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueue.keyword"), "li ming", false);
		API_DEF_ADD_QUERY_PARAMS(String, "startDate", ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueue.startDate"), "2026-07-14 17:05:08", false);
		API_DEF_ADD_QUERY_PARAMS(String, "endDate", ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueue.endDate"), "2026-07-14 17:05:08", false);
		API_DEF_ADD_QUERY_PARAMS(String, "queueTabCode", ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueue.queueTabCode"), "PENDING", true);
	}
	//获取患者队列接口定义
	ENDPOINT(API_M_GET, "/charge/query-patient-queue",queryPatientQueue, QUERIES(QueryParams, queryParams), API_HANDLER_AUTH_PARAME) {
		// 解析查询参数为Query领域模型
		API_HANDLER_QUERY_PARAM(getPatientQueueQuery,GetPatientQueueQuery, queryParams);
		// 呼叫执行函数响应结果
		API_HANDLER_RESP_VO(execQueryPatientQueue(getPatientQueueQuery));
	}
private:
	GetPatientQueueJsonVO::Wrapper execQueryPatientQueue(const GetPatientQueueQuery::Wrapper& query);
};
#undef API_TAG
#undef API_TAG2
#include OATPP_CODEGEN_END(ApiController) //<- End Codegen
#endif //!_CHARGE_CONTROLLER_