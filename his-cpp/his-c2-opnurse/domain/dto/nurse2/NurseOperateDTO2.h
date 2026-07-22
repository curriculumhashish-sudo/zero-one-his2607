#pragma once


#ifndef  _NURSEOPERATEDTO_H_
#define  _NURSEOPERATEDTO_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)


//执行
class NurseOperateDTO2: public oatpp::DTO
{
	//初始化
	DTO_INIT(NurseOperateDTO2, DTO);
	//执行人姓名 （隐藏的） 执行谁？？
	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("nurse2.field.name_code"), true);
	//状态
	API_DTO_FIELD_REQUIRE(String, state, ZH_WORDS_GETTER("nurse2.field.state"), false);
	//医嘱内容
	API_DTO_FIELD_REQUIRE(String, content, ZH_WORDS_GETTER("nurse2.field.content"), false);
	//已执行次数
	API_DTO_FIELD_REQUIRE(String, amount, ZH_WORDS_GETTER("nurse2.field.amount"), false);
	//执行人
	API_DTO_FIELD_REQUIRE(String, person, ZH_WORDS_GETTER("nurse2.field.person"), false);
	//操作
	API_DTO_FIELD_REQUIRE(String, operate, ZH_WORDS_GETTER("nurse2.field.operate"), false);

};



#include OATPP_CODEGEN_END(DTO)

#endif // ! _NURSEOPERATEDTO_H_
