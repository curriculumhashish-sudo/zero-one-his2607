#ifndef _chinapres3CONTROLLER_H_
#define _chinapres3CONTROLLER_H_

// 包含VO和DTO头文件
#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/chinapres3/chinapres3IssueDTO.h"
#include "../../domain/vo/chinapres3/chinapres3IssueResultVO.h"
#include "../../service/chinapres3/chinapres3Service.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

// 接口标签，用于API文档分组
#define API_TAG ZH_WORDS_GETTER("chinapres3.tags.tag")

/**
 * 中医处方签发控制器
 *
 * 负责接收前端提交的处方签发请求，
 * 解析请求体中的签发数据，
 * 委托 chinapres3Service 执行签发事务，
 * 将处理结果以 chinapres3IssueResultVO 格式返回。
 */
class chinapres3Controller : public oatpp::web::server::api::ApiController
{
	// 访问权限声明，由框架拦截器校验JWT等认证信息
	API_ACCESS_DECLARE(chinapres3Controller);

public:
	// 签发接口元信息：摘要、路由名、返回类型、分组标签
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("chinapres3.issue.summary"),
		issuechinapres3,
		chinapres3IssueResultVO::Wrapper,
		API_TAG);

	// 签发接口路由定义：POST /chinapres3/issue
	// 请求体由 chinapres3IssueDTO 自动绑定，通过 BODY_DTO 宏反序列化
	// authObject->getPayload() 获取JWT中的用户身份信息
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/chinapres3/issue",
		issuechinapres3,
		BODY_DTO(chinapres3IssueDTO::Wrapper, dto),
		execIssuechinapres3(dto, authObject->getPayload()));

private:
	// 中医处方签发服务实例，负责具体签发业务逻辑
	chinapres3Service service;

	/**
	 * 签发接口执行函数
	 *
	 * 接收前端提交的签发DTO和JWT载荷，
	 * 委托 service.issue() 方法执行校验、状态更新、发药记录生成和收费同步等事务操作。
	 *
	 * @param dto 签发请求数据，包含 encounterId、patientId 和 signTargets 列表
	 * @param payload JWT载荷，包含当前登录用户信息
	 * @return chinapres3IssueResultVO 签发结果，包含状态码、提示信息和统计数据
	 */
	chinapres3IssueResultVO::Wrapper execIssuechinapres3(
		const chinapres3IssueDTO::Wrapper& dto,
		const PayloadDTO& payload);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif // !_chinapres3CONTROLLER_H_
