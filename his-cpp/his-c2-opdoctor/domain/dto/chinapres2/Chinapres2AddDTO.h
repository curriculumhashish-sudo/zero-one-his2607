#ifndef __CHINAPRES2ADDDTO_H_
#define __CHINAPRES2ADDDTO_H_

#include "../../GlobalInclude.h"
#include "Chinapres2DetailAddDTO.h"  // 引入明细DTO

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 中医处方新增DTO（头部 + 明细列表）
 */
class Chinapres2AddDTO : public oatpp::DTO
{
    DTO_INIT(Chinapres2AddDTO, DTO);

    // ---------- 头部字段（全部保留） ----------
    // 用法 (必填)
    API_DTO_FIELD_REQUIRE(String, usage, ZH_WORDS_GETTER("chinapres2.field.usage"), true);
    // 频次 (必填)
    API_DTO_FIELD_REQUIRE(String, frequency, ZH_WORDS_GETTER("chinapres2.field.frequency"), true);
    // 天数 (必填)
    API_DTO_FIELD_REQUIRE(Int32, days, ZH_WORDS_GETTER("chinapres2.field.days"), true);
    // 付数 (必填)
    API_DTO_FIELD_REQUIRE(Int32, copies, ZH_WORDS_GETTER("chinapres2.field.copies"), true);
    // 代煎 (开关, 非必填)
    API_DTO_FIELD_REQUIRE(Int32, decoction, ZH_WORDS_GETTER("chinapres2.field.decoction"), false);
    // 诊断 (必填)
    API_DTO_FIELD_REQUIRE(String, diagnosis, ZH_WORDS_GETTER("chinapres2.field.diagnosis"), true);
    // 备注 (非必填)
    API_DTO_FIELD_REQUIRE(String, remark, ZH_WORDS_GETTER("chinapres2.field.remark"), false);

     //---------- 核心改动：药材列表（必填） ----------
     //现在改为明细列表
    API_DTO_FIELD_REQUIRE(oatpp::List<Chinapres2DetailAddDTO::Wrapper>, details, ZH_WORDS_GETTER("chinapres2.field.details"), true);

public:
    // 数据校验（包含明细列表校验）
    std::string validate() {
        // 校验头部必填字段（你原来可能在其他地方做了校验，这里补上）
        if (!usage || usage->empty()) return "usage is required.";
        if (!frequency || frequency->empty()) return "frequency is required.";
        if (!days || days <= 0) return "days must be positive.";
        if (!copies || copies <= 0) return "copies must be positive.";
        if (!diagnosis || diagnosis->empty()) return "diagnosis is required.";

        //// 校验明细列表
        if (!details || details->empty()) return "details list cannot be empty.";
        for (auto& detail : *details) {
            if (!detail->medicineId || detail->medicineId->empty()) {
                return "medicineId is required in each detail.";
            }
            if (!detail->quantity || detail->quantity <= 0) {
                return "quantity must be positive in each detail.";
            }
        }
        return "";  // 空字符串表示校验通过
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif // !__CHINAPRES2ADDDTO_H_