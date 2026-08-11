#include "stdafx.h"
#include "OutpatientRecordService.h"
#include "RedisClient.h"
oatpp::List<OutpatientRecordDTO::Wrapper> OutpatientRecordService::convertToDTOList(const std::list<PtrOutpatientRecordViewDO>& doList)
{
    auto dtoList = oatpp::List<OutpatientRecordDTO::Wrapper>::createShared();
    for (auto& doItem : doList) {
        auto dto = OutpatientRecordDTO::createShared();
        dto->encounterId = doItem->getId();        // 就诊ID
        dto->visitDate = doItem->getStartTime();   // 就诊日期
        dto->diagnosis = doItem->getDiagnosis();   // 诊断
        dto->doctorName = doItem->getDoctorName(); // 医生
        dto->deptName = doItem->getDeptName();     // 科室
        dtoList->push_back(dto);
    }
    return dtoList;
}

OutpatientRecordPageDTO::Wrapper OutpatientRecordService::listAll(const OutpatientRecordQuery::Wrapper& query, const PayloadDTO& payload) {
    auto doList = dao.selectWithPage(query, payload);
    auto total = dao.count(query, payload);
    auto dtoList = convertToDTOList(doList);

    auto pageDto = OutpatientRecordPageDTO::createShared();
    pageDto->rows = dtoList;
    pageDto->total = total;
    pageDto->pageIndex = query->pageIndex ? query->pageIndex.getValue(1) : 1;
    pageDto->pageSize = query->pageSize ? query->pageSize.getValue(10) : 10;
    pageDto->pages = (total + pageDto->pageSize - 1) / pageDto->pageSize;
    return pageDto;
}


OutpatientRecordDetailDTO::Wrapper OutpatientRecordService::getDetail(const std::string& encounterId, const PayloadDTO& payload)
{
    if (encounterId.empty()) {
        throw std::runtime_error("encounterId is required");
    }


    // 获取 practitionerId（优先 JWT，若空则从 Redis 获取）
    std::string practitionerId = payload.getPractitionerId();
    if (practitionerId.empty()) {
        std::string username = payload.getUsername();
        try {
            RedisClient redisClient("192.168.112.130", 6379, "@1star");
            std::string key = "user:" + username + ":practitionerId";
            practitionerId = redisClient.execute<std::string>([&key](Redis* r) {
                auto val = r->get(key).value();
                return val;
                });
        }
        catch (const std::exception& e) {
            std::cerr << "Redis get practitionerId error: " << e.what() << std::endl;
        }
        if (practitionerId.empty()) {
            throw std::runtime_error("User not authenticated");
        }
    }

    // ========== 权限校验（恢复） ==========
    if (!dao.checkAccessPermission(encounterId, practitionerId)) {
        throw std::runtime_error("Access denied: You do not have permission to view this medical record");
    }

    // ========== 2. 获取就诊摘要信息 ==========
    std::string patientName, diagnosis, doctorName, deptName, visitDate, status;
    if (!dao.getEncounterSummary(encounterId, patientName, diagnosis, doctorName, deptName, visitDate, status)) {
        throw std::runtime_error("Encounter not found");
    }

    // ========== 3. 获取病历内容 ==========
    std::string tenantId = payload.getId();
    if (tenantId.empty()) {
        throw std::runtime_error("Tenant ID is missing");
    }
    auto detailDO = dao.selectDetailByEncounterId(encounterId, tenantId);
    if (!detailDO) {
        throw std::runtime_error("Medical record content not found");
    }

    // ========== 4. 判断是否可编辑 ==========
    bool editable = (status != "finished" && status != "cancelled" && status != "archived");

    // ========== 5. 组装响应 ==========
    auto dto = OutpatientRecordDetailDTO::createShared();
    dto->id = detailDO->getId();
    dto->encounterId = encounterId;
    dto->content = detailDO->getContent();
    dto->updatedAt = detailDO->getUpdatedAt();
    return dto;
}