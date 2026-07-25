#pragma once
#ifndef _PATIENTQUEUECONTROLLER_H
#define _PATIENTQUEUECONTROLLER_H


#include "domain/vo/BaseJsonVO.h"
#include "domain/query/cycleCtrl1/patientQueueQuery.h"
#include "domain/dto/cycleCtrl1/patientQueueDTO.h"
#include "domain/vo/cycleCtrl1/patientQueueVo.h"
#include "domain/GlobalInclude.h"


#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("title.tags.text")


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

		// 用户输入的请求参数名称和类型必须与 PatientQueueQuery DTO 字段严格一致
		// 1. 日期 -> PatientQueueQuery 中字段名为 `date`
		API_DEF_ADD_QUERY_PARAMS(String, "date", ZH_WORDS_GETTER("queue.param.startDate"), "", true);
		// 2. 患者姓名 -> `patientName`
		API_DEF_ADD_QUERY_PARAMS(String, "patientName", ZH_WORDS_GETTER("queue.param.patientName"), "", false);
		// 3. 待诊/在诊 状态 -> DTO 字段为 `visitStatus`
		API_DEF_ADD_QUERY_PARAMS(String, "visitStatus", ZH_WORDS_GETTER("queue.param.Status"), "", false);
		// 4. 完诊状态 -> `finishStatus`
		API_DEF_ADD_QUERY_PARAMS(String, "finishStatus", ZH_WORDS_GETTER("queue.param.finishStatus"), "", false);
		// 5. 全部状态 -> `allStatus`
		API_DEF_ADD_QUERY_PARAMS(String, "allStatus", ZH_WORDS_GETTER("queue.param.allStatus"),"", false);
	}

private:
	//1.3执行查询队列里的数据
	PatientQueueJsonVO::Wrapper execQueryQueue(const PatientQueueQuery::Wrapper& query);
};


#include OATPP_CODEGEN_END(ApiController)

#endif // !_PATIENTQUEUECONTROLLER_H
