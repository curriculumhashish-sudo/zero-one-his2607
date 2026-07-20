#ifndef __CHINAPREADDDTO_H_
#define __CHINAPREADDDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

//中医处方添加DTO类
class ChinapreAddDTO : public oatpp::DTO
{
    DTO_INIT(ChinapreAddDTO, DTO);
    // 用法 (必填)
    API_DTO_FIELD_REQUIRE(String, usage, ZH_WORDS_GETTER("prescription.field.usage"), true);
    // 频次 (必填)
    API_DTO_FIELD_REQUIRE(String, frequency, ZH_WORDS_GETTER("prescription.field.frequency"), true);
    // 天数 (必填, 用Int32存储数字)
    API_DTO_FIELD_REQUIRE(Int32, days, ZH_WORDS_GETTER("prescription.field.days"), true);
    // 付数 (必填, 用Int32存储数字)
    API_DTO_FIELD_REQUIRE(Int32, copies, ZH_WORDS_GETTER("prescription.field.copies"), true);
    // 代煎 (开关, 非必填)
    API_DTO_FIELD_REQUIRE(Int32, decoction, ZH_WORDS_GETTER("prescription.field.decoction"), false);
    // 诊断 (必填)
    API_DTO_FIELD_REQUIRE(String, diagnosis, ZH_WORDS_GETTER("prescription.field.diagnosis"), true);
    // 备注 (非必填)
    API_DTO_FIELD_REQUIRE(String, remark, ZH_WORDS_GETTER("prescription.field.remark"), false);
    //药材(必填)
    API_DTO_FIELD_REQUIRE(String, medicine, ZH_WORDS_GETTER("prescription.field.medicine"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !__CHINAPREADDDTO_H_