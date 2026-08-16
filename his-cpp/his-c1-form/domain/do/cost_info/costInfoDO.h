#pragma once

#ifndef _COSTINFO_DO_
#define _COSTINFO_DO_
#include "../DoInclude.h"

class CostInfoDO : public BaseDO
{
	MYSQL_SYNTHESIZE(string, chargeID, ChargeID);
	MYSQL_SYNTHESIZE(string, encounterID, EncounterID);
	MYSQL_SYNTHESIZE(string, paymentID, PaymentID);
	MYSQL_SYNTHESIZE(string, cashierID, CashierID);
	MYSQL_SYNTHESIZE(string, name, Name);
	MYSQL_SYNTHESIZE(string, room, Room);
	MYSQL_SYNTHESIZE(string, roomID, RoomID);
	MYSQL_SYNTHESIZE(string, date, Date);
	MYSQL_SYNTHESIZE(string, feeClass, FeeClass);
	MYSQL_SYNTHESIZE(string, feeClassCode, FeeClassCode);
	MYSQL_SYNTHESIZE(string, detailClass, DetailClass);
	MYSQL_SYNTHESIZE(string, detailClassCode, DetailClassCode);
	MYSQL_SYNTHESIZE(string, projectName, ProjectName);
	MYSQL_SYNTHESIZE(string, hisID, HisID);
	MYSQL_SYNTHESIZE(string, spec, Spec);
	MYSQL_SYNTHESIZE(double, number, Number);
	MYSQL_SYNTHESIZE(string, unit, Unit);
	MYSQL_SYNTHESIZE(string, doctor, Doctor);
	MYSQL_SYNTHESIZE(string, checker, Checker);
	MYSQL_SYNTHESIZE(string, settleTime, SettleTime);
	MYSQL_SYNTHESIZE(string, source, Source);
	MYSQL_SYNTHESIZE(string, insuranceLevel, InsuranceLevel);
	MYSQL_SYNTHESIZE(string, insuranceType, InsuranceType);
	MYSQL_SYNTHESIZE(double, originalPrice, OriginalPrice);
	MYSQL_SYNTHESIZE(double, discountAmount, DiscountAmount);
	MYSQL_SYNTHESIZE(double, receivableAmount, ReceivableAmount);
	MYSQL_SYNTHESIZE(double, discountedPrice, DiscountedPrice);
	MYSQL_SYNTHESIZE(double, unitPrice, UnitPrice);
	MYSQL_SYNTHESIZE(double, lineAmount, LineAmount);
	MYSQL_SYNTHESIZE(double, actualAmount, ActualAmount);
	MYSQL_SYNTHESIZE(double, insuranceAmount, InsuranceAmount);
	MYSQL_SYNTHESIZE(double, selfPayAmount, SelfPayAmount);
public:
	CostInfoDO() : BaseDO("")
	{
		MYSQL_ADD_FIELD_PK("chargeID", "s", chargeID);
		MYSQL_ADD_FIELD("encounterID", "s", encounterID);
		MYSQL_ADD_FIELD("paymentID", "s", paymentID);
		MYSQL_ADD_FIELD("cashierID", "s", cashierID);
		MYSQL_ADD_FIELD("name", "s", name);
		MYSQL_ADD_FIELD("room", "s", room);
		MYSQL_ADD_FIELD("roomID", "s", roomID);
		MYSQL_ADD_FIELD("date", "s", date);
		MYSQL_ADD_FIELD("feeClass", "s", feeClass);
		MYSQL_ADD_FIELD("feeClassCode", "s", feeClassCode);
		MYSQL_ADD_FIELD("detailClass", "s", detailClass);
		MYSQL_ADD_FIELD("detailClassCode", "s", detailClassCode);
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

typedef std::shared_ptr<CostInfoDO> PtrCostInfoDO;
#endif // !_COSTINFO_DO_
