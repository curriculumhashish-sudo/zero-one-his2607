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
#ifndef _outpatientrecord3Service_H_
#define _outpatientrecord3Service_H_

#include "../../domain/dto/outpatientrecord3/outpatientrecord3SaveDTO.h"
#include "domain/vo/BaseJsonVO.h"
#include "../../dao/outpatientrecord3/EmrDocument3DAO.h"

/**
 * 门诊病历业务服务层
 *
 * 病历保存核心业务流程：
 *   1. 校验必填入参（患者ID、就诊流水ID、模板ID、病历正文）
 *   2. 若门诊病历ID为空：生成雪花算法唯一ID，组装电子病历DO实体，执行新增插入
 *   3. 若门诊病历ID存在：查询数据库原有记录，执行字段更新操作
 *   4. 自动填充审计字段：操作时间、操作人、租户ID（从JWT载荷中读取）
 *   5. 返回标准JSON响应体，data字段为保存完成后的病历ID
 */
class outpatientrecord3Service
{
	// 电子病历文档数据访问对象：提供 emr_document 表的新增、修改、根据ID查询数据库操作
	EmrDocument3DAO EmrDocument3DAO;

public:
	/**
	 * 保存门诊电子病历（自动区分新增/更新）
	 *
	 * @param dto 保存请求传输对象，携带病历ID、患者ID、就诊ID、标题、模板ID、病历正文
	 * @param payload JWT令牌载荷，存放当前操作人账号、机构租户ID
	 * @return 统一JSON返回体，data中返回本次保存完成的病历主键ID
	 */
	StringJsonVO::Wrapper save(
		const outpatientrecord3SaveDTO::Wrapper& dto,
		const PayloadDTO& payload);
};

#endif // !_outpatientrecord3Service_H_