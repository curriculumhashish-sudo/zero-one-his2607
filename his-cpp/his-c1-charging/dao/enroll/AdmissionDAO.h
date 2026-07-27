#pragma once
#pragma once
#ifndef _ADMISSION_DAO_
#define _ADMISSION_DAO_

#include "BaseDAO.h"
#include "ApiHelper.h"

class AdmissionDAO : public BaseDAO
{
public:
	bool insertadmission(const oatpp::String& name,
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

	bool deleteadmission(
		const oatpp::String& encounterId,
		const oatpp::String& cancelReason,
		const oatpp::String& operUserId,
		const oatpp::String& tenantId
	);
};
#endif 