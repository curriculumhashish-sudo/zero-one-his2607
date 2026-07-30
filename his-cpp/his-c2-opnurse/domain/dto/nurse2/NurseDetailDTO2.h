#ifndef _NURSEDETAILDTO_H_
#define _NURSEDETAILDTO_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// 执行后的数据响应
class NurseDetailDTO2 : public oatpp::DTO
{
	DTO_INIT(NurseDetailDTO2, DTO);

	
	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("nurse2.field.name_code"), true);
	
	API_DTO_FIELD_REQUIRE(String, state, ZH_WORDS_GETTER("nurse2.field.state"), false);
	
	API_DTO_FIELD_REQUIRE(String, content, ZH_WORDS_GETTER("nurse2.field.content"), false);

	API_DTO_FIELD_REQUIRE(Int32, amount, ZH_WORDS_GETTER("nurse2.field.amount"), false);
	
	API_DTO_FIELD_REQUIRE(String, person, ZH_WORDS_GETTER("nurse2.field.person"), false);
	
	API_DTO_FIELD_REQUIRE(String, operate, ZH_WORDS_GETTER("nurse2.field.operate"), false);
};

// 护士信息分页查询返回数据（分页响应结构体）
class NurseDetailPageDTO : public PageDTO<NurseDetailDTO2::Wrapper>
{
	DTO_INIT(NurseDetailPageDTO, PageDTO<NurseDetailDTO2::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_NURSEDETAILDTO_H_
