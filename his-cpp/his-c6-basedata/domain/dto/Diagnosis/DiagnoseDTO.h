#ifndef DIAG_CATEGORY_ADD_DTO_HPP
#define DIAG_CATEGORY_ADD_DTO_HPP
#include <oatpp/core/macro/codegen.hpp>
#include "../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)
//
//class DiagCategoryAddDTO : public oatpp::DTO
//{
//    DTO_INIT(DiagCategoryAddDTO, DTO);
//    // 已有分类的ID
////DTO_FIELD(oatpp::Int64, categoryId);
////DTO_FIELD_INFO(categoryId) {
////    info->description = ZH_WORDS_GETTER("diagnose.field.diag.category_id");
////    info->required = true;
////}
//    // 必填分类名称
//    API_DTO_FIELD_REQUIRE(oatpp::String, catName, ZH_WORDS_GETTER("diagnose.field.category.cat_name"), true);
//};

class DiagnoseAddDTO : public oatpp::DTO
{
    DTO_INIT(DiagnoseAddDTO, DTO);
    // 疾病名称 
    DTO_FIELD(oatpp::String, name);
    DTO_FIELD_INFO(name) {
        info->description = ZH_WORDS_GETTER("diagnose.field.diag.name");
        info->required = true;
    }
    ////主键--序号
    // DTO_FIELD(oatpp::String, id);
    //DTO_FIELD_INFO(id) {
    //    info->description = ZH_WORDS_GETTER("diagnose.field.diag.id");
    //    info->required = true;
    //}

    //// 已有分类的ID
    //DTO_FIELD(oatpp::Int64, categoryId);
    //DTO_FIELD_INFO(categoryId) {
    //    info->description = ZH_WORDS_GETTER("diagnose.field.diag.category_id");
    //    info->required = true;
    //}
    // 以下全为诊断业务可空字段
    //诊断类型：西医诊断，中医主证诊断，中医主病诊断
    DTO_FIELD(oatpp::String, diagType);
    DTO_FIELD_INFO(diagType) { info->description = ZH_WORDS_GETTER("diagnose..field.diag.diag_type"); }
    //诊断编码
    DTO_FIELD(oatpp::String, chronicCode);
    DTO_FIELD_INFO(chronicCode) { info->description = ZH_WORDS_GETTER("diagnose.field.diag.chronic.code"); }
    //医保编码
    DTO_FIELD(oatpp::String, medicalInsuranceCode);
    DTO_FIELD_INFO(medicalInsuranceCode) { info->description = ZH_WORDS_GETTER("diagnose.field.diag.medical_insurance_code"); }
    //状态
    DTO_FIELD(oatpp::Int32, status);
    DTO_FIELD_INFO(status) { info->description = ZH_WORDS_GETTER("diagnose.diag.status"); }
    // 医保标记（复选框bool）
    DTO_FIELD(oatpp::Int32, medicalInsuranceTag);
    DTO_FIELD_INFO(medicalInsuranceTag) {
        info->description = ZH_WORDS_GETTER("diagnose.field.diag.medical_insurance_tag");
    }

    // 医保对码（复选框bool）
    DTO_FIELD(oatpp::Int32, medicalInsuranceMatch);
    DTO_FIELD_INFO(medicalInsuranceMatch) {
        info->description = ZH_WORDS_GETTER("diagnose.field.diag.medical_insurance_match");
    }

    // 拼音助记码
    DTO_FIELD(oatpp::String, pinyinCode);
    DTO_FIELD_INFO(pinyinCode) {
        info->description = ZH_WORDS_GETTER("diagnose.field.diag.pinyin_code");
    }

    // 五笔助记码
    DTO_FIELD(oatpp::String, wubiCode);
    DTO_FIELD_INFO(wubiCode) {
        info->description = ZH_WORDS_GETTER("diagnose.field.diag.wubi_code");
    }

    // 删除标记
    DTO_FIELD(oatpp::Int32, isDelete);
    DTO_FIELD_INFO(isDelete) {
        info->description = ZH_WORDS_GETTER("diagnose.field.diag.is_delete");
    }
  
    // 说明/备注
    DTO_FIELD(oatpp::String, remark);
    DTO_FIELD_INFO(remark) {
        info->description = ZH_WORDS_GETTER("diagnose.field.diag.remark");
    }
    // 必填分类名称
    API_DTO_FIELD_REQUIRE(oatpp::String, catName, ZH_WORDS_GETTER("diagnose.field.category.cat_name"), true);

    // 接收拦截器注入的登录用户载荷，仅后端内部传参，和前端JSON无关
    CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
public:
    std::string validate() 
{
        // 1. 必填项校验
        if (!name || name->empty())
            return "The name of a disease cannot be empty";
        if (!catName || catName->empty())
            return "The category name cannot be empty";

        // 2. 诊断类型枚举校验（有传入才校验，不传不拦截）
        if (diagType && !diagType->empty())
        {
            if (*diagType != "western_medicine_diagnosis" &&
                *diagType != "chinese_main_disease_diagnosis" &&
                *diagType != " chinese_main_symptom_diagnosis")
            {
                return "diagnosis type invalidate";
            }
        }



        // 3. 数字标记字段校验，仅允许0/1
        if (status)
        {
            auto v = *status;
            if (v != 0 && v != 1)
                return "The status only allows 0 or 1";
        }
        if (medicalInsuranceTag)
        {
            auto v = *medicalInsuranceTag;
            if (v != 0 && v != 1)
                return "The medical insurance marking only allows 0 or 1";
        }
        if (medicalInsuranceMatch)
        {
            auto v = *medicalInsuranceMatch;
            if (v != 0 && v != 1)
                return "The medical insurance code marking only allows 0 or 1";
        }
        if (isDelete)
        {
            auto v = *isDelete;
            if (v != 0 && v != 1)
                return "Delete the tag only allows 0 or 1";
        }

    return "";
}
};

class DiagnoseDTO : public DiagnoseAddDTO
{
    DTO_INIT(DiagnoseDTO, DiagnoseAddDTO);

    DTO_FIELD(String, id);
    DTO_FIELD_INFO(id) {
        info->description = ZH_WORDS_GETTER("diagnose.field.diag.id");
    }
};


#include OATPP_CODEGEN_END(DTO)
#endif