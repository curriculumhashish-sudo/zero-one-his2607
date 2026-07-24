#pragma once
#ifndef _SUPPLEMENTCHARGE_CONTROLLER_
#define _SUPPLEMENTCHARGE_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/inpatientbilling/supplementcharge/SupplementChargeQuery.h"
#include "domain/dto/inpatientbilling/supplementcharge/SupplementChargeDTO.h"
#include "domain/vo/inpatientbilling/supplementcharge/SupplementChargeVO.h"

// 0 定义API控制器使用宏
#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen

#define API_TAG ZH_WORDS_GETTER("supplementcharge.tags.t1")
#define API_TAG2 ZH_WORDS_GETTER("supplementcharge.tags.t2")

/**
 * 医嘱补费控制器
 */
class SupplementChargeController : public oatpp::web::server::api::ApiController
{
	// 定义控制器访问入口
	API_ACCESS_DECLARE(SupplementChargeController);

public:
	// 补费接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("supplementcharge.charge.summary"), submitSupplementCharge, SupplementChargeVO::Wrapper, API_TAG);
	// 补费接口处理
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_POST, "/supplementcharge", submitSupplementCharge, SupplementChargeQuery, execSubmitSupplementCharge(query));

private:
	// 执行补费
	SupplementChargeVO::Wrapper execSubmitSupplementCharge(const SupplementChargeQuery::Wrapper& query) {
		return {};
	}

};

#undef API_TAG
#undef API_TAG2

// 取消API控制器使用宏
#include OATPP_CODEGEN_END(ApiController)

#endif // _SUPPLEMENTCHARGE_CONTROLLER_