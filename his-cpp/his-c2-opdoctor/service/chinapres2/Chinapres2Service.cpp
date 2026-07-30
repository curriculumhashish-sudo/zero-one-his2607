/*
 Copyright Zero One Star. All rights reserved.
 @Author: awei
 @Date: 2026/07/25
*/
#include "stdafx.h"
#include "Chinapres2Service.h"

// ========== 私有方法：DO → DTO 转换 ==========
oatpp::List<Chinapres2DetailDTO::Wrapper> Chinapres2Service::convertToDTOList(const std::list<PtrChinapres2ViewDO>& doList)
{
    auto dtoList = oatpp::List<Chinapres2DetailDTO::Wrapper>::createShared();

    for (auto& doItem : doList) {
        auto dto = Chinapres2DetailDTO::createShared();

        // 主表字段映射
        dto->id = doItem->getId();
        dto->usage = doItem->getMethodCode();
        dto->frequency = doItem->getRateCode();
        dto->days = doItem->getExecuteNum();
        dto->copies = (int)doItem->getChineseHerbsDoseQuantity();
        dto->decoction = doItem->getSufferingFlag();
        dto->diagnosis = doItem->getDosageInstruction();
        dto->remark = doItem->getRemark();

        // 视图扩展字段（如果 DTO 有这些字段，可以赋值）
        // dto->diagnosisName = doItem->getDiagnosisName();
        // dto->requesterName = doItem->getRequesterName();
        // dto->medicationNames = doItem->getMedicationNames();
        // dto->totalPrice = doItem->getTotalPrice();

        dtoList->push_back(dto);
    }

    return dtoList;
}

// ========== 查询历史处方 ==========
Chinapres2DetailPageDTO::Wrapper Chinapres2Service::listAll(const Chinapres2Query::Wrapper& query)
{
    // 1. 调用 DAO 查询数据
    auto doList = dao.selectWithPage(query);
    auto total = dao.count(query);

    // 2. DO → DTO 转换
    auto dtoList = convertToDTOList(doList);

    // 3. 组装分页结果
    auto pageDto = Chinapres2DetailPageDTO::createShared();
    pageDto->rows = dtoList;
    pageDto->total = total;
    pageDto->pageIndex = query->pageIndex ? query->pageIndex.getValue(1) : 1;
    pageDto->pageSize = query->pageSize ? query->pageSize.getValue(10) : 10;
    pageDto->pages = (total + pageDto->pageSize - 1) / pageDto->pageSize;

    return pageDto;
}

// ========== 新增处方 ==========
std::string Chinapres2Service::saveData(const Chinapres2AddDTO::Wrapper& dto, const PayloadDTO& payload)
{
    // 1. 生成 ID 和业务号
    auto now = std::chrono::system_clock::now();
    auto ts = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    std::string mainId = "PRES_" + std::to_string(ts);
    std::string busNo = "BUS_" + std::to_string(ts);

    // 2. 获取当前时间（使用项目工具类，如果不行就用硬编码）
    std::string timeStr = "2026-07-25 12:00:00";  // 建议改为 SimpleDateTimeFormat::format()

    // 3. 构造主表 DO
    Chinapres2MainDO mainDO;
    mainDO.setId(mainId);
    mainDO.setBusNo(busNo);
    mainDO.setStatusEnum("DRAFT");
    mainDO.setIsDeleted(0);

    // 从 PayloadDTO 获取用户信息（根据实际 PayloadDTO 调整）
    // 如果 PayloadDTO 没有对应方法，先用硬编码测试
    mainDO.setPatientId("325580322788872192");   // 从 payload 获取
    mainDO.setEncounterId("329874660147597312"); // 从 payload 获取
    mainDO.setRequesterId("308791351298756608"); // 从 payload 获取
    mainDO.setOrgId("322663105141149696");       // 从 payload 获取

    // 从 DTO 赋值
    mainDO.setMethodCode(dto->usage ? dto->usage->c_str() : "");
    mainDO.setRateCode(dto->frequency ? dto->frequency->c_str() : "");
    mainDO.setExecuteNum(dto->days ? *dto->days : 0);
    mainDO.setChineseHerbsDoseQuantity(dto->copies ? (double)*dto->copies : 0.0);
    mainDO.setSufferingFlag(dto->decoction ? *dto->decoction : 0);
    mainDO.setDosageInstruction(dto->diagnosis ? dto->diagnosis->c_str() : "");
    mainDO.setRemark(dto->remark ? dto->remark->c_str() : "");
    mainDO.setCreatedAt(timeStr);

    // 为可空字段设置默认值
    mainDO.setPrescriptionNo("");
    mainDO.setGroupId("");
    mainDO.setStatusReason("0");
    mainDO.setEncounterDiagnosisId("");
    mainDO.setConditionDefinitionId("");

    // 4. 构造明细列表
    std::list<PtrChinapres2DetailDO> detailList;
    int sort = 0;
    for (auto& item : *(dto->details)) {
        auto detail = std::make_shared<Chinapres2DetailDO>();
        detail->setId("DETAIL_" + std::to_string(ts) + "_" + std::to_string(sort));
        detail->setRequestId(mainId);
        detail->setMedicationId(item->medicineId ? item->medicineId->c_str() : "");
        detail->setQuantity(item->quantity ? *item->quantity : 0.0);
        detail->setUnitCode(item->unit ? item->unit->c_str() : "");
        detail->setDose(item->dose ? *item->dose : 0.0);
        detail->setDoseUnitCode(item->doseUnit ? item->doseUnit->c_str() : "");
        detail->setSortNo(sort++);
        detail->setCreateTime(timeStr);
        detailList.push_back(detail);
    }

    // 5. 调用 DAO 插入数据
    if (!dao.insertMain(mainDO)) {
        return "";  // 主表插入失败
    }

    if (!dao.insertDetailsPtr(detailList)) {
        return "";  // 明细表插入失败
    }

    return mainId;
}

// ========== 删除（软删除） ==========
bool Chinapres2Service::removeData(const std::string& id)
{
    if (id.empty()) {
        return false;
    }

    return dao.softDeleteById(id);
}