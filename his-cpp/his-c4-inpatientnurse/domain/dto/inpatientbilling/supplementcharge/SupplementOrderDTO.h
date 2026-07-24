#pragma once
//医嘱补费列表的传输对象
#ifndef _SUPPLEMENTORDERDTO_H_
#define _SUPPLEMENTORDERDTO_H_

#include "../../../GlobalInclude.h"
#include "ApiHelper.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class SupplementOrderDTO : public oatpp::DTO {

	DTO_INIT(SupplementOrderDTO, DTO);

	//医嘱名称
	API_DTO_FIELD(String, orderName, ZH_WORDS_GETTER("supplementorder.field.orderName"), false, "");

	//开方科室
	API_DTO_FIELD(String, openDeptName, ZH_WORDS_GETTER("supplementorder.field.openDeptName"), false, "");

	//剂量/数量
	API_DTO_FIELD(Int32, dosage, ZH_WORDS_GETTER("supplementorder.field.dosage"), false, 0);

	//频次/状态
	API_DTO_FIELD(String, frequency, ZH_WORDS_GETTER("supplementorder.field.frequency"), false, "");

	//时间
	API_DTO_FIELD(String, time, ZH_WORDS_GETTER("supplementorder.field.time"), false, "");

	//操作
	API_DTO_FIELD(String, operation, ZH_WORDS_GETTER("supplementorder.field.operation"), false, "");

};

#include OATPP_CODEGEN_END(DTO)

#endif //_SUPPLEMENTORDERDTO_H_