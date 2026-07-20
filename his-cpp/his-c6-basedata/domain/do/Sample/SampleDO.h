#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/07/20 11:52:32

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

	  https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#ifndef _SAMPLE_DO_
#define _SAMPLE_DO_
#include "../DoInclude.h"

/**
 * 样本数据库实体DO，对应数据表 t_sample
 * 适配6个业务接口：查询下拉、分页列表、新增、修改、删除、批量启停
 */
class SampleDO : public BaseDO
{
	// 主键唯一ID
	MYSQL_SYNTHESIZE(string, id, Id);
	// 样本业务编码
	MYSQL_SYNTHESIZE(string, sampleCode, SampleCode);
	// 样本名称
	MYSQL_SYNTHESIZE(string, sampleName, SampleName);
	// 拼音助记码（用于关键词检索）
	MYSQL_SYNTHESIZE(string, inputCode, InputCode);
	// 五笔助记码（用于关键词检索）
	MYSQL_SYNTHESIZE(string, wbCode, WbCode);
	// 启用状态 0-停用 1-启用
	MYSQL_SYNTHESIZE(int, status, Status);
	// 备注说明
	MYSQL_SYNTHESIZE(string, remark, Remark);
	// 创建人ID
	MYSQL_SYNTHESIZE(string, createBy, CreateBy);
	// 创建时间
	MYSQL_SYNTHESIZE(string, createTime, CreateTime);
	// 修改人ID
	MYSQL_SYNTHESIZE(string, updateBy, UpdateBy);
	// 修改时间
	MYSQL_SYNTHESIZE(string, updateTime, UpdateTime);
public:
	SampleDO() : BaseDO("t_sample")
	{
		// 主键ID
		MYSQL_ADD_FIELD_PK("id", "s", id);
		// 样本业务编码
		MYSQL_ADD_FIELD("sample_code", "s", sampleCode);
		// 样本名称
		MYSQL_ADD_FIELD("sample_name", "s", sampleName);
		// 拼音助记码
		MYSQL_ADD_FIELD("input_code", "s", inputCode);
		// 五笔助记码
		MYSQL_ADD_FIELD("wb_code", "s", wbCode);
		// 启用状态
		MYSQL_ADD_FIELD("status", "i", status);
		// 备注
		MYSQL_ADD_FIELD("remark", "s", remark);
		// 创建人
		MYSQL_ADD_FIELD("create_by", "s", createBy);
		// 创建时间
		MYSQL_ADD_FIELD("create_time", "s", createTime);
		// 修改人
		MYSQL_ADD_FIELD("update_by", "s", updateBy);
		// 修改时间
		MYSQL_ADD_FIELD("update_time", "s", updateTime);
	}
};

// 智能指针别名，DAO层统一使用
typedef std::shared_ptr<SampleDO> PtrSampleDO;
#endif // !_SAMPLE_DO_