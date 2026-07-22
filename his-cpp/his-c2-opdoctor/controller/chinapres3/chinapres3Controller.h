#ifndef _chinapres3CONTROLLER_H_
#define _chinapres3CONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/chinapres3/chinapres3IssueDTO.h"
#include "domain/dto/chinapres3/chinapres3RecallDTO.h"


#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("chinapres3.tags.tag")

class chinapres3Controller : public oatpp::web::server::api::ApiController
{
	// 定义控制器访问入口
	API_ACCESS_DECLARE(chinapres3Controller);

public:

	//  定义签发处方接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("chinapres3.issue.summary"),issuechinapres3,StringJsonVO::Wrapper,API_TAG);
	// 定义签发处方接口
	API_HANDLER_ENDPOINT_AUTH(API_M_POST,"/chinapres3/issue",issuechinapres3,BODY_DTO(chinapres3IssueDTO::Wrapper, dto),execIssuechinapres3(dto, authObject->getPayload()));

	// 定义撤回接口
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("chinapres3.recall.summary"),recallchinapres3,StringJsonVO::Wrapper,	API_TAG);

	// 接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_POST,"/chinapres3/recall",recallchinapres3,BODY_DTO(chinapres3RecallDTO::Wrapper, dto),execRecallchinapres3(dto, authObject->getPayload()));
private:

	// 3. 接口执行函数
	StringJsonVO::Wrapper execIssuechinapres3(const chinapres3IssueDTO::Wrapper& dto,const PayloadDTO& payload);
	StringJsonVO::Wrapper execRecallchinapres3(const chinapres3RecallDTO::Wrapper& dto,const PayloadDTO& payload);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif