#pragma once

#ifndef _EQUIPMENTMODIFYDTO_H_
#define _EQUIPMENTMODIFYDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class EquipmentModifyDTO : public oatpp::DTO
{
	DTO_INIT(EquipmentModifyDTO, DTO);
    // 器材ID (修改必须传ID)
    API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("equipment.field.id"), true);
    // 状态
    API_DTO_FIELD_REQUIRE(Int32, status, ZH_WORDS_GETTER("equipment.field.status"), false);
    // 器材编码 
    API_DTO_FIELD_REQUIRE(String, code, ZH_WORDS_GETTER("equipment.field.code"), false);

    // ================= 1. 基础信息 =================
    // 耗材名称
    API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("equipment.field.name"), true);
    // 商品名称
    API_DTO_FIELD_REQUIRE(String, goods, ZH_WORDS_GETTER("equipment.field.goods"), false);
    // 耗材分类
    API_DTO_FIELD_REQUIRE(String, classification, ZH_WORDS_GETTER("equipment.field.classification"), true);
    // 生产厂家
    API_DTO_FIELD_REQUIRE(String, manufacturer, ZH_WORDS_GETTER("equipment.field.manufacturer"), false);
    // 供应商
    API_DTO_FIELD_REQUIRE(String, supplier, ZH_WORDS_GETTER("equipment.field.supplier"), false);
    // 批准文号
    API_DTO_FIELD_REQUIRE(String, apprno, ZH_WORDS_GETTER("equipment.field.apprno"), false);
    // 包装规格
    API_DTO_FIELD_REQUIRE(String, specification, ZH_WORDS_GETTER("equipment.field.specification"), true);
    // 医保编码
    API_DTO_FIELD_REQUIRE(String, medinscode, ZH_WORDS_GETTER("equipment.field.medinscode"), false);
    // 财务类型
    API_DTO_FIELD_REQUIRE(String, finantype, ZH_WORDS_GETTER("equipment.field.finantype"), true);
    // 医保类别
    API_DTO_FIELD_REQUIRE(String, medinscat, ZH_WORDS_GETTER("equipment.field.medinscat"), false);
    // 医保等级
    API_DTO_FIELD_REQUIRE(String, medinsgra, ZH_WORDS_GETTER("equipment.field.medinsgra"), false);
    // 病案结算类型 
    API_DTO_FIELD_REQUIRE(String, medrecsettle, ZH_WORDS_GETTER("equipment.field.medrecsettle"), false);
    // 医保对码 
    API_DTO_FIELD_REQUIRE(Boolean, is_medins_match, ZH_WORDS_GETTER("equipment.field.is_medins_match"), false);

    // ================= 2. 库存与单位信息 =================
    // 采购单位
    API_DTO_FIELD_REQUIRE(String, purchaunit, ZH_WORDS_GETTER("equipment.field.purchaunit"), true);
    // 采购/拆零倍数
    API_DTO_FIELD_REQUIRE(String, purmin, ZH_WORDS_GETTER("equipment.field.purmin"), true);
    // 采购参考价（包装单位）
    API_DTO_FIELD_REQUIRE(String, purchase, ZH_WORDS_GETTER("equipment.field.purchase"), false);

    // 库存单位
    API_DTO_FIELD_REQUIRE(String, inventory, ZH_WORDS_GETTER("equipment.field.inventory"), true);
    // 库存/拆零倍数
    API_DTO_FIELD_REQUIRE(String, invsplit, ZH_WORDS_GETTER("equipment.field.invstlit"), true);
    // 销售参考价（包装单位）
    API_DTO_FIELD_REQUIRE(String, retail, ZH_WORDS_GETTER("equipment.field.retail"), false);

    // 包装单位
    API_DTO_FIELD_REQUIRE(String, packaging, ZH_WORDS_GETTER("equipment.field.packaging"), true);
    // 包装/拆零倍数
    API_DTO_FIELD_REQUIRE(String, packsplit, ZH_WORDS_GETTER("equipment.field.packsplit"), true);
    // 最高零售价（包装单位）
    API_DTO_FIELD_REQUIRE(String, maxretail, ZH_WORDS_GETTER("equipment.field.maxretail"), false);

    // 拆零单位
    API_DTO_FIELD_REQUIRE(String, split, ZH_WORDS_GETTER("equipment.field.split"), true);

    // 警戒线最高值(库存单位)
    API_DTO_FIELD_REQUIRE(String, warningmax, ZH_WORDS_GETTER("equipment.field.warningmax"), false);
    // 警戒线最低值(库存单位)
    API_DTO_FIELD_REQUIRE(String, warningmin, ZH_WORDS_GETTER("equipment.field.warningmin"), false);

    // 追溯码单位 
    API_DTO_FIELD_REQUIRE(String, traceunit, ZH_WORDS_GETTER("equipment.field.traceunit"), false);
    // 追溯码/拆零倍数 
    API_DTO_FIELD_REQUIRE(String, tracesplit, ZH_WORDS_GETTER("equipment.field.tracesplit"), false);

    // ================= 3. 业务信息 =================
    // 拼音码
    API_DTO_FIELD_REQUIRE(String, pinyin, ZH_WORDS_GETTER("equipment.field.pinyin"), false);
    // 五笔码
    API_DTO_FIELD_REQUIRE(String, wubi, ZH_WORDS_GETTER("equipment.field.wubi"), false);

    // 过敏标记 
    API_DTO_FIELD_REQUIRE(Boolean, is_allergic, ZH_WORDS_GETTER("equipment.field.is_allergic"), false);
    // 医保标记 
    API_DTO_FIELD_REQUIRE(Boolean, is_medins, ZH_WORDS_GETTER("equipment.field.is_medins"), false);
    // 高值耗材标志 
    API_DTO_FIELD_REQUIRE(Boolean, is_high_value, ZH_WORDS_GETTER("equipment.field.is_high_value"), false);
    // 是否收费耗材 
    API_DTO_FIELD_REQUIRE(Boolean, is_chargeable, ZH_WORDS_GETTER("equipment.field.is_chargeable"), false);
    // 说明 
    API_DTO_FIELD_REQUIRE(String, description, ZH_WORDS_GETTER("equipment.field.description"), false);

public:
	// 校验修改数据的必填字段
	std::string validate() const
	{
        // 核心必填
        if (!id || id->empty()) return "器材ID不能为空";

        // 基础信息必填项
        if (!name || name->empty()) return "耗材名称不能为空";
        if (!classification || classification->empty()) return "耗材分类不能为空";
        if (!specification || specification->empty()) return "包装规格不能为空";
        if (!finantype || finantype->empty()) return "财务类型不能为空";

        // 库存与单位信息必填项
        if (!purchaunit || purchaunit->empty()) return "采购单位不能为空";
        if (!purmin || purmin->empty()) return "采购/拆零倍数不能为空";
        if (!inventory || inventory->empty()) return "库存单位不能为空";
        if (!invsplit || invsplit->empty()) return "库存/拆零倍数不能为空";
        if (!packaging || packaging->empty()) return "包装单位不能为空";
        if (!packsplit || packsplit->empty()) return "包装/拆零倍数不能为空";
        if (!split || split->empty()) return "拆零单位不能为空";
		return "";
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif 