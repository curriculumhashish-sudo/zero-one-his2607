#include "stdafx.h"
#include "PatientLeaveService.h"


PatientLeaveJsonVO::Wrapper PatientLeaveService::execLeavePatient(const oatpp::String& tenantId, const oatpp::String& encounterId)
{
    auto vo = PatientLeaveJsonVO::createShared();

    //1. 鏌ヨ灏辫瘖璁板綍
    auto encounterDO = dao.selectEncounterById(tenantId, encounterId);
    if (!encounterDO)
    {
        vo->code = 10001;
        vo->message = u8"该就诊记录不存在";
        vo->data = nullptr;
        return vo;
    }

    //2. 鐘舵�佹牎楠?
    oatpp::String status = encounterDO->getStatusEnum();
    if (status == "onHold" || status == "finished" || status == "arrived")
    {
        vo->code = 10002;
        vo->message = u8"该患者已离院或已完诊或已待诊，不能重复操作";
        vo->data = nullptr;
        return vo;
    }

   
    //3. 鎵ц鏇存柊鍒扮闄㈢姸鎬?--> 鍓嶇瑕佹眰鍦ㄨ瘖鍙樻垚寰呰瘖(inProgress -> arrived)
    uint64_t affectRows = dao.updateEncounterToLeave(tenantId, encounterId);
    if (affectRows == 0)
    {
        vo->code = 10004;
        vo->message = u8"修改就诊记录状态失败";
        vo->data = nullptr;
        return vo;
    }

    //4. 杩斿洖鎴愬姛
    auto dto = PatientLeaveDTO::createShared();
    dto->encounterId = encounterId;
    dto->visitStatus = "arrived";

    vo->code = 10000;
    vo->message = "success";
    vo->data = dto;
    return vo;
}