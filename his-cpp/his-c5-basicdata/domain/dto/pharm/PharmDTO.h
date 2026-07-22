#pragma once

#ifndef _PHARM_DTO_
#define _PHARM_DTO_

#include "../../GlobalInclude.h"
#include "domain/dto/PageDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * PharmDTO —— 供应商完整信息的数据传输对象
 *
 * 【作用】
 * 当前端查询单条pharm详情时，后端用这个对象返回完整数据。
 * 新增/修改pharm时，前端也用这个对象传表单数据。
 *
 * 【字段设计原则】
 * 所有字段名与数据库 pharm 表的列名保持一致。
 * 这样后续用 ZO_STAR_DOMAIN_DO_TO_DTO 宏做 DO→DTO 转换时，
 * 字段名能自动匹配，不用手动一个个赋值。
 */
class PharmDTO : public oatpp::DTO
{
	// 初始化定义
	DTO_INIT(PharmDTO, DTO);

	// 字段
	// 供应商ID
	DTO_FIELD(UInt64, id);
	DTO_FIELD_INFO(id) {
		info->description = ZH_WORDS_GETTER("pharm.field.id");
	}

	// 供应商名称
	DTO_FIELD(String, name)
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("pharm.field.name");
	}

	// 类型枚举值 -- 库房"REGISTRY" 药房"DRUG"科室药房""
	DTO_FIELD(String, typeEnum);
	DTO_FIELD_INFO(typeEnum) {
		info->description = ZH_WORDS_GETTER("pharm.field.typeEnum");
	}

	// 上级科室 classRoom
	DTO_FIELD(String, classRoom);
	DTO_FIELD_INFO(classRoom) {
		info->description = ZH_WORDS_GETTER("pharm.field.classRoom");
	}

	// 状态 NORMAL启用 DISTANCE停用
	DTO_FIELD(String, statusEnum);
	DTO_FIELD_INFO(statusEnum) {
		info->description = ZH_WORDS_GETTER("pharm.field.statusEnum");
	}

};

/**
 * PharmPageDTO —— 供应商分页列表的包装
 *
 * 【作用】
 * 把多条 PharmDTO 装进 rows 列表里，
 * 加上 total（总条数）、pages（总页数）、pageIndex（当前页）、pageSize（每页条数）等信息。
 * 前端拿到后直接渲染分页表格。
 */
class PharmPageDTO : public PageDTO<PharmDTO::Wrapper>
{
	DTO_INIT(PharmPageDTO, PageDTO<PharmDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif	// !_PHARM_DTO_