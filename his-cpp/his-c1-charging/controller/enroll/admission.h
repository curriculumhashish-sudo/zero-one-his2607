#ifndef _ADMISSION_CONTROLLER_
#define _ADMISSION_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/vo/enroll/AdmissionVO.h"
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
		API_DEF_ADD_RSP_JSON_WRAPPER(AdmissionAddResultJsonVO);
		info->summary = ZH_WORDS_GETTER("charging.admission.add_discription");
		info->description = ZH_WORDS_GETTER("charging.admission.add_error_codes");
		API_DEF_ADD_TAG(API_TAG);
	}

	ENDPOINT_INFO(admission_delete) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("charging.admission.delete_discription"));
		API_DEF_ADD_AUTH();
		API_DEF_ADD_RSP_JSON_WRAPPER(StringJsonVO);
		info->summary = ZH_WORDS_GETTER("charging.admission.delete_discription");
		info->description = ZH_WORDS_GETTER("charging.admission.delete_error_codes");
		API_DEF_ADD_TAG(API_TAG);
	}

	ENDPOINT(API_M_DEL, "/charging/admission/delete", admission_delete, BODY_DTO(AdmissionDeleteDTO::Wrapper, dto), API_HANDLER_AUTH_PARAME) {
		API_HANDLER_RESP_VO(execAdmissionDelete(dto, authObject));
	}

	ENDPOINT(API_M_POST, "/charging/admission/add", admission_add, BODY_DTO(AdmissionAddDTO::Wrapper, dto), API_HANDLER_AUTH_PARAME) {
		API_HANDLER_RESP_VO(execAdmissionAdd(dto, authObject));
	}

private:
	StringJsonVO::Wrapper execAdmissionDelete(const AdmissionDeleteDTO::Wrapper& dto, const std::shared_ptr<CustomerAuthorizeObject>& authObject);

	AdmissionAddResultJsonVO::Wrapper execAdmissionAdd(const AdmissionAddDTO::Wrapper& dto, const std::shared_ptr<CustomerAuthorizeObject>& authObject);
};


#undef API_TAG

// Cancel API controller codegen
#include OATPP_CODEGEN_END(ApiController) //<- End Codegen
#endif // !_ADMISSION_CONTROLLER_