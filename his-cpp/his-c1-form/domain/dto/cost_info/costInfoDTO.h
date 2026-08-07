#pragma once

#ifndef _COSTINFO_DTO_
#define _COSTINFO_DTO_
#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"
#include "domain/dto/PageDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class CostInfoDTO : public oatpp::DTO
{
	DTO_INIT(CostInfoDTO, DTO);

	// 收费条目ID
	DTO_FIELD(String, chargeID);
	DTO_FIELD_INFO(chargeID) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.chargeID");
	}

	// 就诊条目ID
	DTO_FIELD(String, encounterID);
	DTO_FIELD_INFO(encounterID) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.encounterID");
	}

	// 支付条目ID
	DTO_FIELD(String, paymentID);
	DTO_FIELD_INFO(paymentID) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.paymentID");
	}

	// 收费员ID
	DTO_FIELD(String, cashierID);
	DTO_FIELD_INFO(cashierID) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.cashierID");
	}

	// 患者
	DTO_FIELD(String, name);
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.name");
	}

	// 挂号科室
	DTO_FIELD(String, room);
	DTO_FIELD_INFO(room) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.room");
	}

	// 就诊日期
	DTO_FIELD(String, date);
	DTO_FIELD_INFO(date) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.date");
	}

	// 大类
	DTO_FIELD(String, feeClass);
	DTO_FIELD_INFO(feeClass) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.feeClass");
	}

	// 细分类
	DTO_FIELD(String, detailClass);
	DTO_FIELD_INFO(detailClass) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.detailClass");
	}

	// 项目名称
	DTO_FIELD(String, projectName);
	DTO_FIELD_INFO(projectName) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.projectName");
	}

	// 院内编码
	DTO_FIELD(String, hisID);
	DTO_FIELD_INFO(hisID) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.hisID");
	}

	// 规格
	DTO_FIELD(String, spec);
	DTO_FIELD_INFO(spec) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.spec");
	}

	// 数量
	DTO_FIELD(Float64, number);
	DTO_FIELD_INFO(number) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.number");
	}

	// 单位
	DTO_FIELD(String, unit);
	DTO_FIELD_INFO(unit) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.unit");
	}

	// 开单医生
	DTO_FIELD(String, doctor);
	DTO_FIELD_INFO(doctor) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.doctor");
	}

	// 收费人
	DTO_FIELD(String, checker);
	DTO_FIELD_INFO(checker) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.checker");
	}

	// 结算时间
	DTO_FIELD(String, settleTime);
	DTO_FIELD_INFO(settleTime) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.settleTime");
	}

	// 来源
	DTO_FIELD(String, source);
	DTO_FIELD_INFO(source) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.source");
	}

	// 医保等级
	DTO_FIELD(String, insuranceLevel);
	DTO_FIELD_INFO(insuranceLevel) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.insuranceLevel");
	}

	// 医保类型
	DTO_FIELD(String, insuranceType);
	DTO_FIELD_INFO(insuranceType) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.insuranceType");
	}

	// 原价
	DTO_FIELD(Float64, originalPrice);
	DTO_FIELD_INFO(originalPrice) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.originalPrice");
	}

	// 优惠金额
	DTO_FIELD(Float64, discountAmount);
	DTO_FIELD_INFO(discountAmount) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.discountAmount");
	}

	// 应收金额
	DTO_FIELD(Float64, receivableAmount);
	DTO_FIELD_INFO(receivableAmount) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.receivableAmount");
	}

	// 折后金额
	DTO_FIELD(Float64, discountedPrice);
	DTO_FIELD_INFO(discountedPrice) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.discountedPrice");
	}

	// 单价
	DTO_FIELD(Float64, unitPrice);
	DTO_FIELD_INFO(unitPrice) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.unitPrice");
	}

	// 行金额
	DTO_FIELD(Float64, lineAmount);
	DTO_FIELD_INFO(lineAmount) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.lineAmount");
	}

	// 实收金额
	DTO_FIELD(Float64, actualAmount);
	DTO_FIELD_INFO(actualAmount) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.actualAmount");
	}

	// 医保金额
	DTO_FIELD(Float64, insuranceAmount);
	DTO_FIELD_INFO(insuranceAmount) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.insuranceAmount");
	}

	// 自费金额
	DTO_FIELD(Float64, selfPayAmount);
	DTO_FIELD_INFO(selfPayAmount) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.data.selfPayAmount");
	}
};

class CostInfoDataDTO : public PageDTO<CostInfoDTO::Wrapper>
{
	DTO_INIT(CostInfoDataDTO, PageDTO<CostInfoDTO::Wrapper>);
};



#include OATPP_CODEGEN_END(DTO)
#endif // !_COSTINFO_DTO_