#pragma once
#ifndef _ENTERORDERSDTO_H_
#define _ENTERORDERSDTO_H_

#include "../../GlobalInclude.h"
#include "../../../../lib-oatpp/include/domain/query/PageQuery.h"
#include OATPP_CODEGEN_BEGIN(DTO)
//EnterOrders
//enter-orders
//ENTERORDERS


//定义获取医嘱列表（条件+分页）DTO类
class EnterOrdersQueryDTO :public oatpp::DTO
{
	//定义
	DTO_INIT(EnterOrdersQueryDTO, DTO);
	//医嘱主键
	API_DTO_FIELD_REQUIRE(String, request_id, ZH_WORDS_GETTER("enter-orders.request_id"), true);
	//租户id
	API_DTO_FIELD_REQUIRE(String, tenant_id, ZH_WORDS_GETTER("enter-orders.tenant_id"), true);
	//诊断id
	API_DTO_FIELD_REQUIRE(String, encounter_id, ZH_WORDS_GETTER("enter-orders.encounter_id"), true);
	//患者id
	API_DTO_FIELD_REQUIRE(String, patient_id, ZH_WORDS_GETTER("enter-orders.patient_id"), true);
	//小组id
	API_DTO_FIELD_REQUIRE(String, group_id, ZH_WORDS_GETTER("enter-orders.group_id"), true);
	//医嘱状态
	API_DTO_FIELD_REQUIRE(String, status, ZH_WORDS_GETTER("enter-orders.status"), true);
	//药品类型
	API_DTO_FIELD_REQUIRE(String, medicine_type, ZH_WORDS_GETTER("enter-orders.medicine_type"), true);
	//医嘱类型
	API_DTO_FIELD_REQUIRE(String, therapy_type, ZH_WORDS_GETTER("enter-orders.therapy_type"), true);
	//药品名称
	API_DTO_FIELD_REQUIRE(String, medicine_name, ZH_WORDS_GETTER("enter-orders.medicine_name"), true);
	//药品规格
	API_DTO_FIELD_REQUIRE(String, specification, ZH_WORDS_GETTER("enter-orders.specification"), true);
	//单次剂量
	API_DTO_FIELD_REQUIRE(String, dose, ZH_WORDS_GETTER("enter-orders.dose"), true);
	//剂量单位
	API_DTO_FIELD_REQUIRE(String, dose_unit, ZH_WORDS_GETTER("enter-orders.dose_unit"), true);
	//服用方式
	API_DTO_FIELD_REQUIRE(String, method, ZH_WORDS_GETTER("enter-orders.method"), true);
	//服用时间
	API_DTO_FIELD_REQUIRE(String, rate, ZH_WORDS_GETTER("enter-orders.rate"), true);
	//总金额
	API_DTO_FIELD_REQUIRE(String, taotal_price, ZH_WORDS_GETTER("enter-orders.taotal_price"), true);
	//诊断描述
	API_DTO_FIELD_REQUIRE(String, description, ZH_WORDS_GETTER("enter-orders.description"), true);
	//开立医生
	API_DTO_FIELD_REQUIRE(String, authored_by, ZH_WORDS_GETTER("enter-orders.authored_by"), true);
	//皮试标志
	API_DTO_FIELD_REQUIRE(Boolean, skin_text, ZH_WORDS_GETTER("enter-orders.skin_text"), true);
};
class EnterOrdersQueryPageDTO : public PageDTO<EnterOrdersQueryDTO::Wrapper>
{
	DTO_INIT(EnterOrdersQueryPageDTO, PageDTO<EnterOrdersQueryDTO::Wrapper>);
};


//定义新增医嘱DTO类
class EnterOrdersAddDTO :public oatpp::DTO
{
	//定义
	DTO_INIT(EnterOrdersAddDTO, DTO);
	////医嘱主键
	//API_DTO_FIELD_REQUIRE(String, request_id, ZH_WORDS_GETTER("enter-orders.request_id"), true);
	//租户id
	API_DTO_FIELD_REQUIRE(String, tenant_id, ZH_WORDS_GETTER("enter-orders.tenant_id"), true);
	//诊断id
	API_DTO_FIELD_REQUIRE(String, encounter_id, ZH_WORDS_GETTER("enter-orders.encounter_id"), true);
	//患者id
	API_DTO_FIELD_REQUIRE(String, patient_id, ZH_WORDS_GETTER("enter-orders.patient_id"), true);
	//小组id
	API_DTO_FIELD_REQUIRE(String, group_id, ZH_WORDS_GETTER("enter-orders.group_id"), true);
	//医嘱状态
	API_DTO_FIELD_REQUIRE(String, status, ZH_WORDS_GETTER("enter-orders.status"), true);
	//药品类型
	API_DTO_FIELD_REQUIRE(String, medicine_type, ZH_WORDS_GETTER("enter-orders.medicine_type"), true);
	//医嘱类型
	API_DTO_FIELD_REQUIRE(String, therapy_type, ZH_WORDS_GETTER("enter-orders.therapy_type"), true);
	//药品名称
	API_DTO_FIELD_REQUIRE(String, medicine_name, ZH_WORDS_GETTER("enter-orders.medicine_name"), true);
	//药品规格
	API_DTO_FIELD_REQUIRE(String, specification, ZH_WORDS_GETTER("enter-orders.specification"), true);
	//单次剂量
	API_DTO_FIELD_REQUIRE(String, dose, ZH_WORDS_GETTER("enter-orders.dose"), true);
	//剂量单位
	API_DTO_FIELD_REQUIRE(String, dose_unit, ZH_WORDS_GETTER("enter-orders.dose_unit"), true);
	//服用方式
	API_DTO_FIELD_REQUIRE(String, method, ZH_WORDS_GETTER("enter-orders.method"), true);
	//服用时间
	API_DTO_FIELD_REQUIRE(String, rate, ZH_WORDS_GETTER("enter-orders.rate"), true);
	//新增时间
	API_DTO_FIELD_REQUIRE(String, created_at, ZH_WORDS_GETTER("enter-orders.created_at"), true);
	//新增人id
	API_DTO_FIELD_REQUIRE(String, created_by, ZH_WORDS_GETTER("enter-orders.created_by"), true);
	//总金额
	API_DTO_FIELD_REQUIRE(String, taotal_price, ZH_WORDS_GETTER("enter-orders.taotal_price"), true);
	//诊断描述
	API_DTO_FIELD_REQUIRE(String, description, ZH_WORDS_GETTER("enter-orders.description"), true);
	//皮试标志
	API_DTO_FIELD_REQUIRE(Boolean, skin_text, ZH_WORDS_GETTER("enter-orders.skin_text"), true);
};

//定义修改医嘱DTO类
class EnterOrdersModifyDTO :public oatpp::DTO
{
	//定义
	DTO_INIT(EnterOrdersModifyDTO, DTO);
	//医嘱主键
	API_DTO_FIELD_REQUIRE(String, request_id, ZH_WORDS_GETTER("enter-orders.request_id"), true);
	//药品类型
	API_DTO_FIELD_REQUIRE(String, medicine_type, ZH_WORDS_GETTER("enter-orders.medicine_type"), true);
	//医嘱类型
	API_DTO_FIELD_REQUIRE(String, therapy_type, ZH_WORDS_GETTER("enter-orders.therapy_type"), true);
	//药品名称
	API_DTO_FIELD_REQUIRE(String, medicine_name, ZH_WORDS_GETTER("enter-orders.medicine_name"), true);
	//药品规格
	API_DTO_FIELD_REQUIRE(String, specification, ZH_WORDS_GETTER("enter-orders.specification"), true);
	//单次剂量
	API_DTO_FIELD_REQUIRE(String, dose, ZH_WORDS_GETTER("enter-orders.dose"), true);
	//剂量单位
	API_DTO_FIELD_REQUIRE(String, dose_unit, ZH_WORDS_GETTER("enter-orders.dose_unit"), true);
	//服用方式
	API_DTO_FIELD_REQUIRE(String, method, ZH_WORDS_GETTER("enter-orders.method"), true);
	//服用时间
	API_DTO_FIELD_REQUIRE(String, rate, ZH_WORDS_GETTER("enter-orders.rate"), true);
	//修改时间
	API_DTO_FIELD_REQUIRE(String, created_at, ZH_WORDS_GETTER("enter-orders.created_at"), true);
	//修改人id
	API_DTO_FIELD_REQUIRE(String, created_by, ZH_WORDS_GETTER("enter-orders.created_by"), true);
	//总金额
	API_DTO_FIELD_REQUIRE(String, taotal_price, ZH_WORDS_GETTER("enter-orders.taotal_price"), true);
	//诊断描述
	API_DTO_FIELD_REQUIRE(String, description, ZH_WORDS_GETTER("enter-orders.description"), true);
	//皮试标志
	API_DTO_FIELD_REQUIRE(Boolean, skin_text, ZH_WORDS_GETTER("enter-orders.skin_text"), true);
};

////定义删除医嘱DTO类
//class EnterOrdersDTO :public oatpp::DTO
//{
//	//定义
//	DTO_INIT(EnterOrdersModifyDTO, DTO);
//	//医嘱主键
//	API_DTO_FIELD_REQUIRE(String, request_id, ZH_WORDS_GETTER("enter-orders.request_id"), true);
//};

#include OATPP_CODEGEN_END(DTO)
#endif
