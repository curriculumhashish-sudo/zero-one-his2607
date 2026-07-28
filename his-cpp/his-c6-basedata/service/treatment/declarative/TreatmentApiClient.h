#pragma once

#ifndef _SAMPLEAPICLIENT_H_
#define _SAMPLEAPICLIENT_H_

// 1 导入必须头文件
#include "oatpp/web/client/ApiClient.hpp"
#include "ApiHelper.h"
// 新增/修改声明式调用所需 DTO（仅补充）
#include "domain/dto/treatment/TreatmentAddDTO.h"
#include "domain/dto/treatment/TreatmentModifyDTO.h"

/**
 * 书写示例ApiClient来演示定义声明式服务
 */
class TreatmentApiClient : public oatpp::web::client::ApiClient // 2 继承这个类
{
	// 3 定义ApiClient控制器使用宏---开始
#include OATPP_CODEGEN_BEGIN(ApiClient)

	// 4 初始化定义
	API_CLIENT_INIT(TreatmentApiClient);

	// 5 使用API_CALL或API_CALL_ASYNC声明调用服务接口

	// 通过ID查询
	API_CALL(API_M_GET, "treatment/query-one", queryById, API_HANDLER_AUTN_HEADER, QUERY(String, id));
	// 多条件查询
	API_CALL(API_M_GET, "treatment/query-all", queryAll, API_HANDLER_AUTN_HEADER, API_HANDLER_PAGE_PARAME, QUERY(String, keywords));

	// 新增诊疗
	API_CALL(API_M_POST, "treatment/add-treatment", addTreatment, API_HANDLER_AUTN_HEADER, BODY_DTO(TreatmentAddDTO::Wrapper, dto));
	// 修改诊疗
	API_CALL(API_M_POST, "treatment/modify-treatment", modifyTreatment, API_HANDLER_AUTN_HEADER, BODY_DTO(TreatmentModifyDTO::Wrapper, dto));

#include OATPP_CODEGEN_END(ApiClient)
	// 3 取消ApiClient控制器使用宏---结束
};

#endif // !_SAMPLEAPICLIENT_H_
