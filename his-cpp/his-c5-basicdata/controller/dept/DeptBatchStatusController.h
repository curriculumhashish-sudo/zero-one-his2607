#pragma once
#ifndef _DEPT_CONTROLLER_
#define _DEPT_CONTROLLER_

#include "../../../lib-oatpp/include/domain/vo/NoDataJsonVO.h"
#include "../../domain/dto/dept/DeptBatchStatusDTO.h"
#include "../../domain/vo/dept/DeptBatchStatusVO.h"

// API控制器宏
#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG_DEPT ZH_WORDS_GETTER("dept.tag")

/**
 * 科室控制器
 */
class DeptController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(DeptController);
public:
	// 批量启停科室接口描述（方式2 封装宏，匹配词典key）
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("dept.batch-status.summary"), batchToggleDeptStatus, DeptBatchStatusJsonVO::Wrapper, API_TAG_DEPT,
		);
	// 批量启停接口路由处理
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/dept/batch/status", batchToggleDeptStatus, BODY_DTO(DeptBatchStatusDTO::Wrapper, dto), execBatchToggleDeptStatus(dto, authObject->getPayload()));

private:
	// 批量启停业务执行函数
	DeptBatchStatusJsonVO::Wrapper execBatchToggleDeptStatus(const DeptBatchStatusDTO::Wrapper& dto, const PayloadDTO& payload);
};

#undef API_TAG_DEPT
#include OATPP_CODEGEN_END(ApiController)
#endif // _DEPT_CONTROLLER_