#pragma once


#ifndef QUERY_DETAIL_QUERY
#define QUERY_DETAIL_QUERY

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)


// 获取明细列表的query
class CostInfoQuery :public PageQuery
{
	DTO_INIT(CostInfoQuery, PageQuery);

	// 开始时间
	DTO_FIELD(UInt64, start_time);
	DTO_FIELD_INFO(start_time) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.query.start_time");
		info->required = true;
	}

	// 结束时间
	DTO_FIELD(UInt64, end_time);
	DTO_FIELD_INFO(end_time) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.query.end_time");
		info->required = true;
	}

	// 患者姓名
	DTO_FIELD(String, name);
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.query.name");
	}

	// 挂号科室
	DTO_FIELD(String, room);
	DTO_FIELD_INFO(room) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.query.room");
	}

	// 收费大类
	DTO_FIELD(String, feeClass);
	DTO_FIELD_INFO(feeClass) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.query.feeClass");
	}

	// 细分类
	DTO_FIELD(String, detailClass);
	DTO_FIELD_INFO(detailClass) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.query.detailClass");
	}

	// 项目名称
	DTO_FIELD(String, projectName);
	DTO_FIELD_INFO(projectName) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.query.projectName");
	}

	// 院内编号
	DTO_FIELD(UInt64, hisID);
	DTO_FIELD_INFO(hisID) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.query.hisID");
	}

	// 收银员
	DTO_FIELD(String, checker);
	DTO_FIELD_INFO(checker) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.query.checker");
	}

	// 收退方向
	DTO_FIELD(String, feeDir);
	DTO_FIELD_INFO(feeDir) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.query.feeDir");
	}

};

#include OATPP_CODEGEN_END(DTO)
#endif