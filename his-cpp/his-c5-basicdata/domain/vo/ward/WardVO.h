#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Lfff
 @Date: 2026/07/23 18:58:00

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
#ifndef _WARD_VO_H_
#define _WARD_VO_H_

#include "ApiHelper.h"
#include "../../GlobalInclude.h"
#include "../../dto/ward/WardDTO.h"
#include "domain/vo/JsonVO.h"
using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(DTO)

// Ward room VO (single)
class WardRoomJsonVO : public JsonVO<WardRoomDTO::Wrapper> {
	DTO_INIT(WardRoomJsonVO, JsonVO<WardRoomDTO::Wrapper>);
};

// Ward room list VO
class WardRoomListJsonVO : public JsonVO< List< WardRoomDTO::Wrapper > > {
	DTO_INIT(WardRoomListJsonVO, JsonVO< List< WardRoomDTO::Wrapper > >);
};

// Ward bed VO (single)
class WardBedJsonVO : public JsonVO<WardBedDTO::Wrapper> {
	DTO_INIT(WardBedJsonVO, JsonVO<WardBedDTO::Wrapper>);
};

// Ward bed list VO
class WardBedListJsonVO : public JsonVO< List< WardBedDTO::Wrapper > > {
	DTO_INIT(WardBedListJsonVO, JsonVO< List< WardBedDTO::Wrapper > >);
};

// Ward main VO
class WardVO : public oatpp::DTO
{
	DTO_INIT(WardVO, DTO);
	API_DTO_FIELD(String, id, "Ward ID", true, "322955305339588608");
	API_DTO_FIELD(String, name, "Ward name", true, "Internal Medicine Ward");
	API_DTO_FIELD(String, busNo, "Business number", true, "LO0028");
	API_DTO_FIELD(String, statusEnum, "Status enum", true, "ACTIVE");
	API_DTO_FIELD(String, operationalEnum, "Operational enum", false, "NOT_IN_USE");
	API_DTO_FIELD(String, formEnum, "Location form enum", true, "WARD");
	API_DTO_FIELD(String, modeEnum, "Location mode enum", true, "INSTANCE");
	API_DTO_FIELD(String, typeJson, "Type JSON snapshot", false, "{}");
	API_DTO_FIELD(String, organizationId, "Organization ID", true, "322663308812357632");
	API_DTO_FIELD(Int32, displayOrder, "Display order", false, 1);
	API_DTO_FIELD(String, pyStr, "Pinyin code", false, "nklq");
	API_DTO_FIELD(String, wbStr, "Wubi code", false, "maui");
	API_DTO_FIELD(String, parentId, "Parent location ID", false, "");
	API_DTO_FIELD(Int32, version, "Optimistic lock version", true, 0);
};

// Ward JSON VO
class WardJsonVO : public JsonVO<WardVO::Wrapper>
{
	DTO_INIT(WardJsonVO, JsonVO<WardVO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // _WARD_VO_H_
