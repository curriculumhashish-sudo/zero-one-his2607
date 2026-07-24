/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei

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
#include "DiagnoseService.h"
#include "../../dao/Diagnosis/DiagnosisDAO.h"
#include "../../lib-common/include/id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"

std::string DiagnoseService::saveData(const DiagnoseAddDTO::Wrapper& dto)
{
	DiagnosisDO data;
	ZO_STAR_DOMAIN_DTO_TO_DO(
		data,
		dto,
		DiagnosisName, name,
		DiagnosisType, diagType,
		DiagnosisCode, chronicCode,
		MedicalInsuranceCode, medicalInsuranceCode,
		Status, status,
		IsDelete, isDelete
	);

	ZO_STAR_DOMAIN_DTO_TO_DO(
		data,
		dto,
		MedicalInsuranceTag, medicalInsuranceTag,
		MedicalInsuranceMatch, medicalInsuranceMatch,
		PinyinCode, pinyinCode,
		WubiCode, wubiCode,
		Remark, remark,
		CatName, catName,
		TenantId,tenantId,
		Source_enum, source_enum);

	UuidFacade uuidFacade;
	data.setId(uuidFacade.genUuid());
	data.setCreateBy(dto->getPayload()->getUsername());
	data.setCreateTime(SimpleDateTimeFormat::format());
	//data.setIsDelete(0);
	data.setVersion(1);

	DiagnosisDAO dao;
	return dao.insert(data) == 1 ? data.getId() : "";
}

bool DiagnoseService::updateData(const DiagnoseDTO::Wrapper& dto)
{
	DiagnosisDO data;
	ZO_STAR_DOMAIN_DTO_TO_DO(
		data,
		dto,
		DiagnosisName, name,
		DiagnosisType, diagType,
		DiagnosisCode, chronicCode,
		MedicalInsuranceCode, medicalInsuranceCode,
		Status, status,
		TenantId, tenantId,
		IsDelete, isDelete,
		Id, id);

	ZO_STAR_DOMAIN_DTO_TO_DO(
		data,
		dto,
		MedicalInsuranceTag, medicalInsuranceTag,
		MedicalInsuranceMatch, medicalInsuranceMatch,
		PinyinCode, pinyinCode,
		WubiCode, wubiCode,
		Remark, remark,
		Source_enum,source_enum,
	Version,version);

	data.setUpdateBy(dto->getPayload()->getUsername());
	data.setUpdateTime(SimpleDateTimeFormat::format());

	DiagnosisDAO dao;
	return dao.update(data) == 1;
}

//删除一条数据（前端有）
bool DiagnoseService::removeOne(const std::string& id)
{
	DiagnosisDO data;
	data.setIsDelete(1);
	data.setUpdateTime(SimpleDateTimeFormat::format());

	DiagnosisDAO dao;
	return dao.update(data) == 1;
}
//物理批量删除
bool DiagnoseService::removeData(const oatpp::List<oatpp::String>& ids)
{
	std::list<std::string> dataIds;
	for (const auto& id : *ids.get())
	{
		dataIds.emplace_back(id.getValue({}));
	}

	DiagnosisDAO dao;
	return dao.deleteByIds<DiagnosisDO>(dataIds) == ids->size();
}

bool DiagnoseService::batchChangeStatus(
	const oatpp::List<oatpp::String>& ids,
	oatpp::String status)
{
	DiagnosisDAO dao;
	int successCount = 0;
	for (const auto& idItem : *ids.get())
	{
		DiagnosisDO data;
		data.setId(idItem.getValue({}));
		data.setStatus(*status);
		data.setUpdateTime(SimpleDateTimeFormat::format());
		if (dao.update(data) == 1)
		{
			++successCount;
		}
	}
	return successCount > 0;
}
