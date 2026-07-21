#pragma once

#ifndef _EQUIPMENTADDDTO_H_
#define _EQUIPMENTADDDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 租户添加DTO类
 */
class EquipmentAddDTO : public oatpp::DTO
{
	DTO_INIT(EquipmentAddDTO, DTO);
	
	// 器材名称
	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("equipment.field.name"), true);
	// 器材分类
	API_DTO_FIELD_REQUIRE(String, classification, ZH_WORDS_GETTER("equipment.field.classification"), true);
	// 器材编码
	API_DTO_FIELD_REQUIRE(String, keywords, ZH_WORDS_GETTER("equipment.field.keywords"), false);
	// 商品名称
	API_DTO_FIELD_REQUIRE(String, goods, ZH_WORDS_GETTER("equipment.field.goods"), false);
	// 包装规格
	API_DTO_FIELD_REQUIRE(String, specification, ZH_WORDS_GETTER("equipment.field.specification"), true);
	// 状态
	API_DTO_FIELD_REQUIRE(Int32, status, ZH_WORDS_GETTER("equipment.field.status"), true);
	// 生产厂家
	API_DTO_FIELD_REQUIRE(String, manufacturer, ZH_WORDS_GETTER("equipment.field.manufacturer"), false);
	// 价格
	API_DTO_FIELD_REQUIRE(String, price, ZH_WORDS_GETTER("equipment.field.price"), true);
	// 供应商
	API_DTO_FIELD_REQUIRE(String, supplier, ZH_WORDS_GETTER("equipment.field.supplier"), false);
	// 批注文号
	API_DTO_FIELD_REQUIRE(String, apprno, ZH_WORDS_GETTER("equipment.field.apprno"), false);
	// 财务类型Financial Type
	API_DTO_FIELD_REQUIRE(String, finantype, ZH_WORDS_GETTER("equipment.field.finantype"), true);
	// 医保类别Med Ins Cat.
	API_DTO_FIELD_REQUIRE(String, medinscat, ZH_WORDS_GETTER("equipment.field.medinscat"), false);
	// 医保等级
	API_DTO_FIELD_REQUIRE(String, medinsgra, ZH_WORDS_GETTER("equipment.field.medinsgra"), false);
	// 采购单位
	API_DTO_FIELD_REQUIRE(String, purchaunit, ZH_WORDS_GETTER("equipment.field.purchaunit"), true);
	// 采购/拆零倍数pur/min
	API_DTO_FIELD_REQUIRE(String, purmin, ZH_WORDS_GETTER("equipment.field.purmin"), true);
	// 购入价（包装单位）
	API_DTO_FIELD_REQUIRE(String, purchase, ZH_WORDS_GETTER("equipment.field.purchase"), true);
	// 库存单位
	API_DTO_FIELD_REQUIRE(String, inventory, ZH_WORDS_GETTER("equipment.field.inventory"), true);
	// 库存/拆零倍数
	API_DTO_FIELD_REQUIRE(String, invsplit, ZH_WORDS_GETTER("equipment.field.invstlit"), true);
	// 零售价（包装单位）
	API_DTO_FIELD_REQUIRE(String, retail, ZH_WORDS_GETTER("equipment.field.retail"), true);
	// 包装单位
	API_DTO_FIELD_REQUIRE(String, packaging, ZH_WORDS_GETTER("equipment.field.packaging"), true);
	// 包装/拆零倍数Packaging / Split bulk
	API_DTO_FIELD_REQUIRE(String, packsplit, ZH_WORDS_GETTER("equipment.field.packsplit"), true);
	// 拆零单位
	API_DTO_FIELD_REQUIRE(String, split, ZH_WORDS_GETTER("equipment.field.split"), true);
	// 最高零售价（包装单位）
	API_DTO_FIELD_REQUIRE(String, maxretail, ZH_WORDS_GETTER("equipment.field.maxretail"), false);
	// 警戒线最高值(库存单位)
	API_DTO_FIELD_REQUIRE(String, warningmax, ZH_WORDS_GETTER("equipment.field.warningmax"), false);
	// 警戒线最低值(库存单位)
	API_DTO_FIELD_REQUIRE(String, warningmin, ZH_WORDS_GETTER("equipment.field.warningmin"), false);
	// 拼音码
	API_DTO_FIELD_REQUIRE(String, pinyin, ZH_WORDS_GETTER("equipment.field.pinyin"), false);
	// 五笔码
	API_DTO_FIELD_REQUIRE(String, wubi, ZH_WORDS_GETTER("equipment.field.wubi"), false);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_EQUIPMENTADDDTO_H_