#ifndef __CHINAPRES2ADDDTO_H_
#define __CHINAPRES2ADDDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

//中医处方添加DTO类
class Chinapres2AddDTO : public oatpp::DTO
{
    DTO_INIT(Chinapres2AddDTO, DTO);
    // 用法 (必填)
    API_DTO_FIELD_REQUIRE(String, usage, ZH_WORDS_GETTER("chinapres2.field.usage"), true);
    // 频次 (必填)
    API_DTO_FIELD_REQUIRE(String, frequency, ZH_WORDS_GETTER("chinapres2.field.frequency"), true);
    // 天数 (必填, 用Int32存储数字)
    API_DTO_FIELD_REQUIRE(Int32, days, ZH_WORDS_GETTER("chinapres2.field.days"), true);
    // 付数 (必填, 用Int32存储数字)
    API_DTO_FIELD_REQUIRE(Int32, copies, ZH_WORDS_GETTER("chinapres2.field.copies"), true);
    // 代煎 (开关, 非必填)
    API_DTO_FIELD_REQUIRE(Int32, decoction, ZH_WORDS_GETTER("chinapres2.field.decoction"), false);
    // 诊断 (必填)
    API_DTO_FIELD_REQUIRE(String, diagnosis, ZH_WORDS_GETTER("chinapres2.field.diagnosis"), true);
    // 备注 (非必填)
    API_DTO_FIELD_REQUIRE(String, remark, ZH_WORDS_GETTER("chinapres2.field.remark"), false);
    //药材(必填)
    API_DTO_FIELD_REQUIRE(String, medicine, ZH_WORDS_GETTER("chinapres2.field.medicine"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !__CHINAPRES2ADDDTO_H_