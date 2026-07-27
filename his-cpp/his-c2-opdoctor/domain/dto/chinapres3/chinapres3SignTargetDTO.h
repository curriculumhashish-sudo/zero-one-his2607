#pragma once
#include "../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * ǩ��Ŀ��DTO
 * ���嵥��ǩ��Ŀ��ı�ʶ��Ϣ������ָ��Ҫ������������¼����ǩ������
 */
class chinapres3SignTargetDTO : public oatpp::DTO
{
    DTO_INIT(chinapres3SignTargetDTO, DTO);

    // ����ҩƷ����ID����Ӧ med_medication_request.id
    API_DTO_FIELD_REQUIRE(String, medicationRequestId, ZH_WORDS_GETTER("chinapres3.field.medicationRequestId"), true);

    // ��������������ڱ�ʶͬһ�ο��ߵĴ�������
    //
   // API_DTO_FIELD_REQUIRE(String, prescriptionGroupKey, ZH_WORDS_GETTER("chinapres3.field.prescriptionGroupKey"), true);
};

#include OATPP_CODEGEN_END(DTO)
