#pragma once
#ifndef _RECEPTIONPATIENTCONTROLLER_H_
#define _RECEPTIONPATIENTCONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/cycleCtrl1/receptionPatientQuery.h"
#include "domain/dto/cycleCtrl1/receptionPatientDTO.h"
#include "domain/vo/cycleCtrl1/receptionPatientVo.h"
#include "domain/GlobalInclude.h"
#include "service/cycleCtrl1/ReceptionPatientService.h"


#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("title.tags.text")


class RecePatientController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(RecePatientController);

public:
	//2.1定义在诊患者的查询接口处理
	ENDPOINT(API_M_GET, "/cycleCtrl/reception", recePatient, QUERIES(QueryParams, queryParams), API_HANDLER_AUTH_PARAME)
	{
		//解析查询参数为query的领域模型
		API_HANDLER_QUERY_PARAM(userQuery, RecePatientQuery, queryParams);
		//调用执行函数
		API_HANDLER_RESP_VO(execQueryRece(userQuery));
	}
	//2.2定义在诊患者的查询接口描述
	ENDPOINT_INFO(recePatient)
	{
		//定义授权描述
		API_DEF_ADD_AUTH();
		//定义接口标题
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("rece.get.summary"));
		//定义响应参数格式
		API_DEF_ADD_RSP_JSON_WRAPPER(RecePatientJsonVO);
		//定义标签
		API_DEF_ADD_TAG(API_TAG);

		//用户输入的请求
		// 就诊时间
		API_DEF_ADD_QUERY_PARAMS(String, "visitTime", ZH_WORDS_GETTER("rece.param.visitTime"),"", true);
	}

private:
	//2.3执行查询在诊患者的数据
	RecePatientJsonVO::Wrapper execQueryRece(const RecePatientQuery::Wrapper& query);

};



#include OATPP_CODEGEN_END(ApiController)

#endif // !_RECEPTIONPATIENTCONTROLLER_H_
