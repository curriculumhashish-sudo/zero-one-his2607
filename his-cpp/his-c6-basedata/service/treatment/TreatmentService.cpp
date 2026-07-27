/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2022/10/25 11:13:11

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
#include "stdafx.h"
#include "TreatmentService.h"
//#include "../../dao/sample/TreatmentDAO.h"
#include "id/UuidFacade.h"
//#include "TreatmentDateTimeFormat.h"

TreatmentPageDTO::Wrapper TreatmentService::listAll(const TreatmentQuery::Wrapper& query)
{
	return {};
}

TreatmentDetailDTO::Wrapper TreatmentService::getById(std::string id)
{
	// 空实现：后续接入 DAO/Mapper，按 id 查询并组装 TreatmentDetailDTO
	return nullptr;
}

std::string TreatmentService::saveData(const TreatmentAddDTO::Wrapper& dto)
{
	// 空实现：后续接入 DAO，写入主表/子项/价格并返回新 id
	return "";
}

bool TreatmentService::updateData(const TreatmentModifyDTO::Wrapper& dto)
{
	// 空实现：后续接入 DAO，更新主表/子项/价格
	return false;
}

bool TreatmentService::removeData(const oatpp::List<oatpp::String>& ids)
{
	return {};
}
