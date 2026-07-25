#pragma once

#ifndef _COSTINFO_DO_
#define _COSTINFO_DO_
#include "../DoInclude.h"

/**
 * CostInfo数据库实体对象
 */
class CostInfoDO : public BaseDO
{
	// 序号
	MYSQL_SYNTHESIZE(string, id, Id);
	// 门诊号
	MYSQL_SYNTHESIZE(string, clinicNo, ClinicNo);
	// 患者姓名
	MYSQL_SYNTHESIZE(string, name, Name);
	// 就诊日期
	MYSQL_SYNTHESIZE(string, date, Date);
	// 大类
	MYSQL_SYNTHESIZE(string, feeClass, FeeClass);
	// 细分类
	MYSQL_SYNTHESIZE(string, detailClass, DetailClass);
	// 项目名称
	MYSQL_SYNTHESIZE(string, projectName, ProjectName);
	// 院内编码
	MYSQL_SYNTHESIZE(string, hisID, HisID);
	// 规格
	MYSQL_SYNTHESIZE(string, spec, Spec);
	// 数量
	MYSQL_SYNTHESIZE(double, number, Number);
	// 单位
	MYSQL_SYNTHESIZE(string, unit, Unit);
	// 开单医生
	MYSQL_SYNTHESIZE(string, doctor, Doctor);
	// 收费人
	MYSQL_SYNTHESIZE(string, checker, Checker);
	// 结算时间
	MYSQL_SYNTHESIZE(string, settleTime, SettleTime);
	// 来源
	MYSQL_SYNTHESIZE(string, source, Source);
	// 医保等级
	MYSQL_SYNTHESIZE(string, insuranceLevel, InsuranceLevel);
	// 医保类型
	MYSQL_SYNTHESIZE(string, insuranceType, InsuranceType);
	// 原价
	MYSQL_SYNTHESIZE(double, originalPrice, OriginalPrice);
	// 优惠金额
	MYSQL_SYNTHESIZE(double, discountAmount, DiscountAmount);
	// 应收金额
	MYSQL_SYNTHESIZE(double, receivableAmount, ReceivableAmount);
	// 折后金额
	MYSQL_SYNTHESIZE(double, discountedPrice, DiscountedPrice);
	// 单价
	MYSQL_SYNTHESIZE(double, unitPrice, UnitPrice);
	// 行金额
	MYSQL_SYNTHESIZE(double, lineAmount, LineAmount);
	// 实收金额
	MYSQL_SYNTHESIZE(double, actualAmount, ActualAmount);
	// 医保金额
	MYSQL_SYNTHESIZE(double, insuranceAmount, InsuranceAmount);
	// 自费金额
	MYSQL_SYNTHESIZE(double, selfPayAmount, SelfPayAmount);
public:
	CostInfoDO() : BaseDO("")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("clinicNo", "s", clinicNo);
		MYSQL_ADD_FIELD("name", "s", name);
		MYSQL_ADD_FIELD("date", "s", date);
		MYSQL_ADD_FIELD("feeClass", "s", feeClass);
		MYSQL_ADD_FIELD("detailClass", "s", detailClass);
		MYSQL_ADD_FIELD("projectName", "s", projectName);
		MYSQL_ADD_FIELD("hisID", "s", hisID);
		MYSQL_ADD_FIELD("spec", "s", spec);
		MYSQL_ADD_FIELD("number", "d", number);
		MYSQL_ADD_FIELD("unit", "s", unit);
		MYSQL_ADD_FIELD("doctor", "s", doctor);
		MYSQL_ADD_FIELD("checker", "s", checker);
		MYSQL_ADD_FIELD("settleTime", "s", settleTime);
		MYSQL_ADD_FIELD("source", "s", source);
		MYSQL_ADD_FIELD("insuranceLevel", "s", insuranceLevel);
		MYSQL_ADD_FIELD("insuranceType", "s", insuranceType);
		MYSQL_ADD_FIELD("originalPrice", "d", originalPrice);
		MYSQL_ADD_FIELD("discountAmount", "d", discountAmount);
		MYSQL_ADD_FIELD("receivableAmount", "d", receivableAmount);
		MYSQL_ADD_FIELD("discountedPrice", "d", discountedPrice);
		MYSQL_ADD_FIELD("unitPrice", "d", unitPrice);
		MYSQL_ADD_FIELD("lineAmount", "d", lineAmount);
		MYSQL_ADD_FIELD("actualAmount", "d", actualAmount);
		MYSQL_ADD_FIELD("insuranceAmount", "d", insuranceAmount);
		MYSQL_ADD_FIELD("selfPayAmount", "d", selfPayAmount);
	}
};

// 为CostInfoDO对象指定一个智能指针别名，方便使用
typedef std::shared_ptr<CostInfoDO> PtrCostInfoDO;
#endif // !_COSTINFO_DO_
