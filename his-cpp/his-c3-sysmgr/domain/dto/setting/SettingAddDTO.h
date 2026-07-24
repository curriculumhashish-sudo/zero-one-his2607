#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author:
 @Date: 2026/07/21

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

#include "domain/GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class SettingAddDTO : public oatpp::DTO {
	DTO_INIT(SettingAddDTO, DTO);

	API_DTO_FIELD(String, name, "setting key name", true, "");
	API_DTO_FIELD(String, value, "setting value", true, "");
	API_DTO_FIELD(String, providerName, "provider type", false, "");
	API_DTO_FIELD(String, providerKey, "provider key", false, "");
	API_DTO_FIELD(String, tenantId, "tenant id", false, "");
	API_DTO_FIELD(String, description, "description", false, "");
};

#include OATPP_CODEGEN_END(DTO)
