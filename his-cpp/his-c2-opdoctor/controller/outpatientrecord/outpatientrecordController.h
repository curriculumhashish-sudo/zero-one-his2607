#ifndef __OUTPATIENTRECORDCONTROLLER_H_
#define __OUTPATIENTRECORDCONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/outpatientrecord/OutpatientRecordQuery.h"
#include "domain/vo/outpatientrecord/OutpatientRecordVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("outpatientRecord.tags.tag")

class OutpatientRecordController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(OutpatientRecordController);
public:
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("outpatientRecord.query.summary"), queryRecord, OutpatientRecordQuery, OutpatientRecordPageJsonVO::Wrapper, API_TAG);
    API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/outpatientrecord", queryRecord, OutpatientRecordQuery, execQueryRecord(query));

private:
    OutpatientRecordPageJsonVO::Wrapper execQueryRecord(const OutpatientRecordQuery::Wrapper& query);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !__OUTPATIENTRECORDCONTROLLER_H_