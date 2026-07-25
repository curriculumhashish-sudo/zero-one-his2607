#pragma once

#ifndef _COSTINFO_DTO_
#define _COSTINFO_DTO_
#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// 查询用的DTO
class CostInfoDTO : public PageQuery
{
	DTO_INIT(CostInfoDTO, PageQuery);

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
	DTO_FIELD(String, hisID);
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

	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
};

// 详细的DTO(用于展示/导出的)
class CostInfoDataDTO : public oatpp::DTO
{
	DTO_INIT(CostInfoDataDTO, DTO);

	DTO_FIELD(String, id);
	DTO_FIELD_INFO(id) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.id");
	}

	DTO_FIELD(String, clinicNo);
	DTO_FIELD_INFO(clinicNo) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.clinicNo");
	}

	DTO_FIELD(String, name);
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.name");
	}

	DTO_FIELD(String, room);
	DTO_FIELD_INFO(room) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.room");
	}

	DTO_FIELD(String, date);
	DTO_FIELD_INFO(date) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.date");
	}

	DTO_FIELD(String, feeClass);
	DTO_FIELD_INFO(feeClass) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.feeClass");
	}

	DTO_FIELD(String, detailClass);
	DTO_FIELD_INFO(detailClass) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.detailClass");
	}

	DTO_FIELD(String, projectName);
	DTO_FIELD_INFO(projectName) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.projectName");
	}

	DTO_FIELD(String, hisID);
	DTO_FIELD_INFO(hisID) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.hisID");
	}

	DTO_FIELD(String, spec);
	DTO_FIELD_INFO(spec) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.spec");
	}

	DTO_FIELD(Float64, number);
	DTO_FIELD_INFO(number) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.number");
	}

	DTO_FIELD(String, unit);
	DTO_FIELD_INFO(unit) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.unit");
	}

	DTO_FIELD(String, doctor);
	DTO_FIELD_INFO(doctor) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.doctor");
	}

	DTO_FIELD(String, checker);
	DTO_FIELD_INFO(checker) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.checker");
	}

	DTO_FIELD(String, settleTime);
	DTO_FIELD_INFO(settleTime) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.settleTime");
	}

	DTO_FIELD(String, source);
	DTO_FIELD_INFO(source) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.source");
	}

	DTO_FIELD(String, insuranceLevel);
	DTO_FIELD_INFO(insuranceLevel) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.insuranceLevel");
	}

	DTO_FIELD(String, insuranceType);
	DTO_FIELD_INFO(insuranceType) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.insuranceType");
	}

	DTO_FIELD(Float64, originalPrice);
	DTO_FIELD_INFO(originalPrice) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.originalPrice");
	}

	DTO_FIELD(Float64, discountAmount);
	DTO_FIELD_INFO(discountAmount) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.discountAmount");
	}

	DTO_FIELD(Float64, receivableAmount);
	DTO_FIELD_INFO(receivableAmount) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.receivableAmount");
	}

	DTO_FIELD(Float64, discountedPrice);
	DTO_FIELD_INFO(discountedPrice) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.discountedPrice");
	}

	DTO_FIELD(Float64, unitPrice);
	DTO_FIELD_INFO(unitPrice) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.unitPrice");
	}

	DTO_FIELD(Float64, lineAmount);
	DTO_FIELD_INFO(lineAmount) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.lineAmount");
	}

	DTO_FIELD(Float64, actualAmount);
	DTO_FIELD_INFO(actualAmount) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.actualAmount");
	}

	DTO_FIELD(Float64, insuranceAmount);
	DTO_FIELD_INFO(insuranceAmount) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.insuranceAmount");
	}

	DTO_FIELD(Float64, selfPayAmount);
	DTO_FIELD_INFO(selfPayAmount) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.selfPayAmount");
	}
};



#include OATPP_CODEGEN_END(DTO)
#endif // !_COSTINFO_DTO_