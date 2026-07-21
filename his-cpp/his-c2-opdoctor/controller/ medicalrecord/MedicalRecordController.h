#ifndef _MEDICALRECORDCONTROLLER_H_
#define _MEDICALRECORDCONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "../../domain/dto/ medicalrecord/MedicalRecordSaveDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("medicalrecord.tags.tag")

class MedicalRecordController
	: public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(MedicalRecordController);

public:

	// ±£´æÃÅÕï²¡Àú
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("medicalrecord.save.summary"),
		saveMedicalRecord,
		StringJsonVO::Wrapper,
		API_TAG);

	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/medicalrecord/save",
		saveMedicalRecord,
		BODY_DTO(MedicalRecordSaveDTO::Wrapper, dto),
		execSaveMedicalRecord(dto, authObject->getPayload()));

private:

	StringJsonVO::Wrapper
		execSaveMedicalRecord(
			const MedicalRecordSaveDTO::Wrapper& dto,
			const PayloadDTO& payload);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif