#pragma once

#ifndef _REFUNDCONTROLLER_H_
#define _REFUNDCONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include"../../domain/query/cycleCtrl2-refund/RefundQuery.h"
#include"../../domain/vo/doctor/Refund2VO.h"
#include"../../domain/dto/doctor/Refund2ApplyDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("cyclectrl2.test.tag")

class Refund2Controller : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(Refund2Controller);
public:
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("cyclectrl2.refund.query"), queryRefund, Refund2Query, Refund2DetailJsonVO::Wrapper, API_TAG);
    API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/cyclectrl/refund_query", queryRefund, Refund2Query, executeQueryRefund(query));
    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("cyclectrl2.refund.apply"), applyRefund, Refund2ApplyResultJsonVO::Wrapper, API_TAG);
    API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/cyclectrl/refund_apply", applyRefund, BODY_DTO(Refund2ApplyDTO::Wrapper, dto), execApplyRefund(dto, authObject->getPayload()));
private:
    Refund2DetailJsonVO::Wrapper executeQueryRefund(const Refund2Query::Wrapper& query);
    Refund2ApplyResultJsonVO::Wrapper execApplyRefund(const Refund2ApplyDTO::Wrapper& dto, const PayloadDTO& payload);
};
#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)
#endif
