#pragma once


#ifndef _NURSEDETAILDTO_H_
#define _NURSEDETAILDTO_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)


//执行后的数据响应  
class NurseDetailDTO2 : public oatpp::DTO
{
	//初始化
	DTO_INIT(NurseDetailDTO2, DTO);
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

// 患者信息分页传输对象
class NurseDetailPageDTO : public PageDTO<NurseDetailDTO2::Wrapper>
{
	DTO_INIT(NurseDetailPageDTO, PageDTO<NurseDetailDTO2::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_NurseEXECUADDDTO_H_
