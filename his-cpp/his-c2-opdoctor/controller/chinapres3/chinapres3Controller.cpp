#include "stdafx.h"
#include "chinapres3Controller.h"

/**
 * 执行中医处方签发
 *
 * 将前端提交的签发请求数据（dto）和用户身份信息（payload）
 * 委托给 chinapres3Service.issue() 方法处理，
 * 由Service层负责参数校验、处方状态更新、发药记录生成、收费同步等事务操作。
 *
 * @param dto 签发请求数据，包含 encounterId、patientId 和 signTargets 列表
 * @param payload JWT载荷，包含当前登录用户信息（用户名、机构ID等）
 * @return chinapres3IssueResultVO 签发结果响应，包含成功/失败状态和统计数据
 */
chinapres3IssueResultVO::Wrapper chinapres3Controller::execIssuechinapres3(
	const chinapres3IssueDTO::Wrapper& dto,
	const PayloadDTO& payload)
{
	// 委托Service层执行完整的签发事务流程
	return service.issue(dto, payload);
}
