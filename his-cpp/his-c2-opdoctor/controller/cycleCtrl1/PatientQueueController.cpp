#include "stdafx.h"
#include "PatientQueueController.h"
#include "service/cycleCtrl1/PatientQueueService.h"
#include "RedisClient.h"
#include "../../../.skeleton/Macros.h"


PatientQueueJsonVO::Wrapper PatientQueueController::execQueryQueue(const PatientQueueQuery::Wrapper& query, const PayloadDTO& payload)
{
    auto jvo = PatientQueueJsonVO::createShared();

    // ÂøÖÈ°ªÁö?query Âí?date
    if (!query || !query->date) {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }

    // ‰ª?payload Ëé∑Âèñ userId ÔºåÂêéÁª≠ÂèØËÉΩÈúÄË¶?
    /*std::string userId = payload.getId();
    if (userId.empty()) {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }*/

    // ‰ª?payload Ëé∑Âèñ practitionerId
    std::string practitionerId = payload.getPractitionerId();

    // ¥¥Ω® Redis øÕªß∂À“‘ªÒ»° tenantId
    RedisClient re("8.163.102.33", 6380, "Lin@@2830564901zero-one-his");
    std::string tenantId = re.execute<std::string>([](Redis* r) {
        auto res = r->hget("his:login:context:u001", "tenantId");
        return res.value();
        });

    // Â¶ÇÊûú practitionerId Êà?tenantId ‰∏∫Á©∫ÔºåÂàô‰ΩøÁî®ÈªòËÆ§ÂÄ?
    if (practitionerId.empty() || tenantId.empty())
    {
        practitionerId = "";
        tenantId = "tenant_001";
    }

    // Ë∞ÉÁî® service Ëé∑ÂèñÊï∞ÊçÆ
    PatientQueueService service;
    auto dto = service.getPatientQueueList(query, tenantId, practitionerId);

    jvo->success(dto);
    return jvo;
}