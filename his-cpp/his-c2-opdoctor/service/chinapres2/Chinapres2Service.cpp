/*
 Copyright Zero One Star. All rights reserved.
 @Author: awei
 @Date: 2026/07/25
*/
#include "stdafx.h"
#include "Chinapres2Service.h"
#include "../../dao/chinapres2/MedicationDAO.h"  // 药品查询
#include <chrono>
#include <iomanip>
#include <sstream>
#include "RedisClient.h"

/*
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
*/

// ========== 新增处方 ==========
// 辅助函数：获取当前时间字符串
static std::string getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}



Chinapres2AddRespDTO::Wrapper Chinapres2Service::saveData(
    const Chinapres2AddDTO::Wrapper& dto,
    const PayloadDTO& payload)
{
    auto resp = Chinapres2AddRespDTO::createShared();

    // ========== 1. 基础校验 ==========
    if (!dto) {
        throw std::runtime_error("dto is null");
    }
    if (!dto->details || dto->details->empty()) {
        throw std::runtime_error("details list is null or empty");
    }

    // 就诊可编辑校验
    if (!dao.isEncounterEditable(dto->encounterId->c_str())) {
        throw std::runtime_error("Encounter does not exist or has been deleted");
    }

    // ========== 2. 生成主键 ==========
    auto now = std::chrono::system_clock::now();
    auto ts = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    std::string mainId = "PRES_" + std::to_string(ts);
    std::string busNo = "BUS_" + std::to_string(ts);
    std::string timeStr = getCurrentTime();

    // ========== 3. 获取患者/就诊 ID（来自请求） ==========
    std::string patientId = dto->patientId->c_str();
    std::string encounterId = dto->encounterId->c_str();

    // 校验患者与就诊匹配
    if (!dao.isPatientMatchEncounter(patientId, encounterId)) {
        throw std::runtime_error("Patient does not match this encounter");
    }

    // ========== 4. 获取医生/机构 ID（优先 payload，其次 Redis） ==========
    std::string requesterId = payload.getPractitionerId();
    std::map<std::string, std::string> orgMap = payload.getOrganizationUnits();

    // 从用户名获取 Redis Key
    std::string username = payload.getUsername();

    // 如果 practitionerId 为空，从 Redis 获取
    if (requesterId.empty()) {
        try {
            // 根据实际环境配置 Redis 地址和密码（建议从配置读取）
            RedisClient redisClient("192.168.112.130", 6379, "@1star");
            std::string key = "user:" + username + ":practitionerId";
            requesterId = redisClient.execute<std::string>([&key](Redis* r) {
                auto val = r->get(key).value();
                return val;
                });
        }
        catch (const std::exception& e) {
            std::cerr << "Redis get practitionerId error: " << e.what() << std::endl;
        }
        if (requesterId.empty()) {
            throw std::runtime_error("Practitioner ID not found for user: " + username);
        }
    }

    // 如果 organizationUnits 为空，从 Redis 获取
    if (orgMap.empty()) {
        try {
            RedisClient redisClient("192.168.112.130", 6379, "@1star");
            std::string orgKey = "user:" + username + ":orgUnits";
            std::string orgJson = redisClient.execute<std::string>([&orgKey](Redis* r) {
                auto val = r->get(orgKey).value();
                return val;
                });
            // 解析格式: {"322663105141149696":"测试医院"}
            if (!orgJson.empty()) {
                size_t objStart = orgJson.find('{');
                size_t objEnd = orgJson.find('}');
                if (objStart != std::string::npos && objEnd != std::string::npos) {
                    std::string content = orgJson.substr(objStart + 1, objEnd - objStart - 1);
                    size_t colonPos = content.find(':');
                    if (colonPos != std::string::npos) {
                        std::string key = content.substr(0, colonPos);
                        std::string value = content.substr(colonPos + 1);
                        // 去除引号和空格
                        key.erase(std::remove(key.begin(), key.end(), '\"'), key.end());
                        key.erase(std::remove(key.begin(), key.end(), ' '), key.end());
                        value.erase(std::remove(value.begin(), value.end(), '\"'), value.end());
                        value.erase(std::remove(value.begin(), value.end(), ' '), value.end());
                        orgMap[key] = value;
                    }
                }
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Redis get orgUnits error: " << e.what() << std::endl;
        }
        if (orgMap.empty()) {
            throw std::runtime_error("Organization units not found for user: " + username);
        }
    }
    std::string orgId = orgMap.begin()->first;

    // ========== 5. 构造主表 DO ==========
    Chinapres2MainDO mainDO;
    mainDO.setId(mainId);
    mainDO.setBusNo(busNo);
    mainDO.setPrescriptionNo("PRESNO_" + std::to_string(ts));
    mainDO.setStatusEnum("UNSIGNED");
    mainDO.setIsDeleted(0);
    mainDO.setPatientId(patientId);
    mainDO.setEncounterId(encounterId);
    mainDO.setRequesterId(requesterId);
    mainDO.setOrgId(orgId);
    mainDO.setMethodCode(dto->usage ? dto->usage->c_str() : "");
    mainDO.setRateCode(dto->frequency ? dto->frequency->c_str() : "");
    mainDO.setExecuteNum(dto->days ? *dto->days : 0);
    mainDO.setChineseHerbsDoseQuantity(dto->copies ? (double)*dto->copies : 0.0);
    mainDO.setSufferingFlag(dto->decoction ? *dto->decoction : 0);
    mainDO.setEncounterDiagnosisId(dto->encounterDiagnosisId ? dto->encounterDiagnosisId->c_str() : "");
    mainDO.setConditionDefinitionId("");
    mainDO.setDosageInstruction("");
    mainDO.setRemark(dto->remark ? dto->remark->c_str() : "");
    mainDO.setCreatedAt(timeStr);
    mainDO.setStatusReason("");

    // ========== 6. 处理明细 ==========
    MedicationDAO medDao;
    double totalAmount = 0.0;
    double singleDoseTotal = 0.0;
    std::list<PtrChinapres2DetailDO> detailList;
    int sort = 0;

    for (auto& item : *(dto->details)) {
        if (!item) continue;

        // 药品有效性校验
        if (!item->medicineId || item->medicineId->empty()) {
            throw std::runtime_error("medicineId is required.");
        }
        auto med = medDao.selectById(item->medicineId->c_str());
        if (!med) {
            throw std::runtime_error("药品ID不存在: " + std::string(item->medicineId->c_str()));
        }

        // 单位合法性校验
        if (item->unit && !item->unit->empty() && !dao.isUnitValid(item->unit->c_str())) {
            throw std::runtime_error("单位不合法: " + std::string(item->unit->c_str()));
        }

        auto detail = std::make_shared<Chinapres2DetailDO>();
        detail->setId("DETAIL_" + std::to_string(ts) + "_" + std::to_string(sort));
        detail->setRequestId(mainId);
        detail->setMedicationId(item->medicineId->c_str());
        detail->setQuantity(item->quantity ? *item->quantity : 0.0);
        detail->setUnitCode(item->unit ? item->unit->c_str() : med->getUnitCode().c_str());
        detail->setDose(item->dose ? *item->dose : 0.0);
        detail->setDoseUnitCode(item->doseUnit ? item->doseUnit->c_str() : med->getUnitCode().c_str());
        detail->setSortNo(sort++);
        detail->setCreateTime(timeStr);

        double itemAmount = detail->getQuantity() * med->getPrice();
        totalAmount += itemAmount;
        singleDoseTotal += itemAmount;
        detailList.push_back(detail);
    }

    // ========== 插入数据库（事务） ==========
    dao.beginTransaction();
    try {
        if (!dao.insertMain(mainDO)) {
            throw std::runtime_error("保存主表失败");
        }
        if (!dao.insertDetailsPtr(detailList)) {
            throw std::runtime_error("保存明细失败");
        }
        dao.commit();
    }
    catch (const std::exception& e) {
        dao.rollback();
        std::cerr << "[ERROR] saveData transaction failed: " << e.what() << std::endl;
        throw;
    }
    catch (...) {
        dao.rollback();
        std::cerr << "[ERROR] saveData transaction failed: unknown error" << std::endl;
        throw;
    }


    // ========== 8. 组装响应 ==========
    resp->id = mainId;
    resp->prescriptionNo = mainDO.getPrescriptionNo();
    resp->status = mainDO.getStatusEnum();
    resp->version = (int64_t)0;
    resp->createdAt = timeStr;
    resp->herbCount = (int32_t)detailList.size();
    resp->singleDoseAmount = singleDoseTotal;
    resp->totalAmount = totalAmount * (dto->copies ? *dto->copies : 1);

    return resp;
}
// ========== updateData 函数实现 ==========
Chinapres2AddRespDTO::Wrapper Chinapres2Service::updateData(
    const Chinapres2UpdateDTO::Wrapper& dto,
    const PayloadDTO& payload)
{
    auto resp = Chinapres2AddRespDTO::createShared();

    // ========== 1. 基础校验 ==========
    if (!dto) {
        throw std::runtime_error("dto is null");
    }
    if (!dto->details || dto->details->empty()) {
        throw std::runtime_error("details list is null or empty");
    }
    if (!dto->id || dto->id->empty()) {
        throw std::runtime_error("prescription id is required");
    }
    if (!dto->version) {
        throw std::runtime_error("version is required");
    }

    // ========== 2. 查询并校验处方状态 ==========
    std::string currentStatus;
    int64_t currentVersion;
    std::string currentEncounter;
    if (!dao.getPresenceStatus(dto->id->c_str(), currentStatus, currentVersion, currentEncounter)) {
        throw std::runtime_error("Prescription not found");
    }
    if (currentEncounter != dto->encounterId->c_str()) {
        throw std::runtime_error("Encounter ID mismatch");
    }
    if (currentStatus != "UNSIGNED") {
        throw std::runtime_error("Prescription status is not UNSIGNED, cannot update");
    }
    if (currentVersion != *dto->version) {
        throw std::runtime_error("Version mismatch, please refresh and retry");
    }

    // 校验就诊是否可编辑（未被软删除）
    if (!dao.isEncounterEditable(dto->encounterId->c_str())) {
        throw std::runtime_error("Encounter does not exist or has been deleted");
    }

    // ========== 3. 获取原处方记录（用于合并） ==========
    auto existing = dao.selectById(dto->id->c_str());
    if (!existing) {
        throw std::runtime_error("Presence not found after validation");
    }

    // ========== 4. 准备更新数据 ==========
    std::string timeStr = getCurrentTime();
    // ========== 4. 获取操作人 ID（从 payload 或 Redis） ==========
    std::string operatorId = payload.getPractitionerId();
    if (operatorId.empty()) {
        std::string username = payload.getUsername();
        try {
            RedisClient redisClient("192.168.112.130", 6379, "@1star");
            std::string key = "user:" + username + ":practitionerId";
            operatorId = redisClient.execute<std::string>([&key](Redis* r) {
                auto val = r->get(key).value();
                return val;
                });
        }
        catch (const std::exception& e) {
            std::cerr << "Redis get practitionerId error: " << e.what() << std::endl;
        }
        if (operatorId.empty()) {
            throw std::runtime_error("Missing practitioner ID in token");
        }
    }

    // 只修改允许修改的字段（保留其他字段原值）
    if (dto->usage && !dto->usage->empty()) {
        existing->setMethodCode(dto->usage->c_str());
    }
    if (dto->frequency && !dto->frequency->empty()) {
        existing->setRateCode(dto->frequency->c_str());
    }
    if (dto->days) {
        existing->setExecuteNum(*dto->days);
    }
    if (dto->copies) {
        existing->setChineseHerbsDoseQuantity((double)*dto->copies);
    }
    if (dto->decoction) {
        existing->setSufferingFlag(*dto->decoction);
    }
    if (dto->remark) {
        existing->setRemark(dto->remark->c_str());
    }
    if (dto->encounterDiagnosisId && !dto->encounterDiagnosisId->empty()) {
        existing->setEncounterDiagnosisId(dto->encounterDiagnosisId->c_str());
    }
    // 更新修改时间（DO 中有 updatedAt 字段）
    existing->setUpdatedAt(timeStr);

    // ========== 5. 处理明细 ==========
    MedicationDAO medDao;
    double totalAmount = 0.0;
    double singleDoseTotal = 0.0;
    std::list<PtrChinapres2DetailDO> detailList;
    int sort = 0;

    for (auto& item : *(dto->details)) {
        if (!item) continue;

        // 药品有效性校验
        if (!item->medicineId || item->medicineId->empty()) {
            throw std::runtime_error("medicineId is required.");
        }
        auto med = medDao.selectById(item->medicineId->c_str());
        if (!med) {
            throw std::runtime_error("药品ID不存在: " + std::string(item->medicineId->c_str()));
        }

        // 单位合法性校验
        if (item->unit && !item->unit->empty() && !dao.isUnitValid(item->unit->c_str())) {
            throw std::runtime_error("单位不合法: " + std::string(item->unit->c_str()));
        }

        auto detail = std::make_shared<Chinapres2DetailDO>();
        auto now = std::chrono::system_clock::now();
        auto ts = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        detail->setId("DETAIL_" + std::to_string(ts) + "_" + std::to_string(sort));
        detail->setRequestId(dto->id->c_str());
        detail->setMedicationId(item->medicineId->c_str());
        detail->setQuantity(item->quantity ? *item->quantity : 0.0);
        detail->setUnitCode(item->unit ? item->unit->c_str() : med->getUnitCode().c_str());
        detail->setDose(item->dose ? *item->dose : 0.0);
        detail->setDoseUnitCode(item->doseUnit ? item->doseUnit->c_str() : med->getUnitCode().c_str());
        detail->setSortNo(sort++);
        detail->setCreateTime(timeStr);

        double itemAmount = detail->getQuantity() * med->getPrice();
        totalAmount += itemAmount;
        singleDoseTotal += itemAmount;
        detailList.push_back(detail);
    }

    // ========== 6. 执行数据库更新（事务） ==========
    dao.beginTransaction();
    try {
        // 6.1 删除旧明细
        if (!dao.deleteDetailsByRequestId(dto->id->c_str())) {
            throw std::runtime_error("Failed to delete old details");
        }
        // 6.2 插入新明细
        if (!dao.insertDetailsPtr(detailList)) {
            throw std::runtime_error("Failed to insert new details");
        }
        // 6.3 更新主表（使用现有对象，传入 encounterId 和 oldVersion）
        if (!dao.updateMain(*existing, dto->encounterId->c_str(), *dto->version)) {
            throw std::runtime_error("Failed to update main record");
        }
        dao.commit();
    }
    catch (const std::exception& e) {
        dao.rollback();
        std::cerr << "[ERROR] updateData transaction failed: " << e.what() << std::endl;
        throw;
    }
    catch (...) {
        dao.rollback();
        std::cerr << "[ERROR] updateData transaction failed: unknown error" << std::endl;
        throw;
    }

    // ========== 7. 重新查询更新后的版本 ==========
    std::string newStatus;
    int64_t newVersion;
    std::string newEncounter;
    if (!dao.getPresenceStatus(dto->id->c_str(), newStatus, newVersion, newEncounter)) {
        throw std::runtime_error("Presence not found after update");
    }

    // ========== 8. 组装响应 ==========
    resp->id = dto->id;
    resp->prescriptionNo = existing->getPrescriptionNo();  // 从现有对象获取
    resp->status = newStatus;
    resp->version = newVersion;
    resp->createdAt = existing->getCreatedAt();  // 保留创建时间
    resp->herbCount = (int32_t)detailList.size();
    resp->singleDoseAmount = singleDoseTotal;
    resp->totalAmount = totalAmount * (dto->copies ? *dto->copies : 1);
    resp->updatedAt = timeStr;

    return resp;
}


// ========== 删除（软删除） ==========
bool Chinapres2Service::removeData(const std::string& id)
{
    if (id.empty()) {
        return false;
    }

    return dao.softDeleteById(id);
}

Chinapres2RemoveRespDTO::Wrapper Chinapres2Service::removeDataEnhanced(
    const std::string& id,
    const std::string& encounterId,
    int64_t version,
    const PayloadDTO& payload)
{
    auto resp = Chinapres2RemoveRespDTO::createShared();

    // ========== 1. 参数校验 ==========
    if (id.empty() || encounterId.empty()) {
        throw std::runtime_error("id and encounterId are required");
    }

    // ========== 2. 获取操作人信息 ==========
    std::string operatorId = payload.getPractitionerId();
    if (operatorId.empty()) {
        // 如果 payload 中缺失，可尝试从 Redis 获取（但依赖外部，暂用异常）
        throw std::runtime_error("operator not identified");
    }

    // ========== 3. 执行删除（事务） ==========
    std::string newVersion;
    std::string updatedAt;

    dao.beginTransaction();
    try {
        if (!dao.removeWithCondition(id, encounterId, version, operatorId, newVersion, updatedAt)) {
            // 删除失败，尝试获取具体原因
            std::string status;
            int64_t currentVersion;
            std::string currentEncounter;
            if (dao.getPresenceStatus(id, status, currentVersion, currentEncounter)) {
                if (currentEncounter != encounterId) {
                    throw std::runtime_error("Encounter ID mismatch");
                }
                if (status != "UNSIGNED") {
                    throw std::runtime_error("Prescription status is not UNSIGNED, cannot delete");
                }
                if (currentVersion != version) {
                    throw std::runtime_error("Version mismatch, please refresh and retry");
                }
                // 如果已经软删除，但 removeWithCondition 未识别为幂等，这里可再查一次
                // 但 removeWithCondition 已处理幂等，所以这里不应该发生
            }
            else {
                throw std::runtime_error("Prescription not found");
            }
            // 如果以上都不匹配，抛出通用错误
            throw std::runtime_error("Delete failed for unknown reason");
        }
        dao.commit();
    }
    catch (const std::exception& e) {
        dao.rollback();
        std::cerr << "[ERROR] removeDataEnhanced transaction failed: " << e.what() << std::endl;
        throw; // 重新抛出，让 Controller 层捕获并返回错误响应
    }
    catch (...) {
        dao.rollback();
        std::cerr << "[ERROR] removeDataEnhanced transaction failed: unknown error" << std::endl;
        throw;
    }

    // ========== 4. 组装响应 ==========
    resp->id = id;
    resp->version = std::stoll(newVersion);
    resp->updatedAt = updatedAt;

    return resp;
}