#pragma once

#ifndef _APPLICATION_FORM_DETAIL_DTO_H_
#define _APPLICATION_FORM_DETAIL_DTO_H_

#include "../../GlobalInclude.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(DTO)

// 可打印的服务申请单明细
class ApplicationFormDetailDTO : public oatpp::DTO
{
    DTO_INIT(ApplicationFormDetailDTO, DTO);

    DTO_FIELD(String, serviceRequestId);
    DTO_FIELD(String, prescriptionNo);
    DTO_FIELD(String, encounterId);
    DTO_FIELD(String, patientId);
    DTO_FIELD(String, status);
    DTO_FIELD(String, category);
    DTO_FIELD(Float64, quantity);
    DTO_FIELD(String, unitCode);
    DTO_FIELD(String, requestTime);
    DTO_FIELD(String, requesterId);
    DTO_FIELD(String, performerId);
    DTO_FIELD(String, locationId);
    DTO_FIELD(String, organizationId);
    DTO_FIELD(String, remark);
    DTO_FIELD(Int32, printCount);
    DTO_FIELD(Int32, version);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_APPLICATION_FORM_DETAIL_DTO_H_
