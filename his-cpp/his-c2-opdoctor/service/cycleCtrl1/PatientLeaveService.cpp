#include "stdafx.h"
#include "PatientLeaveService.h"


PatientLeaveJsonVO::Wrapper PatientLeaveService::execLeavePatient(const oatpp::String& tenantId, const oatpp::String& encounterId)
{
    auto vo = PatientLeaveJsonVO::createShared();

    //1. æŸ¥è¯¢å°±è¯Šè®°å½•
    auto encounterDO = dao.selectEncounterById(tenantId, encounterId);
    if (!encounterDO)
    {
        vo->code = 10001;
        vo->message = u8"å°±è¯Šè®°å½•ä¸å­˜åœ?;
        vo->data = nullptr;
        return vo;
    }

    //2. çŠ¶æ€æ ¡éª?
    oatpp::String status = encounterDO->getStatusEnum();
    if (status == "onHold" || status == "finished" || status == "arrived")
    {
        vo->code = 10002;
        vo->message = u8"¸Ã»¼ÕßÒÑÀëÔº»òÒÑÍêÕï»òÒÑ´ıÕï£¬²»ÄÜÖØ¸´²Ù×÷";
        vo->data = nullptr;
        return vo;
    }

   
    //3. æ‰§è¡Œæ›´æ–°åˆ°ç¦»é™¢çŠ¶æ€?--> å‰ç«¯è¦æ±‚åœ¨è¯Šå˜æˆå¾…è¯Š(inProgress -> arrived)
    uint64_t affectRows = dao.updateEncounterToLeave(tenantId, encounterId);
    if (affectRows == 0)
    {
        vo->code = 10004;
        vo->message = u8"ç¦»é™¢æ“ä½œå¤±è´¥";
        vo->data = nullptr;
        return vo;
    }

    //4. è¿”å›æˆåŠŸ
    auto dto = PatientLeaveDTO::createShared();
    dto->encounterId = encounterId;
    dto->visitStatus = "arrived";

    vo->code = 10000;
    vo->message = "success";
    vo->data = dto;
    return vo;
}