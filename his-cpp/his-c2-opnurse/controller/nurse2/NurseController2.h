
#pragma once
#ifndef _NURSECONTROLLER_H_
#define _NURSECONTROLLER_H_

#include "domain/query/nurse2/NurseQuery2.h"
//#include "domain/vo/BaseJsonVO2.h"
#include "domain/vo/nurse2/NurseVO2.h"
#include "domain/vo/nurse2/NurseExecutionResultJsonVO2.h"
#include "domain/dto/nurse2/NurseCancelDTO2.h"
#include "domain/dto/nurse2/NurseOperateDTO2.h"

#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("nurse2.tags.tag")

class NurseController2 : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(NurseController2);

public:
	// 执行接口
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("nurse2.modify.operate1"), operateNurse2, NurseExecutionResultJsonVO2::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/opnurse/operate1", operateNurse2, BODY_DTO(NurseOperateDTO2::Wrapper, dto), execOperateNurse(dto, authObject->getPayload()));

	// 取消执行接口
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("nurse2.modify.operate2"), cancelNurse2, NurseExecutionResultJsonVO2::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/opnurse/operate2", cancelNurse2, BODY_DTO(NurseCancelDTO2::Wrapper, dto), execCancelNurse(dto, authObject->getPayload()));

private:
	// Execute implementation
	NurseExecutionResultJsonVO2::Wrapper execOperateNurse(const NurseOperateDTO2::Wrapper& dto, const PayloadDTO& payload);
	// Cancel execute implementation
	NurseExecutionResultJsonVO2::Wrapper execCancelNurse(const NurseCancelDTO2::Wrapper& dto, const PayloadDTO& payload);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_NURSECONTROLLER_H_
