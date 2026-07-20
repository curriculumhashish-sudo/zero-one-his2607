#ifndef DIAG_CATEGORY_ADD_DTO_HPP
#define DIAG_CATEGORY_ADD_DTO_HPP
#include <oatpp/core/macro/codegen.hpp>
#include "zh_words_getter.hpp"
#include OATPP_CODEGEN_BEGIN(DTO)

class DiagCategoryAddDTO : public oatpp::DTO
{
    DTO_INIT(DiagCategoryAddDTO, DTO);
    // 必填分类名称
    API_DTO_FIELD_REQUIRE(oatpp::String, catName, ZH_WORDS_GETTER("diagnose.field.category.cat_name"), true);
};

class DiagnoseAddDTO : public oatpp::DTO
{
    DTO_INIT(DiagnoseAddDTO, DTO);
    // 疾病名称 
    DTO_FIELD(oatpp::String, name);
    DTO_FIELD_INFO(name) {
        info->description = ZH_WORDS_GETTER("diagnose.field.diag.name");
        info->required = true;
    }
    // 已有分类的ID
    DTO_FIELD(oatpp::Int64, categoryId);
    DTO_FIELD_INFO(categoryId) {
        info->description = ZH_WORDS_GETTER("diagnose.field.diag.category_id");
        info->required = true;
    }
    // 以下全为诊断业务可空字段
    //诊断类型：西医诊断，中医主证诊断，中医主病诊断
    DTO_FIELD(oatpp::String, diagType);
    DTO_FIELD_INFO(diagType) { info->description = ZH_WORDS_GETTER("diagnose.diag.diag_type"); }
    //诊断编码
    DTO_FIELD(oatpp::String, chronicCode);
    DTO_FIELD_INFO(chronicCode) { info->description = ZH_WORDS_GETTER("diagnose.diag.chronic.code"); }
    //医保编码
    DTO_FIELD(oatpp::String, medicalInsuranceCode);
    DTO_FIELD_INFO(medicalInsuranceCode) { info->description = ZH_WORDS_GETTER("diagnose.diag.medical_insurance_code"); }
    //状态
    DTO_FIELD(oatpp::Int32, status);
    DTO_FIELD_INFO(status) { info->description = ZH_WORDS_GETTER("diagnose.diag.status"); }
    //是否纳入医保
    DTO_FIELD(oatpp::String, remark);
    DTO_FIELD_INFO(remark) { info->description = ZH_WORDS_GETTER("diagnose.diag.medical_insurance_tag"); }

    // 接收拦截器注入的登录用户载荷，仅后端内部传参，和前端JSON无关
    CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
public:
    std::string validate() 
{
    // 1. 基础字段非空
    if(!name || name->empty()) return "疾病名称不能为空";
    if(!categoryId) return "必须选择所属诊断分类";
    // 2. 校验前端传入的categoryId在分类表真实存在（核心：分类一定提前建好）
    bool catExists = diagCategoryDao->existsById(categoryId->getValue());
    if(!catExists) return "所选分类不存在，请先创建对应诊断分类";
    // 3. 同分类下疾病名称不能重复
    bool nameRepeat = diagDao->checkNameRepeat(categoryId->getValue(), name->c_str());
    if(nameRepeat) return "当前分类下已存在同名疾病";
    // 4. 慢病分类联动校验
    auto cat = diagCategoryDao->getById(categoryId->getValue());
    if(cat->isChronicType() && (!chronicCode || chronicCode->empty()))
        return "特慢病分类必须填写慢病编码";
    // 5. 诊断类型枚举限制
    if(diagType && !diagType->empty())
    {
        if(*diagType != "初步" && *diagType != "确诊" && *diagType != "出院诊断")
            return "诊断类型仅支持：初步/确诊/出院诊断";
    }
    return "";
}
};



#include OATPP_CODEGEN_END(DTO)
#endif