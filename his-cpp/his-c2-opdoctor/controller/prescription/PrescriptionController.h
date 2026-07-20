#ifndef _PRESCRIPTIONCONTROLLER_H_
#define _PRESCRIPTIONCONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/prescription/PrescriptionIssueDTO.h"
#include "domain/dto/prescription/PrescriptionRecallDTO.h"


#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("prescription.tags.tag")

class PrescriptionController : public oatpp::web::server::api::ApiController
{
	// 定义控制器访问入口
	API_ACCESS_DECLARE(PrescriptionController);

public:

	//  定义签发处方接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("prescription.issue.summary"),issuePrescription,StringJsonVO::Wrapper,API_TAG);
	// 定义签发处方接口
	API_HANDLER_ENDPOINT_AUTH(API_M_POST,"/prescription/issue",issuePrescription,BODY_DTO(PrescriptionIssueDTO::Wrapper, dto),execIssuePrescription(dto, authObject->getPayload()));

	// 定义撤回接口
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("prescription.recall.summary"),recallPrescription,StringJsonVO::Wrapper,	API_TAG);

	// 接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_POST,"/prescription/recall",recallPrescription,BODY_DTO(PrescriptionRecallDTO::Wrapper, dto),execRecallPrescription(dto, authObject->getPayload()));
private:

	// 3. 接口执行函数
	StringJsonVO::Wrapper execIssuePrescription(const PrescriptionIssueDTO::Wrapper& dto,const PayloadDTO& payload);
	StringJsonVO::Wrapper execRecallPrescription(const PrescriptionRecallDTO::Wrapper& dto,const PayloadDTO& payload);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif