#pragma once
#ifndef _GET_PATIENT_QUEUE_DTO_
#define _GET_PATIENT_QUEUE_DTO_

#include "../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)
/*
	收费明细据传输对象
*/
class ChargeItemDTO : public oatpp::DTO
{
	DTO_INIT(ChargeItemDTO, DTO);
	//收费明细主键
	DTO_FIELD(String, chargeItemId);
	DTO_FIELD_INFO(chargeItemId) {
		info->description = ZH_WORDS_GETTER("lhfc.charging.charge.ChargeItemDTO.chargeItemId");
	}

	//收费状态编码
	DTO_FIELD(String, statusCode);
	DTO_FIELD_INFO(statusCode) {
		info->description = ZH_WORDS_GETTER("lhfc.charging.charge.ChargeItemDTO.statusCode");
	}

	//收费状态文本
	DTO_FIELD(String, statusText);
	DTO_FIELD_INFO(statusText) {
		info->description = ZH_WORDS_GETTER("lhfc.charging.charge.ChargeItemDTO.statusText");
	}
	
	//该行总金额
	DTO_FIELD(Float64, totalPrice);
	DTO_FIELD_INFO(totalPrice) {
		info->description = ZH_WORDS_GETTER("lhfc.charging.charge.ChargeItemDTO.totalPrice");
	}

	//收费项目展示名称
	DTO_FIELD(String, itemLabel);
	DTO_FIELD_INFO(itemLabel) {
		info->description = ZH_WORDS_GETTER("lhfc.charging.charge.ChargeItemDTO.itemLabel");
	}
};
/*
	获取患者队列数据传输对象
*/
class GetPatientQueueDTO : public oatpp::DTO
{
	DTO_INIT(GetPatientQueueDTO, DTO);

	// 关联一个PayloadDTO负载数据对象
	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);

	//就诊单id
	DTO_FIELD(String, encounterId);
	DTO_FIELD_INFO(encounterId) {
		info->description = ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.encounterId");
	}

	//就诊业务流水号
	DTO_FIELD(String, encounterBusNo);
	DTO_FIELD_INFO(encounterBusNo) {
		info->description = ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.encounterBusNo");
	}

	//患者id
	DTO_FIELD(String, patientId);
	DTO_FIELD_INFO(patientId) {
		info->description = ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.patientId");
	}

	//患者姓名
	DTO_FIELD(String, patientName);
	DTO_FIELD_INFO(patientName) {
		info->description = ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.patientName");
	}

	//患者性别编码
	DTO_FIELD(String, genderCode);
	DTO_FIELD_INFO(genderCode) {
		info->description = ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.genderCode");
	}

	//患者性别文本
	DTO_FIELD(String, genderEnumText);
	DTO_FIELD_INFO(genderEnumText) {
		info->description = ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.genderEnumText");
	}

	//患者年龄
	DTO_FIELD(String, ageText);
	DTO_FIELD_INFO(ageText) {
		info->description = ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.ageText");
	}

	//科室名称
	DTO_FIELD(String, organizationName);
	DTO_FIELD_INFO(organizationName) {
		info->description = ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.organizationName");
	}

	//接诊医生姓名
	DTO_FIELD(String, doctorName);
	DTO_FIELD_INFO(doctorName) {
		info->description = ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.doctorName");
	}

	//就诊开始时间
	DTO_FIELD(String, startTime);
	DTO_FIELD_INFO(startTime) {
		info->description = ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.startTime");
	}

	//队列状态
	DTO_FIELD(String, queueLabelCode);
	DTO_FIELD_INFO(queueLabelCode) {
		info->description = ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.queueLabelCode");
	}

	//就诊费用总金额
	DTO_FIELD(Float64, totalAmount);
	DTO_FIELD_INFO(totalAmount) {
		info->description = ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.totalAmount");
	}

	//已支付金额
	DTO_FIELD(Float64, paidAmount);
	DTO_FIELD_INFO(paidAmount) {
		info->description = ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.paidAmount");
	}

	//收费明细摘要列表
	DTO_FIELD(Vector<Object<ChargeItemDTO>>, cashierChargeItems);
	DTO_FIELD_INFO(cashierChargeItems) {
		info->description = ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.cashierChargeItems");
	}

	//收费状态汇总编码
	DTO_FIELD(String, cashierChargeRollupStatusCode);
	DTO_FIELD_INFO(cashierChargeRollupStatusCode) {
		info->description = ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.cashierChargeRollupStatusCode");
	}

	//收费状态汇总文本
	DTO_FIELD(String, cashierChargeRollupStatusText);
	DTO_FIELD_INFO(cashierChargeRollupStatusText) {
		info->description = ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.cashierChargeRollupStatusText");
	}

	//最近一次成功支付流水主键
	DTO_FIELD(String, paymentId);
	DTO_FIELD_INFO(paymentId) {
		info->description = ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.paymentId");
	}
public:
	// 数据校验（如需启用可自行解除注释并按需实现）
	//std::string validate()
	//{
	//	if (!age || age < 0)
	//		return "age invalidate.";
	//	return "";
	//}
};

/*
	获取患者队列分页数据传输对象
*/
class GetPatientQueuePageDTO : public PageDTO<GetPatientQueueDTO::Wrapper>
{
	DTO_INIT(GetPatientQueuePageDTO, DTO);
};
#include OATPP_CODEGEN_END(DTO)
#endif //!_GET_PATIENT_QUEUE_DTO_