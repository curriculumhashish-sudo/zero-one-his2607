#pragma once
#ifndef _DEPT_DTO_
#define _DEPT_DTO_

#include <string>

#include "../../GlobalInclude.h"

namespace DeptEnum
{
    constexpr const char* HOSPITAL = "HOSPITAL";
    constexpr const char* DEPARTMENT = "DEPARTMENT";
    constexpr const char* OUTPATIENT = "OUTPATIENT";
    constexpr const char* INPATIENT = "INPATIENT";
    constexpr const char* PHARMACY = "PHARMACY";
    constexpr const char* WAREHOUSE = "WAREHOUSE";
    constexpr const char* FINANCE = "FINANCE";
    constexpr const char* NURSE_STATION = "NURSE_STATION";
    constexpr const char* MANAGEMENT = "MANAGEMENT";
    constexpr const char* LOGISTICS_SUPPORT = "LOGISTICS_SUPPORT";
    constexpr const char* STATUS_ENABLED = "1";
    constexpr const char* STATUS_DISABLED = "0";

    bool isType(const std::string& value);
    bool isClass(const std::string& value);
    bool isStatus(const std::string& value);
}

#include OATPP_CODEGEN_BEGIN(DTO)

class DeptAddDTO : public oatpp::DTO
{
    DTO_INIT(DeptAddDTO, DTO);

    // 科室名称
    API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("dept.field.name"), true)
    // 类型
    API_DTO_FIELD_REQUIRE(String, typeEnum, ZH_WORDS_GETTER("dept.field.type-enum"), true)
    // 类别
    API_DTO_FIELD_REQUIRE(String, classEnum, ZH_WORDS_GETTER("dept.field.class-enum"), false)
    // 上级组织编号
    API_DTO_FIELD_REQUIRE(String, parentId, ZH_WORDS_GETTER("dept.field.parent-id"), false)
    // 医保编码
    API_DTO_FIELD_REQUIRE(String, ybNo, ZH_WORDS_GETTER("dept.field.yb-no"), false)
    // 医保名称
    API_DTO_FIELD_REQUIRE(String, ybName, ZH_WORDS_GETTER("dept.field.yb-name"), false)
    // 机构标识
    API_DTO_FIELD_REQUIRE(String, medinsId, ZH_WORDS_GETTER("dept.field.medins-id"), false)
    // 医保区划
    API_DTO_FIELD_REQUIRE(String, medinsAdmdvs, ZH_WORDS_GETTER("dept.field.medins-admdvs"), false)
    // 机构类型
    API_DTO_FIELD_REQUIRE(String, medinsType, ZH_WORDS_GETTER("dept.field.medins-type"), false)
    // 机构等级
    API_DTO_FIELD_REQUIRE(String, medinsLv, ZH_WORDS_GETTER("dept.field.medins-lv"), false)
    // 拼音码
    API_DTO_FIELD_REQUIRE(String, pyStr, ZH_WORDS_GETTER("dept.field.py-str"), false)
    // 五笔码
    API_DTO_FIELD_REQUIRE(String, wbStr, ZH_WORDS_GETTER("dept.field.wb-str"), false)
    // 默认医生编号
    API_DTO_FIELD_REQUIRE(String, defDoctorId, ZH_WORDS_GETTER("dept.field.def-doctor-id"), false)
    // 科别
    API_DTO_FIELD_REQUIRE(String, caty, ZH_WORDS_GETTER("dept.field.caty"), false)
    // 显示顺序
    API_DTO_FIELD(Int32, displayOrder, ZH_WORDS_GETTER("dept.field.display-order"), false, 0)
    // 状态
    API_DTO_FIELD(String, statusEnum, ZH_WORDS_GETTER("dept.field.status-enum"), false, "1")

    // 关联一个PayloadDTO负载数据对象
    CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);

public:
    std::string validate() const;
};

class DeptModifyDTO : public oatpp::DTO
{
    DTO_INIT(DeptModifyDTO, DTO);

    // 科室编号
    API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("dept.field.id"), true)
    // 科室名称
    API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("dept.field.name"), false)
    // 类型
    API_DTO_FIELD_REQUIRE(String, typeEnum, ZH_WORDS_GETTER("dept.field.type-enum"), false)
    // 类别
    API_DTO_FIELD_REQUIRE(String, classEnum, ZH_WORDS_GETTER("dept.field.class-enum"), false)
    // 上级组织编号
    API_DTO_FIELD_REQUIRE(String, parentId, ZH_WORDS_GETTER("dept.field.parent-id"), false)
    // 医保编码
    API_DTO_FIELD_REQUIRE(String, ybNo, ZH_WORDS_GETTER("dept.field.yb-no"), false)
    // 医保名称
    API_DTO_FIELD_REQUIRE(String, ybName, ZH_WORDS_GETTER("dept.field.yb-name"), false)
    // 机构标识
    API_DTO_FIELD_REQUIRE(String, medinsId, ZH_WORDS_GETTER("dept.field.medins-id"), false)
    // 医保区划
    API_DTO_FIELD_REQUIRE(String, medinsAdmdvs, ZH_WORDS_GETTER("dept.field.medins-admdvs"), false)
    // 机构类型
    API_DTO_FIELD_REQUIRE(String, medinsType, ZH_WORDS_GETTER("dept.field.medins-type"), false)
    // 机构等级
    API_DTO_FIELD_REQUIRE(String, medinsLv, ZH_WORDS_GETTER("dept.field.medins-lv"), false)
    // 拼音码
    API_DTO_FIELD_REQUIRE(String, pyStr, ZH_WORDS_GETTER("dept.field.py-str"), false)
    // 五笔码
    API_DTO_FIELD_REQUIRE(String, wbStr, ZH_WORDS_GETTER("dept.field.wb-str"), false)
    // 默认医生编号
    API_DTO_FIELD_REQUIRE(String, defDoctorId, ZH_WORDS_GETTER("dept.field.def-doctor-id"), false)
    // 科别
    API_DTO_FIELD_REQUIRE(String, caty, ZH_WORDS_GETTER("dept.field.caty"), false)
    // 显示顺序
    API_DTO_FIELD_REQUIRE(Int32, displayOrder, ZH_WORDS_GETTER("dept.field.display-order"), false)
    // 状态
    API_DTO_FIELD_REQUIRE(String, statusEnum, ZH_WORDS_GETTER("dept.field.status-enum"), false)

    // 关联一个PayloadDTO负载数据对象
    CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);

public:
    // 校验修改字段；未传或传null的字段不修改，空字符串保存为空字符串
    std::string validate() const;
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_DEPT_DTO_
