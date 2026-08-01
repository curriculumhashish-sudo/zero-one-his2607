#ifndef _ADMISSION_CONTROLLER_
#define _ADMISSION_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/enroll/admissionDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen

#define API_TAG ZH_WORDS_GETTER("charging.admission.tag")

class AdmissionController : public oatpp::web::server::api::ApiController // Inherit ApiController
{
	// Define controller access entry
	API_ACCESS_DECLARE(AdmissionController);
	// Define interfaces
public:
	ENDPOINT_INFO(admission_add) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("charging.admission.add_discription"));
		API_DEF_ADD_AUTH();
		API_DEF_ADD_RSP_JSON_WRAPPER(StringJsonVO);
		info->summary = ZH_WORDS_GETTER("charging.admission.add_discription");
		API_DEF_ADD_TAG(API_TAG);

		API_DEF_ADD_PATH_PARAMS(String, "idCard", ZH_WORDS_GETTER("charging.admission.idCard"), "11010119900101123X", true);
		API_DEF_ADD_PATH_PARAMS(String, "name", ZH_WORDS_GETTER("charging.admission.name"), "Zhang San", false);
		API_DEF_ADD_PATH_PARAMS(String, "birthtime", ZH_WORDS_GETTER("charging.admission.birthtime"), "1990-01-01", false);
		API_DEF_ADD_PATH_PARAMS(Int32, "age", ZH_WORDS_GETTER("charging.admission.age"), 18, true);
		API_DEF_ADD_PATH_PARAMS(String, "sex", ZH_WORDS_GETTER("charging.admission.sex"), "man", false);
		API_DEF_ADD_PATH_PARAMS(String, "phone", ZH_WORDS_GETTER("charging.admission.phone"), "13800000000", true);
		API_DEF_ADD_PATH_PARAMS(String, "admissionDept", ZH_WORDS_GETTER("charging.admission.admissionDept"), "Internal Medicine", true);
		API_DEF_ADD_PATH_PARAMS(String, "admission_ward", ZH_WORDS_GETTER("charging.admission.admission_ward"), "Ward 1", true);
		API_DEF_ADD_PATH_PARAMS(String, "attending_doctor", ZH_WORDS_GETTER("charging.admission.attending_doctor"), "Dr. Li", false);
		API_DEF_ADD_PATH_PARAMS(String, "admissionDiagnosis", ZH_WORDS_GETTER("charging.admission.admissionDiagnosis"), "Common cold", true);
		API_DEF_ADD_PATH_PARAMS(String, "diagnosisDesc", ZH_WORDS_GETTER("charging.admission.diagnosisDesc"), "Description", false);
		API_DEF_ADD_PATH_PARAMS(String, "patientCondition", ZH_WORDS_GETTER("charging.admission.patientCondition"), "Stable", false);
		API_DEF_ADD_PATH_PARAMS(String, "admissionType", ZH_WORDS_GETTER("charging.admission.admissionType"), "NORMAL", false);
		API_DEF_ADD_PATH_PARAMS(String, "admissionMode", ZH_WORDS_GETTER("charging.admission.admissionMode"), "WALK_IN", false);
		API_DEF_ADD_PATH_PARAMS(String, "feeNature", ZH_WORDS_GETTER("charging.admission.feeNature"), "SELF_PAY", true);
		API_DEF_ADD_PATH_PARAMS(String, "admissionDate", ZH_WORDS_GETTER("charging.admission.admissionDate"), "2026-07-30", true);
		API_DEF_ADD_PATH_PARAMS(String, "prepayAmount", ZH_WORDS_GETTER("charging.admission.prepayAmount"), "0", false);
		API_DEF_ADD_PATH_PARAMS(String, "payWay", ZH_WORDS_GETTER("charging.admission.payWay"), "CASH", false);
	}

	ENDPOINT_INFO(admission_delete) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("charging.admission.delete_discription"));
		API_DEF_ADD_AUTH();
		API_DEF_ADD_RSP_JSON_WRAPPER(StringJsonVO);
		info->summary = ZH_WORDS_GETTER("charging.admission.delete_discription");
		API_DEF_ADD_TAG(API_TAG);

		API_DEF_ADD_PATH_PARAMS(String, "encounterId", ZH_WORDS_GETTER("charging.admission.delete"), "demo-encounter-001", true);
	}

	ENDPOINT(API_M_DEL, "/charging/admission/delete", admission_delete, BODY_DTO(AdmissionDeleteDTO::Wrapper, dto), API_HANDLER_AUTH_PARAME) {
		API_HANDLER_RESP_VO(execAdmissionDelete(dto, authObject));
	}

	ENDPOINT(API_M_POST, "/charging/admission/add", admission_add, BODY_DTO(AdmissionAddDTO::Wrapper, dto), API_HANDLER_AUTH_PARAME) {
		API_HANDLER_RESP_VO(execAdmissionAdd(dto, authObject));
	}

private:
	StringJsonVO::Wrapper execAdmissionAdd(const AdmissionAddDTO::Wrapper& dto, const std::shared_ptr<CustomerAuthorizeObject>& authObject);

	StringJsonVO::Wrapper execAdmissionDelete(const AdmissionDeleteDTO::Wrapper& dto, const std::shared_ptr<CustomerAuthorizeObject>& authObject);
};


#undef API_TAG

// Cancel API controller codegen
#include OATPP_CODEGEN_END(ApiController) //<- End Codegen
#endif // !_ADMISSION_CONTROLLER_