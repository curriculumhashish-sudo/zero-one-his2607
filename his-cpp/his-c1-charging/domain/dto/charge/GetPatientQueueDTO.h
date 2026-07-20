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
	API_DTO_FIELD(String, chargeItemId, ZH_WORDS_GETTER("lhfc.charging.charge.ChargeItemBriefDTO.chargeItemId"), true, "335346159333150720");

	//收费状态编码
	API_DTO_FIELD(String, statusCode, ZH_WORDS_GETTER("lhfc.charging.charge.ChargeItemBriefDTO.statusCode"), true, "SETTLED");

	//收费状态文本
	API_DTO_FIELD(String, statusText, ZH_WORDS_GETTER("lhfc.charging.charge.ChargeItemBriefDTO.statusText"), true, "已结算");

	//该行总金额
	API_DTO_FIELD(Float64, totalPrice, ZH_WORDS_GETTER("lhfc.charging.charge.ChargeItemBriefDTO.totalPrice"), true, 0.23);

	//收费项目展示名称
	API_DTO_FIELD(String, itemLabel, ZH_WORDS_GETTER("lhfc.charging.charge.ChargeItemBriefDTO.itemLabel"), true, "盐酸氨溴索注射液");
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
	API_DTO_FIELD(String, encounterId, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.encounterId"), true, "335345648856993792");

	//就诊业务流水号
	API_DTO_FIELD(String, encounterBusNo, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.encounterBusNo"), false, "EN2026-07-140001");

	//患者id
	API_DTO_FIELD(String, patientId, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.patientId"), true, "326940849401761792");

	//患者姓名
	API_DTO_FIELD(String, patientName, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.patientName"), true, "周女士");

	//患者性别编码
	API_DTO_FIELD(String, genderCode, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.genderCode"), false, "FEMALE");

	//患者性别文本
	API_DTO_FIELD(String, genderEnumText, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.genderEnumText"), false, "女性");

	//患者年龄
	API_DTO_FIELD(String, ageText, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.ageText"), false, "26岁");

	//科室名称
	API_DTO_FIELD(String, organizationName, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.organizationName"), false, "门诊内科");

	//接诊医生姓名
	API_DTO_FIELD(String, doctorName, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.doctorName"), false, "admin");

	//就诊开始时间
	API_DTO_FIELD(String, startTime, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.startTime"), false, "2026-07-14 17:05:08");

	//队列状态
	API_DTO_FIELD(String, queueLabelCode, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.queueLabelCode"), true, "SETTLED");

	//就诊费用总金额
	API_DTO_FIELD(Float64, totalAmount, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.totalAmount"), false, 568.50);

	//已支付金额
	API_DTO_FIELD(Float64, paidAmount, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.paidAmount"), false, 200.00);

	//收费明细摘要列表
	API_DTO_FIELD(Vector<Object<ChargeItemBriefDTO>>, cashierChargeItems, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.cashierChargeItems"), false, {});

	//收费状态汇总编码
	API_DTO_FIELD(String, cashierChargeRollupStatusCode, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.cashierChargeRollupStatusCode"), false, "SETTLED");

	//收费状态汇总文本
	API_DTO_FIELD(String, cashierChargeRollupStatusText, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.cashierChargeRollupStatusText"), false, "已结算");

	//最近一次成功支付流水主键
	API_DTO_FIELD(String, paymentId, ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueDTO.paymentId"), false, "335373637774217216");
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
