#pragma once

#ifndef _EQUIPMENTADDDTO_H_
#define _EQUIPMENTADDDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 器材添加DTO类
 */
class EquipmentAddDTO : public oatpp::DTO
{
	DTO_INIT(EquipmentAddDTO, DTO);
	
    // 状态
    API_DTO_FIELD_REQUIRE(Int32, status, ZH_WORDS_GETTER("equipment.field.status"), false);
    // 目录编码Catalog Code 
    API_DTO_FIELD_REQUIRE(String, catalogcode, ZH_WORDS_GETTER("equipment.field.catalogcode"), false);

    // ================= 1. 基础信息 =================
    // 耗材名称 (必填)
    API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("equipment.field.name"), true);
    // 商品名称 (对应DB: merchandise_name)
    API_DTO_FIELD_REQUIRE(String, goods, ZH_WORDS_GETTER("equipment.field.goods"), false);
    // 耗材分类 (必填)
    API_DTO_FIELD_REQUIRE(String, classification, ZH_WORDS_GETTER("equipment.field.classification"), true);
    // 生产厂家 (对应DB: manufacturer_text)
    API_DTO_FIELD_REQUIRE(String, manufacturer, ZH_WORDS_GETTER("equipment.field.manufacturer"), false);
    // 供应商 (对应DB: supply_id)
    API_DTO_FIELD_REQUIRE(String, supplier, ZH_WORDS_GETTER("equipment.field.supplier"), false);
    // 批准文号 (对应DB: approval_number)
    API_DTO_FIELD_REQUIRE(String, apprno, ZH_WORDS_GETTER("equipment.field.apprno"), false);
    // 包装规格 (必填)
    API_DTO_FIELD_REQUIRE(String, specification, ZH_WORDS_GETTER("equipment.field.specification"), true);
    // 医保编码 (对应DB: yb_no)
    API_DTO_FIELD_REQUIRE(String, medcode, ZH_WORDS_GETTER("equipment.field.medcode"), false);
    // 财务类型 (必填，对应DB: type_code / fin_type_code)
    API_DTO_FIELD_REQUIRE(String, finantype, ZH_WORDS_GETTER("equipment.field.finantype"), true);
    // 医保类别 (对应DB: yb_type)
    API_DTO_FIELD_REQUIRE(String, medinscat, ZH_WORDS_GETTER("equipment.field.medinscat"), false);
    // 医保等级 (对应DB: chrgitm_lv)
    API_DTO_FIELD_REQUIRE(Int32, medinsgra, ZH_WORDS_GETTER("equipment.field.medinsgra"), false);
    // 病案结算类型
    API_DTO_FIELD_REQUIRE(String, medrecsettle, ZH_WORDS_GETTER("equipment.field.medrecsettle"), false);
    // 医保对码标记 (复选框，对应DB: yb_match_flag)
    API_DTO_FIELD_REQUIRE(Boolean, is_medins_match, ZH_WORDS_GETTER("equipment.field.is_medins_match"), false);

    // ================= 2. 库存与单位信息 =================
    // 采购单位 (必填)
    API_DTO_FIELD_REQUIRE(String, purchaunit, ZH_WORDS_GETTER("equipment.field.purchaunit"), true);
    // 采购单位->拆零单位换算倍数 (必填，对应DB: purchase_unit_ratio)
    API_DTO_FIELD_REQUIRE(Float64, purmin, ZH_WORDS_GETTER("equipment.field.purmin"), true);
    // 【特殊处理】采购参考价（包装单位，必填，需存入附加价格表）
    API_DTO_FIELD_REQUIRE(Float64, purchase, ZH_WORDS_GETTER("equipment.field.purchase"), true);

    // 库存单位 (必填，对应DB: stock_unit_code)
    API_DTO_FIELD_REQUIRE(String, inventory, ZH_WORDS_GETTER("equipment.field.inventory"), true);
    // 库存单位->拆零单位换算倍数 (必填，对应DB: stock_unit_ratio)
    API_DTO_FIELD_REQUIRE(Float64, invsplit, ZH_WORDS_GETTER("equipment.field.invstlit"), true);
    // 【特殊处理】销售参考价（包装单位，必填，需存入附加价格表）
    API_DTO_FIELD_REQUIRE(Float64, retail, ZH_WORDS_GETTER("equipment.field.retail"), true);

    // 包装单位/销售单位 (必填，对应DB: package_unit_code)
    API_DTO_FIELD_REQUIRE(String, packaging, ZH_WORDS_GETTER("equipment.field.packaging"), true);
    // 包装单位->拆零单位换算倍数 (必填，对应DB: package_unit_ratio)
    API_DTO_FIELD_REQUIRE(Float64, packsplit, ZH_WORDS_GETTER("equipment.field.packsplit"), true);
    // 【特殊处理】最高零售价（包装单位，非必填，需存入附加价格表）
    API_DTO_FIELD_REQUIRE(Float64, maxretail, ZH_WORDS_GETTER("equipment.field.maxretail"), false);

    // 拆零单位 (必填，对应DB: split_unit_code，换算基准单位，倍数为1)
    API_DTO_FIELD_REQUIRE(String, split, ZH_WORDS_GETTER("equipment.field.split"), true);
    // 警戒线最大库存 (对应DB: item_max_quantity)
    API_DTO_FIELD_REQUIRE(Float64, warningmax, ZH_WORDS_GETTER("equipment.field.warningmax"), false);
    // 警戒线最小库存 (对应DB: item_min_quantity)
    API_DTO_FIELD_REQUIRE(Float64, warningmin, ZH_WORDS_GETTER("equipment.field.warningmin"), false);
    // 追溯码单位
    API_DTO_FIELD_REQUIRE(String, traceunit, ZH_WORDS_GETTER("equipment.field.traceunit"), false);
    // 追溯码/拆零倍数
    API_DTO_FIELD_REQUIRE(Float64, tracesplit, ZH_WORDS_GETTER("equipment.field.tracesplit"), false);

    // ================= 3. 业务信息 =================
     // 拼音检索码 (对应DB: merchandise_py_str)
    API_DTO_FIELD_REQUIRE(String, pinyin, ZH_WORDS_GETTER("equipment.field.pinyin"), false);
    // 五笔检索码 (对应DB: merchandise_wb_str)
    API_DTO_FIELD_REQUIRE(String, wubi, ZH_WORDS_GETTER("equipment.field.wubi"), false);

    // 过敏标记 (复选框，对应DB: allergen_flag)
    API_DTO_FIELD_REQUIRE(Boolean, is_allergic, ZH_WORDS_GETTER("equipment.field.is_allergic"), false);
    // 医保标记 (复选框，对应DB: yb_flag)
    API_DTO_FIELD_REQUIRE(Boolean, is_medins, ZH_WORDS_GETTER("equipment.field.is_medins"), false);
    // 高值器材标志 (复选框，对应DB: hvcm_flag)
    API_DTO_FIELD_REQUIRE(Boolean, is_high_value, ZH_WORDS_GETTER("equipment.field.is_high_value"), false);
    // 是否收费耗材 (复选框，通常依据此标识判断是否允许开立计费)
    API_DTO_FIELD_REQUIRE(Boolean, is_chargeable, ZH_WORDS_GETTER("equipment.field.is_chargeable"), false);
    // 器材说明 (对应DB: description)
    API_DTO_FIELD_REQUIRE(String, description, ZH_WORDS_GETTER("equipment.field.description"), false);

    // 关联一个PayloadDTO负载数据对象
    CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);


public:
	// 数据校验
	std::string validate()
	{
        // 1. 基础信息必填校验
        if (!name || name->empty()) return "耗材名称不能为空 (name invalid)";
        if (!classification || classification->empty()) return "耗材分类不能为空 (classification invalid)";
        if (!specification || specification->empty()) return "包装规格不能为空 (specification invalid)";
        if (!finantype || finantype->empty()) return "财务类型不能为空 (finantype invalid)";

        // 2. 库存与单位倍数校验
        if (!purchaunit || purchaunit->empty()) return "采购单位不能为空 (purchaunit invalid)";
        if (!purmin || *purmin <= 0) return "采购/拆零倍数必须大于0 (purmin invalid)";

        if (!inventory || inventory->empty()) return "库存单位不能为空 (inventory invalid)";
        if (!invsplit || *invsplit <= 0) return "库存/拆零倍数不能为空 (invsplit invalid)";

        if (!packaging || packaging->empty()) return "包装单位不能为空 (packaging invalid)";
        if (!packsplit || *packsplit <= 0) return "包装/拆零倍数不能为空 (packsplit invalid)";

        if (!split || split->empty()) return "拆零单位不能为空 (split invalid)";
        if (!purchase) return "采购参考价不能为空 (purchase invalid)";
        if (!retail) return "销售参考价不能为空 (retail invalid)";


        return "";
	}
};

/**
 * 示例传输对象
 */
class EquipmentDTO : public EquipmentAddDTO
{
	DTO_INIT(EquipmentDTO, EquipmentAddDTO);
	// 编号
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("equipment.field.id"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_EQUIPMENTADDDTO_H_