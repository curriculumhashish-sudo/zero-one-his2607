#ifndef _SAMPLE_CONTROLLER_
#define _SAMPLE_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/admission/admissionDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen

#define API_TAG ZH_WORDS_GETTER("charging.admission.tag")

#define API_TAG1 ZH_WORDS_GETTER("charging.admission.delete")
//#define API_TAG2 ZH_WORDS_GETTER("charging.admission.tag1")

/*
 * 示例控制器，演示基础接口的使用
 */
class AdmissionController : public oatpp::web::server::api::ApiController // 1 继承控制器
{
	// 2 定义控制器访问入口
	API_ACCESS_DECLARE(AdmissionController);
	// 3 定义接口
public:
	// 3.1 定义查询接口描述
	ENDPOINT_INFO(admission_add) {
		// 定义接口标题
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("admission_add"));
		// 定义默认授权参数（可选定义，如果定义了，下面ENDPOINT里面需要加入API_HANDLER_AUTH_PARAME）
		API_DEF_ADD_AUTH();

		info->summary = ZH_WORDS_GETTER("charging.admission.add_discription");

		// 定义响应参数格式
		API_DEF_ADD_RSP_JSON_WRAPPER(StringJsonVO);
		// 定义标签
		API_DEF_ADD_TAG(API_TAG);

		// 定义其他查询参数描述

		API_DEF_ADD_PATH_PARAMS(Int64, "id", ZH_WORDS_GETTER("charging.admisssion.id"), 1, true);

		API_DEF_ADD_PATH_PARAMS(String, "name", ZH_WORDS_GETTER("charging.admisssion.name"), "1", false);

		API_DEF_ADD_PATH_PARAMS(String, "age", ZH_WORDS_GETTER("charging.admisssion.age"), 1, true);

		API_DEF_ADD_PATH_PARAMS(String, "sex", ZH_WORDS_GETTER("charging.admisssion.sex"), "man", false);

		API_DEF_ADD_PATH_PARAMS(String, "phone", ZH_WORDS_GETTER("charging.admisssion.phone"), "1", true);

		API_DEF_ADD_PATH_PARAMS(String, "admissionDept", ZH_WORDS_GETTER("charging.admisssion.admissionDept"), "1", true);

		API_DEF_ADD_PATH_PARAMS(String, "admission_ward", ZH_WORDS_GETTER("charging.admisssion.admission_ward"), "1", true);

		API_DEF_ADD_PATH_PARAMS(String, "attending_doctor", ZH_WORDS_GETTER("charging.admisssion.attending_doctor"), "1", false);

		API_DEF_ADD_PATH_PARAMS(String, "admissionDiagnosis", ZH_WORDS_GETTER("charging.admisssion.admissionDiagnosis"), "1", true);

		API_DEF_ADD_PATH_PARAMS(String, "diagnosisDesc", ZH_WORDS_GETTER("charging.admisssion.diagnosisDesc"), "1", false);

		API_DEF_ADD_PATH_PARAMS(String, "patientCondition", ZH_WORDS_GETTER("charging.admisssion.patientCondition"), "1", false);

		API_DEF_ADD_PATH_PARAMS(String, "admissionType", ZH_WORDS_GETTER("charging.admisssion.admissionType"), "1", false);

		API_DEF_ADD_PATH_PARAMS(String, "admissionMode", ZH_WORDS_GETTER("charging.admisssion.admissionMode"), "1", false);

		API_DEF_ADD_PATH_PARAMS(String, "feeNature", ZH_WORDS_GETTER("charging.admisssion.feeNature"), "1", true);

		API_DEF_ADD_PATH_PARAMS(String, "admissiondata", ZH_WORDS_GETTER("charging.admisssion.admissiondata"), "1", true);

	}

	ENDPOINT_INFO(admission_delete) {
		// 定义接口标题
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("admisssion_delete"));
		// 定义默认授权参数（可选定义，如果定义了，下面ENDPOINT里面需要加入API_HANDLER_AUTH_PARAME）
		API_DEF_ADD_AUTH();
		// 定义响应参数格式
		API_DEF_ADD_RSP_JSON_WRAPPER(StringJsonVO);

		info->summary = ZH_WORDS_GETTER("charging.admission.add_discription");
		// 定义标签
		API_DEF_ADD_TAG(API_TAG);
		// 定义其他查询参数描述

		

		API_DEF_ADD_PATH_PARAMS(String, "delete", ZH_WORDS_GETTER("charging.admisssion.delete"), " ", false);

	}

	ENDPOINT(API_M_DEL, "/charging/admission/delete", admission_delete, BODY_DTO(AdmissionDeleteDTO::Wrapper, dto), API_HANDLER_AUTH_PARAME) {
		API_HANDLER_RESP_VO(admission_delete(dto));
	}

	ENDPOINT(API_M_POST, "/charging/admission/add", admission_add, BODY_DTO(AdmissionAddDTO::Wrapper, dto), API_HANDLER_AUTH_PARAME) {
		API_HANDLER_RESP_VO(admission_add(dto));
	}

private:
	StringJsonVO::Wrapper admission_add(const AdmissionAddDTO::Wrapper& dto);

	StringJsonVO::Wrapper admission_delete(const AdmissionDeleteDTO::Wrapper& dto);
};
	

#undef API_TAG

// 0 取消API控制器使用宏
#include OATPP_CODEGEN_END(ApiController) //<- End Codegen
#endif // _SAMPLE_CONTROLLER_