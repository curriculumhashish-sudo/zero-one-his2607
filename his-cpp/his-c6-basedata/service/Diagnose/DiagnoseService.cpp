//<<<<<<< HEAD
// #include "DiagnoseService.h"
//#include "../../dao/Diagnose/DiagnoseDAO.h"
//#include "../../lib-common/include/id/UuidFacade.h"
//=======
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
#include "id/UuidFacade.h"
>>>>>>> 7a964f5c8d17ed83b384e758fad9c7e6fabfd56b
#include "SimpleDateTimeFormat.h"

#include <charconv>
static const std::string SOURCE_GENERAL_DIAGNOSIS = "general_dx";
static const std::string SOURCE_CHRONIC_SPECIAL = "chronic_spec";
static const std::string SOURCE_DISEASE_PACKAGE = "disease_pkg";
static const std::string SOURCE_PAID_DIAGNOSIS = "paid_dx";
static const std::string SOURCE_DAY_SURGERY = "day_surgery_dx";
static const std::string SOURCE_TCM_DIAGNOSIS = "tcm_dx";
static const std::string SOURCE_TCM_SYNDROME = "tcm_syndrome";
static const std::string SOURCE_TUMOR_MORPHOLOGY = "tumor_morph";
static const std::string DIAGNOSIS_KIND_TCM = "TCM";
static const std::string DIAGNOSIS_KIND_WESTERN = "WESTERN";
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
	return sourceEnum == SOURCE_TCM_DIAGNOSIS || sourceEnum == SOURCE_TCM_SYNDROME;
}

static bool isWesternMedicineSource(const std::string& sourceEnum)
{
	return sourceEnum == SOURCE_GENERAL_DIAGNOSIS
		|| sourceEnum == SOURCE_CHRONIC_SPECIAL
		|| sourceEnum == SOURCE_DISEASE_PACKAGE
		|| sourceEnum == SOURCE_PAID_DIAGNOSIS
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
DiagnosisSelectPageDTO::Wrapper DiagnoseService::listSelectableDiagnoses(
	const DiagnosisSelectQuery::Wrapper& query,
	const PayloadDTO& payload)
{
	if (!query || !query->validate().empty() || payload.getId().empty())
	{
		return nullptr;
	}

	DiagnosisDAO dao;
	const std::string tenantId = dao.selectCurrentTenantId(payload.getId());
	if (tenantId.empty())
	{
		return nullptr;
	}

	auto page = DiagnosisSelectPageDTO::createShared();
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
		auto dto = DiagnosisSelectDTO::createShared();
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
			dto->diagnosisKind = DIAGNOSIS_KIND_TCM;
			dto->diagnosisKindText = ZH_WORDS_GETTER("diagnose.type.tcm");
		}
		else if (isWesternMedicineSource(sourceEnum))
		{
			dto->diagnosisKind = DIAGNOSIS_KIND_WESTERN;
			dto->diagnosisKindText = ZH_WORDS_GETTER("diagnose.type.western");
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
    // 1. 鍏ュ弬DTO 杞?鏁版嵁搴揇O
    DiagnoseDO data;
    ZO_STAR_DOMAIN_DTO_TO_DO(data, dto,
        DiagnoseName, name,//璇婃柇鍚嶇О
        //CategoryId, categoryId,//璇婃柇鐩綍id澶栭敭
        DiagnoseType, diagType,//璇婃柇绫诲瀷
        DiagnoseCode, chronicCode,//璇婃柇缂栫爜
        MedicalInsuranceCode, medicalInsuranceCode,//鍖讳繚缂栫爜
        Status, status,//鐘舵€?
        IsDelete, isDelete//鍒犻櫎鏍囪
    );

    ZO_STAR_DOMAIN_DTO_TO_DO(data, dto,
        MedicalInsuranceTag, medicalInsuranceTag,//鍖讳繚鏍囩
        MedicalInsuranceMatch, medicalInsuranceMatch,//鍖讳繚瀵圭爜锛圱鎴朏锛?
        PinyinCode, pinyinCode,//鎷奸煶鍔╄
        WubiCode, wubiCode,//浜旂瑪鍔╄
        Remark, remark,//璇存槑
        CatName,catName//鍒嗙被椤圭洰鍚?
        );

    // 2. 鐢熸垚鍏ㄥ眬鍞竴涓婚敭ID
    UuidFacade uf;
    data.setId(uf.genUuid());
>>>>>>> parent of d02f5cf44 (axiao瀹屾垚璇婄枟鍒楄〃鏌ヨ鍔熻兘)

    // 3. 浠嶥TO鍐呭祵鐨勭櫥褰曡浇鑽峰彇鍒涘缓浜猴紙鍖归厤CC_SYNTHESIZE鐢熸垚鐨刧etPayload锛?
    data.setCreateBy(dto->getPayload()->getUsername());
    // 4. 鑷姩濉厖鍒涘缓鏃堕棿
    data.setCreateTime(SimpleDateTimeFormat::format());
    // 5. 閫昏緫鍒犻櫎鏍囪榛樿姝ｅ父
    data.setIsDelete(0);

    // 6. 鎵ц鍏ュ簱锛屾垚鍔熻繑鍥炰富閿甀D锛屽け璐ヨ繑鍥炵┖涓?
    DiagnoseDAO dao;
    return dao.insert(data) == 1 ? data.getId() : "";
}
// 鏍规嵁ID淇敼璇婃柇璁板綍
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

<<<<<<< HEAD:his-cpp/his-c6-basedata/service/Diagnosis/DiagnoseService.cpp
//鍒犻櫎涓€鏉℃暟鎹紙鍓嶇鏈夛級
=======
<<<<<<< HEAD
//删除一条数据（前端有）
=======
    DiagnoseDAO dao;

    //// 1. 鍏堟寜涓婚敭鏌ヨ鍘熸湁鏁版嵁锛岄伩鍏嶅叏閲忔洿鏂拌鐩栨湭浼犲瓧娈?
    //DiagnoseDO data = dao.selectById(dto->getId()->getValue({}));
    //if (data.getId().empty()) {
    //    return false; // 鏁版嵁涓嶅瓨鍦ㄧ洿鎺ヨ繑鍥炲け璐?
    //}
    DiagnoseDO data;
    // 2. 鍏ュ弬DTO 瑕嗙洊鏇存柊鍒版暟鎹簱DO
    ZO_STAR_DOMAIN_DTO_TO_DO(data, dto,
        DiagnoseName, name,       //璇婃柇鍚嶇О
        //CategoryId, categoryId,//璇婃柇鐩綍id澶栭敭
        DiagnoseType, diagType,   //璇婃柇绫诲瀷
        DiagnoseCode, chronicCode,//璇婃柇缂栫爜
        MedicalInsuranceCode, medicalInsuranceCode, //鍖讳繚缂栫爜
        Status, status             //鐘舵€?
    );

    ZO_STAR_DOMAIN_DTO_TO_DO(data, dto,
        MedicalInsuranceTag, medicalInsuranceTag,    //鍖讳繚鏍囩
        MedicalInsuranceMatch, medicalInsuranceMatch,//鍖讳繚瀵圭爜
        PinyinCode, pinyinCode,    //鎷奸煶鍔╄
        WubiCode, wubiCode,        //浜旂瑪鍔╄
        Remark, remark             //璇存槑
    );

    // 3. 浠庣櫥褰曡浇鑽峰彇鎿嶄綔浜猴紝濉厖淇敼浜?
    data.setUpdateBy(dto->getPayload()->getUsername());

    // 4. 鑷姩濉厖淇敼鏃堕棿
    data.setUpdateTime(SimpleDateTimeFormat::format());

    // 5. 鎵ц鏇存柊锛屽奖鍝嶈鏁颁负1鍒欐垚鍔?
    return dao.update(data) == 1;
}
// 閫昏緫鍗曟潯鍒犻櫎
>>>>>>> parent of d02f5cf44 (axiao瀹屾垚璇婄枟鍒楄〃鏌ヨ鍔熻兘)
=======
// Logically deletes one diagnosis record.
>>>>>>> 7a964f5c8d17ed83b384e758fad9c7e6fabfd56b
>>>>>>> e116cb648dc93aca8541a4335647e317e439b35c:his-cpp/his-c6-basedata/service/Diagnose/DiagnoseService.cpp
bool DiagnoseService::removeOne(const std::string& id)
{
    DiagnoseDAO dao;

    // 1. 鏌ヨ鍘熸湁鏁版嵁锛屼繚璇佹暟鎹瓨鍦ㄤ笖閬垮厤鍏ㄩ噺鏇存柊涓㈠瓧娈?
  /*  DiagnoseDO data = dao.selectById(id);
    if (data.getId().empty()) {
        return false;
    }*/

    DiagnoseDO data;
    // 2. 鏍囪閫昏緫鍒犻櫎
    data.setIsDelete(1);

    // 3. 濉厖淇敼鏃堕棿
    data.setUpdateTime(SimpleDateTimeFormat::format());

    // 娉細鑻ラ渶璁板綍鍒犻櫎鎿嶄綔浜猴紝闇€琛ュ厖鎿嶄綔浜哄弬鏁帮紝璋冪敤 data.setUpdateBy(鎿嶄綔浜虹敤鎴峰悕);

    // 4. 鎵ц鏇存柊瀹屾垚閫昏緫鍒犻櫎
    return dao.update(data) == 1;
}
<<<<<<< HEAD:his-cpp/his-c6-basedata/service/Diagnosis/DiagnoseService.cpp
//鐗╃悊鎵归噺鍒犻櫎
=======
<<<<<<< HEAD
<<<<<<< HEAD
//物理批量删除
=======
// 閫昏緫鎵归噺鍒犻櫎锛堝仠鐢級璇婃柇璁板綍
>>>>>>> parent of d02f5cf44 (axiao瀹屾垚璇婄枟鍒楄〃鏌ヨ鍔熻兘)
=======
// Physically deletes a diagnosis batch.
>>>>>>> 7a964f5c8d17ed83b384e758fad9c7e6fabfd56b
>>>>>>> e116cb648dc93aca8541a4335647e317e439b35c:his-cpp/his-c6-basedata/service/Diagnose/DiagnoseService.cpp
bool DiagnoseService::removeData(const oatpp::List<oatpp::String>& ids)
{
    DiagnoseDAO dao;
    //int successCount = 0;

    // 閬嶅巻ID鍒楄〃閫愪釜鎵ц閫昏緫鍒犻櫎
    //std::list<std::string> datas;
    //for (auto& idItem : *ids) {
    //    std::string id = idItem->getValue({});
    //    DiagnoseDO data = dao.selectById(id);

    //    if (data.getId().empty()) {
    //        continue; // 涓嶅瓨鍦ㄧ殑ID璺宠繃
    //    }

    //    data.setIsDelete(1);
    //    data.setUpdateTime(SimpleDateTimeFormat::format());
    //    // 鍚屼笂锛岄渶璁板綍鎿嶄綔浜哄彲鍦ㄦ缁熶竴璁剧疆

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
<<<<<<< HEAD:his-cpp/his-c6-basedata/service/Diagnosis/DiagnoseService.cpp
	const oatpp::List<DiagnoseBatchDTO::Wrapper> ids = dto->ids;
	DiagnosisDAO dao;
	int successCount = 0;
	for (const auto& idItem : *ids.get())
	{
		DiagnosisDO data;
		data.setId(idItem->id);
=======
	const oatpp::List<oatpp::String> ids = dto->ids;
	DiagnoseDAO dao;
	int successCount = 0;
	for (const auto& idItem : *ids.get())
	{
		DiagnoseDO data;
		data.setId(idItem.getValue({}));
>>>>>>> e116cb648dc93aca8541a4335647e317e439b35c:his-cpp/his-c6-basedata/service/Diagnose/DiagnoseService.cpp
		data.setStatus(*status);
		data.setUpdateBy(dto->getPayload()->getUsername());
		data.setUpdateTime(SimpleDateTimeFormat::format());
		data.setVersion((idItem->version)+ 1);
		if (dao.update(data) == 1)
		{
			++successCount;
		}
	}
	return successCount > 0;
}
//閫昏緫鎵归噺鍒犻櫎
bool DiagnoseService::execRemoveData(const DiagnoseRemoveDTO::Wrapper& dto)
{
	const oatpp::List<oatpp::String> ids = dto->ids;
	DiagnoseDAO dao;
	int successCount = 0;
	for (const auto& idItem : *ids.get())//杩斿洖鍘熺敓鐨剉ector骞舵灇涓鹃亶鍘嗭紝get鑾峰彇瑁告寚閽?
	{
		DiagnoseDO data;
		data.setId(idItem.getValue({}));
		data.setDeletedBy(dto->getPayload()->getUsername());
		data.setDeletedTime(SimpleDateTimeFormat::format());
		data.setIsDelete(1);
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

    // 鐩存帴鏇存柊鐘舵€侊紱涓氬姟绾﹀畾浼犲叆ID涓€瀹氬瓨鍦紝鏃犻渶鏌ヨ鍒ょ┖
    for (auto& idItem : *ids.get())
    {
        std::string id = idItem.getValue({});
        DiagnoseDO data;
        // 涓婚敭鐢ㄤ簬瀹氫綅鏇存柊琛?
        data.setId(id);
        // 璁剧疆鐩爣鍚敤/鍋滅敤鐘舵€?
        data.setStatus(*status);
        // 濉厖淇敼鏃堕棿
        data.setUpdateTime(SimpleDateTimeFormat::format());
        // data.setUpdateBy(鎿嶄綔鐢ㄦ埛鍚?;

        int affectRows = dao.update(data);
        if (affectRows == 1)
        {
            successCount++;
        }
    }

    // 鑷冲皯鎴愬姛涓€鏉¤繑鍥瀟rue
    return successCount > 0;
>>>>>>> parent of d02f5cf44 (axiao瀹屾垚璇婄枟鍒楄〃鏌ヨ鍔熻兘)
}
=======
}
>>>>>>> 7a964f5c8d17ed83b384e758fad9c7e6fabfd56b
