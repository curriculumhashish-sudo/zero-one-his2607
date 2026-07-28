<<<<<<< HEAD
 #include "DiagnoseService.h"
#include "../../dao/Diagnose/DiagnoseDAO.h"
#include "../../lib-common/include/id/UuidFacade.h"
=======
�?*
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
#include "../../dao/Diagnose/DiagnoseDAO.h"
#include "id/UuidFacade.h"
>>>>>>> 7a964f5c8d17ed83b384e758fad9c7e6fabfd56b
#include "SimpleDateTimeFormat.h"

#include <charconv>
static const std::string SOURCE_GENERAL_diagnose = "general_dx";
static const std::string SOURCE_CHRONIC_SPECIAL = "chronic_spec";
static const std::string SOURCE_DISEASE_PACKAGE = "disease_pkg";
static const std::string SOURCE_PAID_diagnose = "paid_dx";
static const std::string SOURCE_DAY_SURGERY = "day_surgery_dx";
static const std::string SOURCE_TCM_diagnose = "tcm_dx";
static const std::string SOURCE_TCM_SYNDROME = "tcm_syndrome";
static const std::string SOURCE_TUMOR_MORPHOLOGY = "tumor_morph";
static const std::string diagnose_KIND_TCM = "TCM";
static const std::string diagnose_KIND_WESTERN = "WESTERN";
static const std::string STATUS_ACTIVE = "ACTIVE";
static constexpr int STATUS_ACTIVE_SORT_ORDER = 2;

static bool tryParseInt32(const std::string& value, int32_t& result)
{
	if (value.empty())
	{
		return false;
	}
	const char* begin = value.data();
	const char* end = begin + value.size();
	const auto parsed = std::from_chars(begin, end, result);
	return parsed.ec == std::errc() && parsed.ptr == end;
}

static bool isTraditionalChineseMedicineSource(const std::string& sourceEnum)
{
	return sourceEnum == SOURCE_TCM_diagnose || sourceEnum == SOURCE_TCM_SYNDROME;
}

static bool isWesternMedicineSource(const std::string& sourceEnum)
{
	return sourceEnum == SOURCE_GENERAL_diagnose
		|| sourceEnum == SOURCE_CHRONIC_SPECIAL
		|| sourceEnum == SOURCE_DISEASE_PACKAGE
		|| sourceEnum == SOURCE_PAID_diagnose
		|| sourceEnum == SOURCE_DAY_SURGERY
		|| sourceEnum == SOURCE_TUMOR_MORPHOLOGY;
}

static std::string getSourceEnumText(const std::string& sourceEnum)
{
	if (!isWesternMedicineSource(sourceEnum) &&
		!isTraditionalChineseMedicineSource(sourceEnum))
	{
		return "";
	}
	return ZH_WORDS_GETTER("diagnose.source." + sourceEnum);
}

<<<<<<< HEAD
DiagnoseSelectPageDTO::Wrapper DiagnoseService::listSelectableDiagnoses(
	const DiagnoseSelectQuery::Wrapper& query)
{
	auto page = DiagnoseSelectPageDTO::createShared();
	page->pageIndex = query->pageIndex;
	page->pageSize = query->pageSize;

	DiagnoseDAO dao;
	const uint64_t total = dao.count(query);
=======
DiagnoseSelectPageDTO::Wrapper DiagnoseService::listSelectableDiagnoses(
	const DiagnoseSelectQuery::Wrapper& query,
	const PayloadDTO& payload)
{
	if (!query || !query->validate().empty() || payload.getId().empty())
	{
		return nullptr;
	}

	DiagnoseDAO dao;
	const std::string tenantId = dao.selectCurrentTenantId(payload.getId());
	if (tenantId.empty())
	{
		return nullptr;
	}

	auto page = DiagnoseSelectPageDTO::createShared();
	page->pageIndex = query->pageIndex;
	page->pageSize = query->pageSize;

	const uint64_t total = dao.count(query, tenantId);
>>>>>>> 7a964f5c8d17ed83b384e758fad9c7e6fabfd56b
	page->total = static_cast<int64_t>(total);
	page->calcPages();
	if (total == 0)
	{
		return page;
	}

	std::list<ClnConditionDefinitionDO> records = dao.selectWithPage(query, tenantId);
	for (ClnConditionDefinitionDO& record : records)
	{
<<<<<<< HEAD
		auto dto = DiagnoseSelectDTO::createShared();
		dto->conditionDefinitionId = record.getId();
		dto->conditionCode = record.getConditionCode();
		dto->conditionDefinitionName = record.getName();
		dto->pyStr = record.getPyStr();
		dto->wbStr = record.getWbStr();
=======
		auto dto = DiagnoseSelectDTO::createShared();
		ZO_STAR_DOMAIN_DO_TO_DTO(
			dto,
			record,
			conditionDefinitionId, Id,
			conditionCode, ConditionCode,
			conditionDefinitionName, Name,
			pyStr, PyStr,
			wbStr, WbStr);
>>>>>>> 7a964f5c8d17ed83b384e758fad9c7e6fabfd56b

		const std::string sourceEnum = record.getSourceEnum();
		const bool isTcm = isTraditionalChineseMedicineSource(sourceEnum);
		if (isTcm)
		{
<<<<<<< HEAD
			dto->diagnoseKind = "TCM";
			dto->diagnoseKindText = ZH_WORDS_GETTER("diagnose.type.tcm");
		}
		else if (isWesternMedicineSource(sourceEnum))
		{
			dto->diagnoseKind = "WESTERN";
			dto->diagnoseKindText = ZH_WORDS_GETTER("diagnose.type.western");
=======
			dto->diagnoseKind = diagnose_KIND_TCM;
			dto->diagnoseKindText = ZH_WORDS_GETTER("diagnose.type.tcm");
		}
		else if (isWesternMedicineSource(sourceEnum))
		{
			dto->diagnoseKind = diagnose_KIND_WESTERN;
			dto->diagnoseKindText = ZH_WORDS_GETTER("diagnose.type.western");
>>>>>>> 7a964f5c8d17ed83b384e758fad9c7e6fabfd56b
		}
		else
		{
			dto->diagnoseKind = nullptr;
			dto->diagnoseKindText = nullptr;
		}

		int32_t sourceEnumValue = 0;
		dto->sourceEnum = tryParseInt32(sourceEnum, sourceEnumValue)
			? oatpp::Int32(sourceEnumValue)
			: oatpp::Int32(nullptr);
		const std::string sourceEnumText = getSourceEnumText(sourceEnum);
		dto->sourceEnumText = sourceEnumText.empty()
			? oatpp::String(nullptr)
			: oatpp::String(sourceEnumText.c_str());

		const int ybFlag = record.getYbFlag() == 1 ? 1 : 0;
		dto->ybFlag = ybFlag;
		dto->ybFlagText = ybFlag == 1
			? ZH_WORDS_GETTER("diagnose.flag.yes")
			: ZH_WORDS_GETTER("diagnose.flag.no");
		dto->statusEnum = record.getStatusEnum() == STATUS_ACTIVE
			? oatpp::Int32(STATUS_ACTIVE_SORT_ORDER)
			: oatpp::Int32(nullptr);
		page->addData(dto);
	}
	return page;
}

std::string DiagnoseService::saveData(const DiagnoseAddDTO::Wrapper& dto)
{
<<<<<<< HEAD
	DiagnoseDO data;
	ZO_STAR_DOMAIN_DTO_TO_DO(
		data,
		dto,
		DiagnoseName, name,
		DiagnoseType, diagType,
		DiagnoseCode, chronicCode,
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
=======
    // 1. 入参DTO �?数据库DO
    DiagnoseDO data;
    ZO_STAR_DOMAIN_DTO_TO_DO(data, dto,
        DiagnoseName, name,//诊断名称
        //CategoryId, categoryId,//诊断目录id外键
        DiagnoseType, diagType,//诊断类型
        DiagnoseCode, chronicCode,//诊断编码
        MedicalInsuranceCode, medicalInsuranceCode,//医保编码
        Status, status,//状�?
        IsDelete, isDelete//删除标记
    );

    ZO_STAR_DOMAIN_DTO_TO_DO(data, dto,
        MedicalInsuranceTag, medicalInsuranceTag,//医保标签
        MedicalInsuranceMatch, medicalInsuranceMatch,//医保对码（T或F�?
        PinyinCode, pinyinCode,//拼音助记
        WubiCode, wubiCode,//五笔助记
        Remark, remark,//说明
        CatName,catName//分类项目�?
        );

    // 2. 生成全局唯一主键ID
    UuidFacade uf;
    data.setId(uf.genUuid());
>>>>>>> parent of d02f5cf44 (axiao完成诊疗列表查询功能)

    // 3. 从DTO内嵌的登录载荷取创建人（匹配CC_SYNTHESIZE生成的getPayload�?
    data.setCreateBy(dto->getPayload()->getUsername());
    // 4. 自动填充创建时间
    data.setCreateTime(SimpleDateTimeFormat::format());
    // 5. 逻辑删除标记默认正常
    data.setIsDelete(0);

    // 6. 执行入库，成功返回主键ID，失败返回空�?
    DiagnoseDAO dao;
    return dao.insert(data) == 1 ? data.getId() : "";
}
// 根据ID修改诊断记录
bool DiagnoseService::updateData(const DiagnoseDTO::Wrapper& dto)
{
<<<<<<< HEAD
	DiagnoseDO data;
	ZO_STAR_DOMAIN_DTO_TO_DO(
		data,
		dto,
		DiagnoseName, name,
		DiagnoseType, diagType,
		DiagnoseCode, chronicCode,
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
	data.setVersion(*(dto->version)+1);
	DiagnoseDAO dao;
	return dao.update(data) == 1;
}

<<<<<<< HEAD
//ɾ��һ�����ݣ�ǰ���У�
=======
    DiagnoseDAO dao;

    //// 1. 先按主键查询原有数据，避免全量更新覆盖未传字�?
    //DiagnoseDO data = dao.selectById(dto->getId()->getValue({}));
    //if (data.getId().empty()) {
    //    return false; // 数据不存在直接返回失�?
    //}
    DiagnoseDO data;
    // 2. 入参DTO 覆盖更新到数据库DO
    ZO_STAR_DOMAIN_DTO_TO_DO(data, dto,
        DiagnoseName, name,       //诊断名称
        //CategoryId, categoryId,//诊断目录id外键
        DiagnoseType, diagType,   //诊断类型
        DiagnoseCode, chronicCode,//诊断编码
        MedicalInsuranceCode, medicalInsuranceCode, //医保编码
        Status, status             //状�?
    );

    ZO_STAR_DOMAIN_DTO_TO_DO(data, dto,
        MedicalInsuranceTag, medicalInsuranceTag,    //医保标签
        MedicalInsuranceMatch, medicalInsuranceMatch,//医保对码
        PinyinCode, pinyinCode,    //拼音助记
        WubiCode, wubiCode,        //五笔助记
        Remark, remark             //说明
    );

    // 3. 从登录载荷取操作人，填充修改�?
    data.setUpdateBy(dto->getPayload()->getUsername());

    // 4. 自动填充修改时间
    data.setUpdateTime(SimpleDateTimeFormat::format());

    // 5. 执行更新，影响行数为1则成�?
    return dao.update(data) == 1;
}
// 逻辑单条删除
>>>>>>> parent of d02f5cf44 (axiao完成诊疗列表查询功能)
=======
// Logically deletes one diagnose record.
>>>>>>> 7a964f5c8d17ed83b384e758fad9c7e6fabfd56b
bool DiagnoseService::removeOne(const std::string& id)
{
    DiagnoseDAO dao;

    // 1. 查询原有数据，保证数据存在且避免全量更新丢字�?
  /*  DiagnoseDO data = dao.selectById(id);
    if (data.getId().empty()) {
        return false;
    }*/

    DiagnoseDO data;
    // 2. 标记逻辑删除
    data.setIsDelete(1);

    // 3. 填充修改时间
    data.setUpdateTime(SimpleDateTimeFormat::format());

    // 注：若需记录删除操作人，需补充操作人参数，调用 data.setUpdateBy(操作人用户名);

    // 4. 执行更新完成逻辑删除
    return dao.update(data) == 1;
}
<<<<<<< HEAD
<<<<<<< HEAD
//��������ɾ��
=======
// 逻辑批量删除（停用）诊断记录
>>>>>>> parent of d02f5cf44 (axiao完成诊疗列表查询功能)
=======
// Physically deletes a diagnose batch.
>>>>>>> 7a964f5c8d17ed83b384e758fad9c7e6fabfd56b
bool DiagnoseService::removeData(const oatpp::List<oatpp::String>& ids)
{
    DiagnoseDAO dao;
    //int successCount = 0;

    // 遍历ID列表逐个执行逻辑删除
    //std::list<std::string> datas;
    //for (auto& idItem : *ids) {
    //    std::string id = idItem->getValue({});
    //    DiagnoseDO data = dao.selectById(id);

    //    if (data.getId().empty()) {
    //        continue; // 不存在的ID跳过
    //    }

    //    data.setIsDelete(1);
    //    data.setUpdateTime(SimpleDateTimeFormat::format());
    //    // 同上，需记录操作人可在此统一设置

    //    if (dao.update(data) == 1) {
    //        successCount++;
    //    }
    //}
    std::list<std::string> datas;
    for (auto one : *ids.get())
        datas.emplace_back(one.getValue({}));
    int rows = dao.deleteByIds<DiagnoseDO>(datas);
    if (rows == ids->size())
        return true;
    return false;
 
}
<<<<<<< HEAD

bool DiagnoseService::batchChangeStatus(const DiagnoseBatchStatusDTO::Wrapper& dto,oatpp::String status)
{
	const oatpp::List<oatpp::String> ids = dto->ids;
	DiagnoseDAO dao;
	int successCount = 0;
	for (const auto& idItem : *ids.get())
	{
		DiagnoseDO data;
		data.setId(idItem.getValue({}));
		data.setStatus(*status);
		data.setUpdateBy(dto->getPayload()->getUsername());
		data.setUpdateTime(SimpleDateTimeFormat::format());
		data.setVersion(*(dto->version) + 1);
		if (dao.update(data) == 1)
		{
			++successCount;
		}
	}
	return successCount > 0;
}
// Logically deletes a diagnose batch.
bool DiagnoseService::execRemoveData(const DiagnoseRemoveDTO::Wrapper& dto)
{
	const oatpp::List<oatpp::String> ids = dto->ids;
	DiagnoseDAO dao;
	int successCount = 0;
	for (const auto& idItem : *ids.get())
	{
		DiagnoseDO data;
		data.setId(idItem.getValue({}));
		data.setDeletedBy(dto->getPayload()->getUsername());
		data.setDeletedTime(SimpleDateTimeFormat::format());
		data.setIsDelete(1);
		data.setVersion(*(dto->version) + 1);

		if (dao.update(data) == 1)
		{
			++successCount;
		}
	}
	return successCount > 0;

<<<<<<< HEAD
=======
bool DiagnoseService::batchChangeStatus(const oatpp::List<oatpp::String>& ids, oatpp::Int32 status)
{
    DiagnoseDAO dao;
    int successCount = 0;

    // 直接更新状态；业务约定传入ID一定存在，无需查询判空
    for (auto& idItem : *ids.get())
    {
        std::string id = idItem.getValue({});
        DiagnoseDO data;
        // 主键用于定位更新�?
        data.setId(id);
        // 设置目标启用/停用状�?
        data.setStatus(*status);
        // 填充修改时间
        data.setUpdateTime(SimpleDateTimeFormat::format());
        // data.setUpdateBy(操作用户�?;

        int affectRows = dao.update(data);
        if (affectRows == 1)
        {
            successCount++;
        }
    }

    // 至少成功一条返回true
    return successCount > 0;
>>>>>>> parent of d02f5cf44 (axiao完成诊疗列表查询功能)
}
=======
}
>>>>>>> 7a964f5c8d17ed83b384e758fad9c7e6fabfd56b
