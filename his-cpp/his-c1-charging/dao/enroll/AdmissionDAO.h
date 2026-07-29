#pragma once

#ifndef _ADMISSION_DAO_
#define _ADMISSION_DAO_

#include "BaseDAO.h"
#include "ApiHelper.h"

/*
 * 入院数据访问对象。
 * 处理住院入院登记的多表插入/删除操作。
 */
class AdmissionDAO : public BaseDAO
{
public:
	// 插入新的住院入院记录（多表事务）。
	// 成功返回 true；outPatientId/outEncounterId/outBusNo 接收生成的ID。
	bool insertAdmission(
		const oatpp::String& name,
		const oatpp::String& idCard,
		const oatpp::String& birthTime,
		const oatpp::Int32& age,
		const oatpp::String& sex,
		const oatpp::String& phone,
		const oatpp::String& admissionDept,
		const oatpp::String& admissionWard,
		const oatpp::String& attendingDoctor,
		const oatpp::String& admissionDiagnosis,
		const oatpp::String& diagnosisDesc,
		const oatpp::String& patientCondition,
		const oatpp::String& admissionType,
		const oatpp::String& admissionMode,
		const oatpp::String& feeNature,
		const oatpp::String& admissionDate,
		const oatpp::String& prepayAmount,
		const oatpp::String& payWay,
		const oatpp::String& tenantId,
		const oatpp::String& orgId,
		const oatpp::String& operUserId,
		oatpp::String& outPatientId,
		oatpp::String& outEncounterId,
		oatpp::String& outBusNo
	);

	// 取消住院入院（软删除：设置 is_deleted = 1）。
	// 成功返回 true。
	bool deleteAdmission(
		const oatpp::String& encounterId,
		const oatpp::String& cancelReason,
		const oatpp::String& operUserId,
		const oatpp::String& tenantId
	);

private:
	// 生成不含分隔符的UUID字符串
	std::string generateUuid();

	// 生成业务编号，如"ZY202607280001"
	std::string generateBusNo(const oatpp::String& prefix);
};

#endif // !_ADMISSION_DAO_
