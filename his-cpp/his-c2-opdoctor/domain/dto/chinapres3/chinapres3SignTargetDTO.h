#pragma once
#include "../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * ����Ŀ��DTO
 * �洢������ʶ�����Ϣ������ͬ��ǩ�𴦷�����
 */
class chinapres3SignTargetDTO : public oatpp::DTO
{
    DTO_INIT(chinapres3SignTargetDTO, DTO);

    // ҩƷ���뵥ID ��Ӧ med_medication_request.id
    API_DTO_FIELD_REQUIRE(String, medicationRequestId, ZH_WORDS_GETTER("chinapres3.field.medicationRequestId"), true);

    // ���������ʶ����������ͬһ���ζ��Ŵ���
    // API_DTO_FIELD_REQUIRE(String, prescriptionGroupKey, ZH_WORDS_GETTER("chinapres3.field.prescriptionGroupKey"), true);
};

#include OATPP_CODEGEN_END(DTO)