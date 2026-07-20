#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: bandao
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
#include "domain/dto/drug/MedicationCatalogDTO.h"
#include "domain/vo/drug/MedicationCatalogVO.h"
#include "domain/vo/NoDataJsonVO.h"

using namespace oatpp;

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
		ids->push_back("10001");
		ids->push_back("10002");
		return ids;
	}

	static MedicationCatalogModifyDTO::Wrapper sampleEdit()
	{
		auto dto = MedicationCatalogModifyDTO::createShared();
		fillRequiredEditFields(dto);
		return dto;
	}

	static MedicationCatalogDetailJsonVO::Wrapper successDetail()
	{
		auto detail = MedicationCatalogDetailVO::createShared();
		fillRequiredEditFields(detail);
		detail->specification = detail->totalVolume;

		auto result = MedicationCatalogDetailJsonVO::createShared();
		result->success(detail);
		return result;
	}

	static NoDataJsonVO::Wrapper successNoData()
	{
		auto result = NoDataJsonVO::createShared();
		result->init(RS_SUCCESS);
		return result;
	}

private:
	template<class T>
	static void fillRequiredEditFields(const T& dto)
	{
		dto->id = "10001";
		dto->name = ZH_WORDS_GETTER("medication.example.name");
		dto->categoryCode = "FL001";
		dto->merchandiseName = ZH_WORDS_GETTER("medication.example.merchandiseName");
		dto->purchaseUnitCode = "DW001";
		dto->splitUnitCode = "DW002";
		dto->pharmacologyCategoryCode = "YL001";
		dto->manufacturerText = ZH_WORDS_GETTER("medication.example.manufacturerText");
		dto->supplyId = "60001";
		dto->partAttributeEnum = "1";
		dto->thoPartAttributeEnum = "1";
		dto->typeCode = "CW001";
		dto->totalVolume = "5ml:0.5g";
		dto->doseFormCode = "JX001";
		dto->doseUnitCode = "DW001";
		dto->packageUnitCode = "DW003";
		dto->stockUnitCode = "DW004";
		dto->doseUnitRatio = 10.0;
		dto->purchaseUnitRatio = 10.0;
		dto->stockUnitRatio = 10.0;
		dto->packageUnitRatio = 10.0;
		dto->purchasePrice = 10.5;
		dto->retailPrice = 15.8;
	}
};

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("medication.tag")
#define MEDICATION_CATALOG_PATH "/api/app/medicationCatalog"

class MedicationCatalogController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(MedicationCatalogController);

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medication.edit.summary"), medicationCatalog_editMedicationCatalog,
		MedicationCatalogDetailJsonVO::Wrapper, API_TAG,
		MedicationCatalogSwagger::addBodyExample(info, ZH_WORDS_GETTER("medication.body.edit"),
			MedicationCatalogSwagger::sampleEdit());
		MedicationCatalogSwagger::addResponseExample(info, MedicationCatalogSwagger::successDetail());
	);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, MEDICATION_CATALOG_PATH "/editMedicationCatalog",
		medicationCatalog_editMedicationCatalog,
		BODY_DTO(MedicationCatalogModifyDTO::Wrapper, dto), execEditMedicationCatalog(dto));

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medication.delete.summary"), medicationCatalog_deleteMedicationCatalog,
		NoDataJsonVO::Wrapper, API_TAG,
		MedicationCatalogSwagger::addBodyExample(info, ZH_WORDS_GETTER("medication.body.ids"),
			MedicationCatalogSwagger::sampleIds());
		MedicationCatalogSwagger::addResponseExample(info, MedicationCatalogSwagger::successNoData());
	);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, MEDICATION_CATALOG_PATH "/deleteMedicationCatalog",
		medicationCatalog_deleteMedicationCatalog,
		BODY_DTO(List<String>, ids), execDeleteMedicationCatalog(ids));

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medication.start.summary"), medicationCatalog_startMedicationCatalog,
		NoDataJsonVO::Wrapper, API_TAG,
		MedicationCatalogSwagger::addBodyExample(info, ZH_WORDS_GETTER("medication.body.ids"),
			MedicationCatalogSwagger::sampleIds());
		MedicationCatalogSwagger::addResponseExample(info, MedicationCatalogSwagger::successNoData());
	);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, MEDICATION_CATALOG_PATH "/startMedicationCatalog",
		medicationCatalog_startMedicationCatalog,
		BODY_DTO(List<String>, ids), execStartMedicationCatalog(ids));

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medication.stop.summary"), medicationCatalog_stopMedicationCatalog,
		NoDataJsonVO::Wrapper, API_TAG,
		MedicationCatalogSwagger::addBodyExample(info, ZH_WORDS_GETTER("medication.body.ids"),
			MedicationCatalogSwagger::sampleIds());
		MedicationCatalogSwagger::addResponseExample(info, MedicationCatalogSwagger::successNoData());
	);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, MEDICATION_CATALOG_PATH "/stopMedicationCatalog",
		medicationCatalog_stopMedicationCatalog,
		BODY_DTO(List<String>, ids), execStopMedicationCatalog(ids));

private:
	MedicationCatalogDetailJsonVO::Wrapper execEditMedicationCatalog(const MedicationCatalogModifyDTO::Wrapper& dto);
	NoDataJsonVO::Wrapper execDeleteMedicationCatalog(const List<String>& ids);
	NoDataJsonVO::Wrapper execStartMedicationCatalog(const List<String>& ids);
	NoDataJsonVO::Wrapper execStopMedicationCatalog(const List<String>& ids);
};

#undef MEDICATION_CATALOG_PATH
#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif // !_MEDICATION_CATALOG_CONTROLLER_H_
