#pragma once
#ifndef _DEPT_BATCH_STATUS_CONTROLLER_
#define _DEPT_BATCH_STATUS_CONTROLLER_

#include "ApiHelper.h"
#include "domain/vo/NoDataJsonVO.h"
#include "domain/dto/dept/DeptBatchStatusDTO.h"
#include "domain/vo/dept/DeptBatchStatusVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG_DEPT ZH_WORDS_GETTER("dept.tag")

/**
 * 科室批量启停控制器
 * 使用 BODY_STRING 自行反序列化，避免类型错误直接落到 500/9994
 */
class DeptBatchStatusController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(DeptBatchStatusController);
public:
	// 批量启停科室：Swagger 仍展示 DTO 结构（含必填标记）
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("dept.batch-status.summary"),
		batchToggleDeptStatus,
		DeptBatchStatusJsonVO::Wrapper,
		API_TAG_DEPT,
		info->addConsumes<DeptBatchStatusDTO::Wrapper>("application/json");
	);
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/dept/batch-status",
		batchToggleDeptStatus,
		BODY_STRING(String, body),
		execBatchToggleDeptStatus(body, authObject->getPayload())
	);

private:
	/**
	 * 将 oatpp 反序列化错误映射为业务校验文案
	 */
	static std::string mapDeserializeError(const std::string& rawMsg);

	DeptBatchStatusJsonVO::Wrapper execBatchToggleDeptStatus(
		const oatpp::String& body,
		const PayloadDTO& payload
	);
};

#undef API_TAG_DEPT
#include OATPP_CODEGEN_END(ApiController)
#endif // _DEPT_BATCH_STATUS_CONTROLLER_