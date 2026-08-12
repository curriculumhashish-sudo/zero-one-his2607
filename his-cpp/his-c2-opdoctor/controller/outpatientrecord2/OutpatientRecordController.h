#ifndef __OUTPATIENTRECORDCONTROLLER_H_
#define __OUTPATIENTRECORDCONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/outpatientrecord2/OutpatientRecordQuery.h"
#include "domain/vo/outpatientrecord2/OutpatientRecordVO.h"
#include "domain/dto/outpatientrecord2/OutpatientRecordDetailDTO.h"  // 新增
#include "domain/vo/outpatientrecord2/OutpatientRecordDetailJsonVO.h"  
#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("outpatientRecord.tags.tag")

class OutpatientRecordController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(OutpatientRecordController);
public:
    // 1. 列表查询（原接口，改名为查询列表）
    // 列表查询（保留 QUERY_AUTH 宏，但传递 payload）
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("outpatientRecord.queryList.summary"), queryRecordList,
        OutpatientRecordQuery, OutpatientRecordPageJsonVO::Wrapper, API_TAG);
    API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/outpatientrecord2", queryRecordList,
        OutpatientRecordQuery, execQueryRecordList(query, authObject->getPayload()));

    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("outpatientRecord.detail.summary"), getRecordDetail,
        OutpatientRecordDetailJsonVO::Wrapper, API_TAG);
    API_HANDLER_ENDPOINT_AUTH(API_M_GET, "/outpatientrecord2/detail/{encounterId}", getRecordDetail,
        PATH(String, encounterId), execGetRecordDetail(encounterId, authObject->getPayload()));

private:
    OutpatientRecordPageJsonVO::Wrapper execQueryRecordList(const OutpatientRecordQuery::Wrapper& query, const PayloadDTO& payload);
    OutpatientRecordDetailJsonVO::Wrapper execGetRecordDetail(const String& encounterId, const PayloadDTO& payload);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif