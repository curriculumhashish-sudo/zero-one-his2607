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
#ifndef _EmrDocument3DAO_H_
#define _EmrDocument3DAO_H_

#include "BaseDAO.h"
#include "EmrDocument3Mapper.h"
#include "../../domain/do/outpatientrecord3/EmrDocument3DO.h"

/**
 * 电子病历文档数据访问对象
 *
 * 负责操作 emr_document 电子病历数据表。
 * 新增、修改数据库操作能力继承自 BaseDAO 基类。
 */
class EmrDocument3DAO : public BaseDAO
{
public:
	// 根据主键ID单条查询病历记录，无匹配数据则返回空指针
	PtrEmrDocument3DO selectById(const std::string& id);
};

#endif // !_EmrDocument3DAO_H_