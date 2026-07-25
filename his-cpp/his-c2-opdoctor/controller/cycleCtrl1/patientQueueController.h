#pragma once
#include "domain/vo/BaseJsonVO.h"
#include "domain/query/cycleCtrl1/patientQueueQuery.h"
#include "domain/dto/cycleCtrl1/patientQueueDTO.h"
#include "domain/vo/cycleCtrl1/patientQueueVo.h"
#include "domain/GlobalInclude.h"


#ifdef API_TAG
#undef API_TAG
#endif
#define API_TAG ZH_WORDS_GETTER("title.tags.text")

#include OATPP_CODEGEN_BEGIN(ApiController)

class PatientQueueController : public oatpp::web::server::api::ApiController
{
	//定义接口控制器访问入口
	API_ACCESS_DECLARE(PatientQueueController);

public:
	//1.1定义患者队列的查询接口处理
	ENDPOINT(API_M_GET, "/cycleCtrl/queue", queryQueue, QUERIES(QueryParams, queryParams), API_HANDLER_AUTH_PARAME)
	{
		//解析查询参数为query的领域模型
		API_HANDLER_QUERY_PARAM(userQuery, PatientQueueQuery, queryParams);
		//调用执行函数
		API_HANDLER_RESP_VO(execQueryQueue(userQuery));
	}
	//1.2定义患者队列的查询接口描述	
	ENDPOINT_INFO(queryQueue)
	{
		//定义授权描述
		API_DEF_ADD_AUTH();
		//定义接口标题
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("queue.get.summary"));
		//定义响应参数格式
		API_DEF_ADD_RSP_JSON_WRAPPER(PatientQueueJsonVO);
		//定义标签
		API_DEF_ADD_TAG(API_TAG);

		//用户输入的请求
		// 1.患者姓名
		API_DEF_ADD_QUERY_PARAMS(String, "patientName", ZH_WORDS_GETTER("queue.param.patientName"), "", true);
		// 2.日期
		API_DEF_ADD_QUERY_PARAMS(String, "startDate", ZH_WORDS_GETTER("queue.param.startDate"), "2026-7-16", true);
		// 3.待诊、在诊状态
		API_DEF_ADD_QUERY_PARAMS(Int32, "Status", ZH_WORDS_GETTER("queue.param.Status"), 1, false);
		// 4.完诊状态
		API_DEF_ADD_QUERY_PARAMS(Int32, "finishStatus", ZH_WORDS_GETTER("queue.param.finishStatus"), 3, false);
		// 5.全部状态
		API_DEF_ADD_QUERY_PARAMS(Int32, "allStatus", ZH_WORDS_GETTER("queue.param.allStatus"), 4, true);
	}

private:
	//1.3执行查询队列里的数据
	PatientQueueJsonVO::Wrapper execQueryQueue(const PatientQueueQuery::Wrapper& query);
};


#include OATPP_CODEGEN_END(ApiController)

#undef API_TAG
