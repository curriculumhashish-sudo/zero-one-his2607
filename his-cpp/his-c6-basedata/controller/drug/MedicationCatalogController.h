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

#include "domain/GlobalInclude.h"
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
#define MEDICATION_CATALOG_PATH "/api/app/medication-catalog"
#define MEDICATION_CATALOG_LEGACY_PATH "/api/app/medicationCatalog"

class MedicationCatalogController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(MedicationCatalogController);

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medication.query-page.summary"), queryMedicationCatalogPage,
		MedicationCatalogPageJsonVO::Wrapper, API_TAG,
		MedicationCatalogSwagger::addBodyExample(info, ZH_WORDS_GETTER("medication.body.page"),
			MedicationCatalogPageQuery::createShared());
		MedicationCatalogSwagger::addResponseExample(info, MedicationCatalogPageJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, MEDICATION_CATALOG_PATH "/query-all", queryMedicationCatalogPage,
		BODY_DTO(MedicationCatalogPageQuery::Wrapper, query), execQueryMedicationCatalogPage(query));

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medication.query-one.summary"), queryMedicationCatalog,
		MedicationCatalogDetailJsonVO::Wrapper, API_TAG,
		MedicationCatalogSwagger::addBodyExample(info, ZH_WORDS_GETTER("medication.body.id"),
			oatpp::String("322994171216400384"));
		MedicationCatalogSwagger::addResponseExample(info, MedicationCatalogDetailJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, MEDICATION_CATALOG_PATH "/query-one", queryMedicationCatalog,
		BODY_DTO(String, id), execQueryMedicationCatalog(id));

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medication.add.summary"), addMedicationCatalog,
		MedicationCatalogDetailJsonVO::Wrapper, API_TAG,
		MedicationCatalogSwagger::addBodyExample(info, ZH_WORDS_GETTER("medication.body.add"),
			MedicationCatalogAddDTO::createShared());
		MedicationCatalogSwagger::addResponseExample(info, MedicationCatalogDetailJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, MEDICATION_CATALOG_PATH "/add-medication", addMedicationCatalog,
		BODY_DTO(MedicationCatalogAddDTO::Wrapper, dto), execAddMedicationCatalog(dto));

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medication.modify.summary"), modifyMedicationCatalog,
		MedicationCatalogDetailJsonVO::Wrapper, API_TAG,
		MedicationCatalogSwagger::addBodyExample(info, ZH_WORDS_GETTER("medication.body.modify"),
			MedicationCatalogModifyDTO::createShared());
		MedicationCatalogSwagger::addResponseExample(info, MedicationCatalogDetailJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, MEDICATION_CATALOG_PATH "/modify-medication", modifyMedicationCatalog,
		BODY_DTO(MedicationCatalogModifyDTO::Wrapper, dto), execModifyMedicationCatalog(dto));

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medication.remove.summary"), removeMedicationCatalog,
		NoDataJsonVO::Wrapper, API_TAG,
		MedicationCatalogSwagger::addBodyExample(info, ZH_WORDS_GETTER("medication.body.ids"),
			MedicationCatalogSwagger::sampleIds());
		MedicationCatalogSwagger::addResponseExample(info, MedicationCatalogSwagger::successNoData());
	);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, MEDICATION_CATALOG_PATH "/remove-medication", removeMedicationCatalog,
		BODY_DTO(List<String>, ids), execRemoveMedicationCatalog(ids));

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medication.enable.summary"), enableMedicationCatalog,
		NoDataJsonVO::Wrapper, API_TAG,
		MedicationCatalogSwagger::addBodyExample(info, ZH_WORDS_GETTER("medication.body.ids"),
			MedicationCatalogSwagger::sampleIds());
		MedicationCatalogSwagger::addResponseExample(info, MedicationCatalogSwagger::successNoData());
	);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, MEDICATION_CATALOG_PATH "/enable-medication", enableMedicationCatalog,
		BODY_DTO(List<String>, ids), execEnableMedicationCatalog(ids));

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medication.disable.summary"), disableMedicationCatalog,
		NoDataJsonVO::Wrapper, API_TAG,
		MedicationCatalogSwagger::addBodyExample(info, ZH_WORDS_GETTER("medication.body.ids"),
			MedicationCatalogSwagger::sampleIds());
		MedicationCatalogSwagger::addResponseExample(info, MedicationCatalogSwagger::successNoData());
	);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, MEDICATION_CATALOG_PATH "/disable-medication", disableMedicationCatalog,
		BODY_DTO(List<String>, ids), execDisableMedicationCatalog(ids));

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medication.query-page.summary"), pageMedicationCatalogLegacy,
		MedicationCatalogPageJsonVO::Wrapper, API_TAG, info->hide = true;);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, MEDICATION_CATALOG_LEGACY_PATH "/pageMedicationCatalog",
		pageMedicationCatalogLegacy, BODY_DTO(MedicationCatalogPageQuery::Wrapper, query),
		execQueryMedicationCatalogPage(query));

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medication.query-one.summary"), getMedicationCatalogLegacy,
		MedicationCatalogDetailJsonVO::Wrapper, API_TAG, info->hide = true;);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, MEDICATION_CATALOG_LEGACY_PATH "/getMedicationCatalog",
		getMedicationCatalogLegacy, BODY_DTO(String, id), execQueryMedicationCatalog(id));

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medication.add.summary"), createMedicationCatalogLegacy,
		MedicationCatalogDetailJsonVO::Wrapper, API_TAG, info->hide = true;);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, MEDICATION_CATALOG_LEGACY_PATH "/createMedicationCatalog",
		createMedicationCatalogLegacy, BODY_DTO(MedicationCatalogAddDTO::Wrapper, dto),
		execAddMedicationCatalog(dto));

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medication.modify.summary"), editMedicationCatalogLegacy,
		MedicationCatalogDetailJsonVO::Wrapper, API_TAG, info->hide = true;);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, MEDICATION_CATALOG_LEGACY_PATH "/editMedicationCatalog",
		editMedicationCatalogLegacy, BODY_DTO(MedicationCatalogModifyDTO::Wrapper, dto),
		execModifyMedicationCatalog(dto));

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medication.remove.summary"), deleteMedicationCatalogLegacy,
		NoDataJsonVO::Wrapper, API_TAG, info->hide = true;);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, MEDICATION_CATALOG_LEGACY_PATH "/deleteMedicationCatalog",
		deleteMedicationCatalogLegacy, BODY_DTO(List<String>, ids), execRemoveMedicationCatalog(ids));

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medication.enable.summary"), startMedicationCatalogLegacy,
		NoDataJsonVO::Wrapper, API_TAG, info->hide = true;);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, MEDICATION_CATALOG_LEGACY_PATH "/startMedicationCatalog",
		startMedicationCatalogLegacy, BODY_DTO(List<String>, ids), execEnableMedicationCatalog(ids));

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medication.disable.summary"), stopMedicationCatalogLegacy,
		NoDataJsonVO::Wrapper, API_TAG, info->hide = true;);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, MEDICATION_CATALOG_LEGACY_PATH "/stopMedicationCatalog",
		stopMedicationCatalogLegacy, BODY_DTO(List<String>, ids), execDisableMedicationCatalog(ids));

private:
	MedicationCatalogPageJsonVO::Wrapper execQueryMedicationCatalogPage(const MedicationCatalogPageQuery::Wrapper& query);
	MedicationCatalogDetailJsonVO::Wrapper execQueryMedicationCatalog(const String& id);
	MedicationCatalogDetailJsonVO::Wrapper execAddMedicationCatalog(const MedicationCatalogAddDTO::Wrapper& dto);
	MedicationCatalogDetailJsonVO::Wrapper execModifyMedicationCatalog(const MedicationCatalogModifyDTO::Wrapper& dto);
	NoDataJsonVO::Wrapper execRemoveMedicationCatalog(const List<String>& ids);
	NoDataJsonVO::Wrapper execEnableMedicationCatalog(const List<String>& ids);
	NoDataJsonVO::Wrapper execDisableMedicationCatalog(const List<String>& ids);
};

#undef MEDICATION_CATALOG_LEGACY_PATH
#undef MEDICATION_CATALOG_PATH
#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif // !_MEDICATION_CATALOG_CONTROLLER_H_
