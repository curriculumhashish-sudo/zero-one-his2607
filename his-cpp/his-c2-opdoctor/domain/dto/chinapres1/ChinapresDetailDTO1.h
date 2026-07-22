#pragma once

#ifndef _CHINAPRESDETAILDTO1_H_
#define _CHINAPRESDETAILDTO1_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class ChinapresHerbDTO: public  oatpp::DTO
{
	DTO_INIT(ChinapresHerbDTO, DTO);
	//药材名称
	API_DTO_FIELD_REQUIRE(String, herbName, ZH_WORDS_GETTER("chinapres.field.herbname"), true);
	//剂量
	API_DTO_FIELD_REQUIRE(String, dosage, ZH_WORDS_GETTER("chinapres.field.dosage"), true);
	//剂量金额
	API_DTO_FIELD_REQUIRE(Float64, unitprice, ZH_WORDS_GETTER("chinapres.field.unitprice"), true);
	//总计
	API_DTO_FIELD_REQUIRE(Float64, subtotal, ZH_WORDS_GETTER("chinapres.field.subtotal"), true);
};

class ChinapresDetailDTO1: public  oatpp::DTO
{
	DTO_INIT(ChinapresDetailDTO1, DTO);
	//处方号
	API_DTO_FIELD_REQUIRE(String, presCno, ZH_WORDS_GETTER("chinapres.field.prescno"), true);
	//处方状态
	API_DTO_FIELD_REQUIRE(String, status, ZH_WORDS_GETTER("chinapres.field.status"),true);
	//用法
	API_DTO_FIELD_REQUIRE(String, usage, ZH_WORDS_GETTER("chinapres.field.usage"), true);
	//频次
	API_DTO_FIELD_REQUIRE(String, frequency, ZH_WORDS_GETTER("chinapres.field.frequency"), true);
	//天数
	API_DTO_FIELD_REQUIRE(Int32, days, ZH_WORDS_GETTER("chinapres.field.days"), true);
	//付数
	API_DTO_FIELD_REQUIRE(Int32, doses, ZH_WORDS_GETTER("chinapres.field.doses"), true);
	//代煎
	API_DTO_FIELD_REQUIRE(Boolean, decoct, ZH_WORDS_GETTER("chinapres.field.decoct"), true);
	//诊断
	API_DTO_FIELD_REQUIRE(String, diagnosis, ZH_WORDS_GETTER("chinapres.field.diagnosis"),false);
	//备注
	API_DTO_FIELD_REQUIRE(String, remark, ZH_WORDS_GETTER("chinapres.field.remark"), false);
	//共几味
	API_DTO_FIELD_REQUIRE(Int32, totalcount, ZH_WORDS_GETTER("chinapres.field.totalcount"), true);
	//单剂金额
	API_DTO_FIELD_REQUIRE(Float64, doseAmount, ZH_WORDS_GETTER("chinapres.field.doseamount"), true);
	//总计金额
	API_DTO_FIELD_REQUIRE(Float64, totalAmount, ZH_WORDS_GETTER("chinapres.field.totalamount"), true);
	//药材列表
	API_DTO_FIELD_REQUIRE(List<Object<ChinapresHerbDTO>>, details, ZH_WORDS_GETTER("chinapres.field.datails"), true);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_CHINAPRESDETAILDTO1_H_