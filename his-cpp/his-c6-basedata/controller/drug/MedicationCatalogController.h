#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: axiao
 @Date: 2026/07/20

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#ifndef _MEDICATION_CATALOG_CONTROLLER_H_
#define _MEDICATION_CATALOG_CONTROLLER_H_

#include "ApiHelper.h"
#include "domain/query/drug/MedicationCatalogQuery.h"
#include "domain/dto/drug/MedicationCatalogDTO.h"
#include "domain/vo/drug/MedicationCatalogVO.h"
#include "domain/vo/NoDataJsonVO.h"

#ifdef _RELEASE_DOC_
class MedicationCatalogDocAuthorizeHandler final : public oatpp::web::server::handler::BearerAuthorizationHandler
{
public:
	std::shared_ptr<AuthorizationObject> authorize(const oatpp::String& token) override
	{
		PayloadDTO payload;
		payload.setCode(PayloadCode::SUCCESS);
		payload.setToken(token ? token->c_str() : "");
		return std::make_shared<CustomerAuthorizeObject>(payload);
	}
};
#endif

class MedicationCatalogSwagger final
{
public:
	template<class T>
	static void addBodyExample(const std::shared_ptr<oatpp::web::server::api::Endpoint::Info>& info,
		const oatpp::String& description, const T& example)
	{
		info->body.description = description;
		info->body.required = true;
		info->body.addExample("default", example);
	}

	template<class T>
	static void addResponseExample(const std::shared_ptr<oatpp::web::server::api::Endpoint::Info>& info,
		const T& example)
	{
		info->responses[oatpp::web::protocol::http::Status::CODE_200].addExample("success", example);
	}

	static oatpp::List<oatpp::String> sampleIds()
	{
		auto ids = oatpp::List<oatpp::String>::createShared();
		ids->push_back("322994171216400384");
		ids->push_back("322994171216400385");
		return ids;
	}

	static NoDataJsonVO::Wrapper successNoData()
	{
		auto result = NoDataJsonVO::createShared();
		result->init(RS_SUCCESS);
		return result;
	}
};

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("medication.tag")
#define MEDICATION_CATALOG_PATH "/api/app/medicationCatalog"

class MedicationCatalogController : public oatpp::web::server::api::ApiController
{
public:
	MedicationCatalogController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
		: oatpp::web::server::api::ApiController(objectMapper)
	{
#ifdef _RELEASE_DOC_
		setDefaultAuthorizationHandler(std::make_shared<MedicationCatalogDocAuthorizeHandler>());
#else
		setDefaultAuthorizationHandler(std::make_shared<CustomerAuthorizeHandler>());
#endif
	}

	static std::shared_ptr<MedicationCatalogController> createShared(
		OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
	{
		return std::make_shared<MedicationCatalogController>(objectMapper);
	}

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medication.page.summary"), pageMedicationCatalog,
		MedicationCatalogPageJsonVO::Wrapper, API_TAG,
		MedicationCatalogSwagger::addBodyExample(info, ZH_WORDS_GETTER("medication.body.page"),
			MedicationCatalogPageQuery::createShared());
		MedicationCatalogSwagger::addResponseExample(info, MedicationCatalogPageJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, MEDICATION_CATALOG_PATH "/pageMedicationCatalog", pageMedicationCatalog,
		BODY_DTO(MedicationCatalogPageQuery::Wrapper, query), execPageMedicationCatalog(query));

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medication.get.summary"), getMedicationCatalog,
		MedicationCatalogDetailJsonVO::Wrapper, API_TAG,
		MedicationCatalogSwagger::addBodyExample(info, ZH_WORDS_GETTER("medication.body.id"),
			oatpp::String("322994171216400384"));
		MedicationCatalogSwagger::addResponseExample(info, MedicationCatalogDetailJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, MEDICATION_CATALOG_PATH "/getMedicationCatalog", getMedicationCatalog,
		BODY_DTO(String, id), execGetMedicationCatalog(id));

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medication.create.summary"), createMedicationCatalog,
		MedicationCatalogDetailJsonVO::Wrapper, API_TAG,
		MedicationCatalogSwagger::addBodyExample(info, ZH_WORDS_GETTER("medication.body.create"),
			MedicationCatalogCreateDTO::createShared());
		MedicationCatalogSwagger::addResponseExample(info, MedicationCatalogDetailJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, MEDICATION_CATALOG_PATH "/createMedicationCatalog", createMedicationCatalog,
		BODY_DTO(MedicationCatalogCreateDTO::Wrapper, dto), execCreateMedicationCatalog(dto));

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medication.edit.summary"), editMedicationCatalog,
		MedicationCatalogDetailJsonVO::Wrapper, API_TAG,
		MedicationCatalogSwagger::addBodyExample(info, ZH_WORDS_GETTER("medication.body.edit"),
			MedicationCatalogUpdateDTO::createShared());
		MedicationCatalogSwagger::addResponseExample(info, MedicationCatalogDetailJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, MEDICATION_CATALOG_PATH "/editMedicationCatalog", editMedicationCatalog,
		BODY_DTO(MedicationCatalogUpdateDTO::Wrapper, dto), execEditMedicationCatalog(dto));

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medication.delete.summary"), deleteMedicationCatalog,
		NoDataJsonVO::Wrapper, API_TAG,
		MedicationCatalogSwagger::addBodyExample(info, ZH_WORDS_GETTER("medication.body.ids"),
			MedicationCatalogSwagger::sampleIds());
		MedicationCatalogSwagger::addResponseExample(info, MedicationCatalogSwagger::successNoData());
	);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, MEDICATION_CATALOG_PATH "/deleteMedicationCatalog", deleteMedicationCatalog,
		BODY_DTO(List<String>, ids), execDeleteMedicationCatalog(ids));

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medication.start.summary"), startMedicationCatalog,
		NoDataJsonVO::Wrapper, API_TAG,
		MedicationCatalogSwagger::addBodyExample(info, ZH_WORDS_GETTER("medication.body.ids"),
			MedicationCatalogSwagger::sampleIds());
		MedicationCatalogSwagger::addResponseExample(info, MedicationCatalogSwagger::successNoData());
	);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, MEDICATION_CATALOG_PATH "/startMedicationCatalog", startMedicationCatalog,
		BODY_DTO(List<String>, ids), execStartMedicationCatalog(ids));

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medication.stop.summary"), stopMedicationCatalog,
		NoDataJsonVO::Wrapper, API_TAG,
		MedicationCatalogSwagger::addBodyExample(info, ZH_WORDS_GETTER("medication.body.ids"),
			MedicationCatalogSwagger::sampleIds());
		MedicationCatalogSwagger::addResponseExample(info, MedicationCatalogSwagger::successNoData());
	);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, MEDICATION_CATALOG_PATH "/stopMedicationCatalog", stopMedicationCatalog,
		BODY_DTO(List<String>, ids), execStopMedicationCatalog(ids));

private:
	MedicationCatalogPageJsonVO::Wrapper execPageMedicationCatalog(const MedicationCatalogPageQuery::Wrapper& query);
	MedicationCatalogDetailJsonVO::Wrapper execGetMedicationCatalog(const String& id);
	MedicationCatalogDetailJsonVO::Wrapper execCreateMedicationCatalog(const MedicationCatalogCreateDTO::Wrapper& dto);
	MedicationCatalogDetailJsonVO::Wrapper execEditMedicationCatalog(const MedicationCatalogUpdateDTO::Wrapper& dto);
	NoDataJsonVO::Wrapper execDeleteMedicationCatalog(const List<String>& ids);
	NoDataJsonVO::Wrapper execStartMedicationCatalog(const List<String>& ids);
	NoDataJsonVO::Wrapper execStopMedicationCatalog(const List<String>& ids);
};

#undef MEDICATION_CATALOG_PATH
#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif // !_MEDICATION_CATALOG_CONTROLLER_H_
