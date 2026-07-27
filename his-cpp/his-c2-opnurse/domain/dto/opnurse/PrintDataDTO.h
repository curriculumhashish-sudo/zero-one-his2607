#pragma once
/*
 Copyright Zero One Star. All rights reserved.
 @Author: sqdxy
 @Date: 2026/07/23
*/
#ifndef _PRINT_DATA_DTO_
#define _PRINT_DATA_DTO_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PrintDataDTO : public oatpp::DTO
{
	DTO_INIT(PrintDataDTO, DTO);
	// common fields across all doc types
	DTO_FIELD(String, patientName);
	DTO_FIELD_INFO(patientName) { info->description = ZH_WORDS_GETTER("print.field.patientName"); }
	DTO_FIELD(String, doctorName);
	DTO_FIELD_INFO(doctorName) { info->description = ZH_WORDS_GETTER("print.field.doctorName"); }
	DTO_FIELD(String, remark);
	DTO_FIELD_INFO(remark) { info->description = ZH_WORDS_GETTER("print.field.remark"); }
	// treatment
	DTO_FIELD(String, treatmentItem);
	DTO_FIELD(String, execDept);
	DTO_FIELD(String, orderTime);
	DTO_FIELD(String, execTime);
	DTO_FIELD(String, executor);
	// prescription
	DTO_FIELD(String, drugName);
	DTO_FIELD(String, specification);
	DTO_FIELD(String, quantity);
	DTO_FIELD(String, usage);
	DTO_FIELD(String, frequency);
	// charge_receipt
	DTO_FIELD(String, receiptNo);
	DTO_FIELD(String, chargeTime);
	DTO_FIELD(String, itemName);
	DTO_FIELD(String, unitPrice);
	DTO_FIELD(String, amount);
	DTO_FIELD(String, totalAmount);
	// reg_receipt
	DTO_FIELD(String, deptName);
	DTO_FIELD(String, regTime);
	DTO_FIELD(String, regType);
	DTO_FIELD(String, regLevel);
	DTO_FIELD(String, queueNo);
	DTO_FIELD(String, regFee);
	// infusion_label
	DTO_FIELD(String, bedNo);
	DTO_FIELD(String, admissionNo);
	DTO_FIELD(String, dosage);
	DTO_FIELD(String, dripRate);
	// daily_settlement
	DTO_FIELD(String, settleDate);
	DTO_FIELD(String, operatorName);
	DTO_FIELD(String, regTotal);
	DTO_FIELD(String, drugTotal);
	DTO_FIELD(String, examTotal);
	DTO_FIELD(String, labTotal);
	DTO_FIELD(String, treatmentTotal);
	// exam_request / lab_request
	DTO_FIELD(String, applyDept);
	DTO_FIELD(String, examItem);
	DTO_FIELD(String, examBodyPart);
	DTO_FIELD(String, examMethod);
	DTO_FIELD(String, labItem);
	DTO_FIELD(String, specimenType);
	DTO_FIELD(String, sampleTime);
	DTO_FIELD(String, diagnosis);
	// system
	DTO_FIELD(String, printTime);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PRINT_DATA_DTO_
