#pragma once
#ifndef _GET_PATIENT_QUEUE_DTO_
#define _GET_PATIENT_QUEUE_DTO_

#include "../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)
/*
	收费明细据传输对象
*/
class ChargeItemBriefDTO : public oatpp::DTO
{
	DTO_INIT(ChargeItemBriefDTO, DTO);
	//收费明细主键
	API_DTO_FIELD_DEFAULT(String, chargeItemId, ZH_WORDS_GETTER("lhfc.charging.charge.ChargeItemBriefDTO.chargeItemId"));

	//收费状态编码
	API_DTO_FIELD_DEFAULT(String, statusCode, ZH_WORDS_GETTER("lhfc.charging.charge.ChargeItemBriefDTO.statusCode"));

	//收费状态文本
	API_DTO_FIELD_DEFAULT(String, statusText, ZH_WORDS_GETTER("lhfc.charging.charge.ChargeItemBriefDTO.statusText"));
	
	//该行总金额
	API_DTO_FIELD_DEFAULT(Float64, totalPrice, ZH_WORDS_GETTER("lhfc.charging.charge.ChargeItemBriefDTO.totalPrice"));

	//收费项目展示名称
	API_DTO_FIELD_DEFAULT(String, itemLabel, ZH_WORDS_GETTER("lhfc.charging.charge.ChargeItemBriefDTO.itemLabel"));
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
	API_DTO_FIELD_DEFAULT(String, encounterId, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.encounterId"));

	//就诊业务流水号
	API_DTO_FIELD_DEFAULT(String, encounterBusNo, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.encounterBusNo"));

	//患者id
	API_DTO_FIELD_DEFAULT(String, patientId, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.patientId"));

	//患者姓名
	API_DTO_FIELD_DEFAULT(String, patientName, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.patientName"));

	//患者性别编码
	API_DTO_FIELD_DEFAULT(String, genderCode, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.genderCode"));

	//患者性别文本
	API_DTO_FIELD_DEFAULT(String, genderEnumText, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.genderEnumText"));

	//患者年龄
	API_DTO_FIELD_DEFAULT(String, ageText, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.ageText"));

	//科室名称
	API_DTO_FIELD_DEFAULT(String, organizationName, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.organizationName"));

	//接诊医生姓名
	API_DTO_FIELD_DEFAULT(String, doctorName, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.doctorName"));

	//就诊开始时间
	API_DTO_FIELD_DEFAULT(String, startTime, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.startTime"));

	//队列状态
	API_DTO_FIELD_DEFAULT(String, queueLabelCode, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.queueLabelCode"));

	//就诊费用总金额
	API_DTO_FIELD_DEFAULT(Float64, totalAmount, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.totalAmount"));

	//已支付金额
	API_DTO_FIELD_DEFAULT(Float64, paidAmount, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.paidAmount"));

	//收费明细摘要列表
	API_DTO_FIELD_DEFAULT(Vector<Object<ChargeItemBriefDTO>>, cashierChargeItems, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.cashierChargeItems"));

	//收费状态汇总编码
	API_DTO_FIELD_DEFAULT(String, cashierChargeRollupStatusCode, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.cashierChargeRollupStatusCode"));

	//收费状态汇总文本
	API_DTO_FIELD_DEFAULT(String, cashierChargeRollupStatusText, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.cashierChargeRollupStatusText"));

	//最近一次成功支付流水主键
	API_DTO_FIELD_DEFAULT(String, paymentId, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.paymentId"));
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
	DTO_INIT(GetPatientQueuePageDTO, PageDTO<GetPatientQueueDTO::Wrapper>);
};
#include OATPP_CODEGEN_END(DTO)
#endif //!_GET_PATIENT_QUEUE_DTO_