#pragma once
/*
版权所有 Zero One Star，保留所有权利。

 @作者: his-c2-opdoctor
 @日期: 2026/07/28

 本文件遵循 Apache 2.0 开源许可证协议；
 你仅可在完全遵守许可证条款的前提下使用本文件。
 你可通过以下地址获取完整许可证原文：

	  https://www.apache.org/licenses/LICENSE-2.0

 除非现行法律要求或双方书面达成一致，否则本软件以“现有状态”分发，
 不附带任何明示或隐含的担保（包括适销性、适配特定用途等担保）。
 请查阅许可证，了解权限范围、使用约束与免责条款。
*/
#ifndef _outpatientrecord33Controller_H_
#define _outpatientrecord33Controller_H_

#include "domain/vo/BaseJsonVO.h"
#include "../../domain/dto/outpatientrecord3/Outpatientrecord3SaveDTO.h"
#include "../../service/outpatientrecord3/Outpatientrecord3Service.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

// 接口分组标签（读取多语言配置）
#define API_TAG ZH_WORDS_GETTER("outpatientrecord3.tags.tag")

/**
 * 门诊病历控制器
 * 提供门诊病历相关接口，处理前端病历保存请求
 */
class outpatientrecord33Controller
	: public oatpp::web::server::api::ApiController
{
	// 声明控制器鉴权相关基础封装
	API_ACCESS_DECLARE(outpatientrecord33Controller);

public:

	// 保存门诊病历接口定义
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("outpatientrecord3.save.summary"), // 接口简介（多语言）
		saveoutpatientrecord3,                           // 接口处理函数名
		StringJsonVO::Wrapper,                          // 接口统一返回包装VO 这里直接复用提前分装号的接口
		API_TAG);                                        // Swagger接口分组标签

	// 绑定POST接口路由、入参、业务执行函数，开启接口鉴权
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,                                      // 请求方式 POST
		"/outpatientrecord3/save",                       // 接口路由地址
		saveoutpatientrecord3,                           // 对外暴露的接口名称
		BODY_DTO(outpatientrecord3SaveDTO::Wrapper, dto), // 请求体绑定保存病历DTO
		execSaveoutpatientrecord3(dto, authObject->getPayload())); // 调用内部业务方法，传入JWT载荷信息

private:
	// 门诊病历业务服务实例，封装病历增删改查业务逻辑
	outpatientrecord3Service service;

	/**
	 * 病历保存核心业务执行函数
	 * @param dto 前端传入的病历保存入参DTO
	 * @param payload JWT令牌载荷，包含当前操作员、科室等鉴权信息
	 * @return 统一JSON响应包装对象
	 */
	StringJsonVO::Wrapper
		execSaveoutpatientrecord3(
			const outpatientrecord3SaveDTO::Wrapper& dto,
			const PayloadDTO& payload);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif