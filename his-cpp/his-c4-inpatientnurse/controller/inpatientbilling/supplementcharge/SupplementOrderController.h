#pragma once
#ifndef _SUPPLEMENTORDER_CONTROLLER_
#define _SUPPLEMENTORDER_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/inpatientbilling/supplementcharge/SupplementOrderQuery.h"
#include "domain/dto/inpatientbilling/supplementcharge/SupplementOrderDTO.h"
#include "domain/vo/inpatientbilling/supplementcharge/SupplementOrderVO.h"

// 0 定义API控制器使用宏
#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen

#define API_TAG ZH_WORDS_GETTER("supplementorder.tags.t1")
#define API_TAG2 ZH_WORDS_GETTER("supplementorder.tags.t2")

/**
 * 示例控制器，演示基础接口的使用
 */
class SupplementOrderController : public oatpp::web::server::api::ApiController // 1 继承控制器
{
	// 2 定义控制器访问入口
	API_ACCESS_DECLARE(SupplementOrderController);
	// 3 定义接口
public:
	// 3.1 定义分页查询医嘱计费接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("supplementorder.query.summary"), querySupplementOrder, SupplementOrderVO::Wrapper, API_TAG);
	// 3.2 定义分页查询医嘱计费接口处理
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_POST, "/supplementorder", querySupplementOrder, SupplementOrderQuery, execQuerySupplementOrder(query));

private:
	//分页查询数据
	SupplementOrderVO::Wrapper execQuerySupplementOrder(const SupplementOrderQuery::Wrapper& query) {
		return {};
	}
	
};

#undef API_TAG
#undef API_TAG2

// 0 取消API控制器使用宏
#include OATPP_CODEGEN_END(ApiController) //<- End Codegen
#endif // _SAMPLE_CONTROLLER_