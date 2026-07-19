#pragma once


#ifndef EXPORT_DTO_H
#define EXPORT_DTO_H

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)


// 报表导出
class CostInfoExport : public PageQuery
{
	DTO_INIT(CostInfoExport, PageQuery);

	// 挂号科室
	DTO_FIELD(String, register_room);
	DTO_FIELD_INFO(register_room) {
		info->description = "test1";
	}

	// 患者姓名
	DTO_FIELD(String, name);
	DTO_FIELD_INFO(name) {
		info->description = "test2";
	}
};

#include OATPP_CODEGEN_END(DTO)