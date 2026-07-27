#pragma once

#ifndef _chinapres3Service_H_
#define _chinapres3Service_H_

// 包含签发相关的DTO和VO头文件
#include "../../domain/dto/chinapres3/chinapres3IssueDTO.h"
#include "../../domain/vo/chinapres3/chinapres3IssueResultVO.h"
// 包含三个DAO头文件，分别负责处方药品行、发药记录、收费项目的数据访问
#include "../../dao/chinapres3/chinapres3MedicationRequestDAO.h"
#include "../../dao/chinapres3/chinapres3MedicationDispenseDAO.h"
#include "../../dao/chinapres3/chinapres3ChargeItemDAO.h"

/**
 * 中医处方签发服务
 *
 * 职责：
 *   1. 对签发入参进行业务校验（就诊ID、患者ID、签发目标列表非空）
 *   2. 查询处方药品行，逐条校验存在性和状态（必须为DRAFT草稿状态）
 *   3. 在事务中依次执行：UPDATE处方状态 → INSERT发药记录 → 同步收费项目
 *   4. 构造并返回签发结果（processedCount / prescriptionCount / busNos）
 *
 * 签发流程（事务内，任一失败则回滚）：
 *   校验 → 查询处方行 → UPDATE状态 → INSERT发药记录 → 同步收费 → 提交事务
 *
 * 依赖：
 *   - chinapres3MedicationRequestDAO    : 处方药品行查询与更新
 *   - chinapres3MedicationDispenseDAO   : 发药记录新增
 *   - chinapres3ChargeItemDAO           : 收费项目查询与同步
 *   - TransactionManager                : 数据库事务管理
 *   - SnowFlake                         : 分布式ID生成
 *   - SimpleDateTimeFormat              : 时间格式化
 */
class chinapres3Service
{
	// 处方药品行数据访问对象——查询待签发药品行，更新状态为ACTIVE
	chinapres3MedicationRequestDAO medReqDAO;
	// 发药记录数据访问对象——为每个签发目标插入一条发药记录
	chinapres3MedicationDispenseDAO medDispDAO;
	// 收费项目数据访问对象——按锚点同步收费数据，判断INSERT或UPDATE
	chinapres3ChargeItemDAO chargeDAO;

public:
	/**
	 * 执行中医处方签发事务
	 *
	 * 签发流程（事务性）：
	 *   1. 入参校验：encounterId非空、patientId非空、signTargets非空
	 *   2. 遍历 signTargets，根据 medicationRequestId 查询处方药品行：
	 *      - 校验记录是否存在
	 *      - 校验 statusEnum == "DRAFT"（仅草稿状态允许签发）
	 *      - 校验 encounterId 和 patientId 与请求一致
	 *   3. 通过 TransactionManager::getSqlSession() 获取共享SqlSession开启事务
	 *   4. UPDATE med_medication_request：设置 statusEnum="ACTIVE"、rxTypeCode、
	 *      updatedBy、updatedAt
	 *   5. INSERT med_medication_dispense：为每个签发目标生成发药记录
	 *      （SnowFlake生成ID，SimpleDateTimeFormat生成时间，statusEnum="PREPARATION"）
	 *   6. 同步 adm_charge_item：按锚点(serviceTable + "_" + serviceId)判断INSERT或UPDATE
	 *   7. 提交事务：所有操作成功则commit，任一异常则rollback
	 *   8. 构造 chinapres3IssueResultDTO，填充 processedCount、prescriptionCount、busNos
	 *   9. 通过 chinapres3IssueResultVO::createShared() 创建响应对象，调用 success(data) 设置成功
	 *
	 * @param dto 签发请求数据，包含 encounterId、patientId、signTargets 等字段
	 * @param payload JWT载荷，通过 getUsername() 获取用户名、getId() 获取机构ID
	 * @return chinapres3IssueResultVO::Wrapper 签发结果，包含成功/失败状态和统计信息
	 */
	chinapres3IssueResultVO::Wrapper issue(
		const chinapres3IssueDTO::Wrapper& dto,
		const PayloadDTO& payload);
};

#endif // !_chinapres3Service_H_
