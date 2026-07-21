#ifndef _PATIENTACTDTO_H_
#define _PATIENTACTDTO_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PatientActDTO :public oatpp::DTO
{
	DTO_INIT(PatientActDTO, DTO);
	// 状态
	API_DTO_FIELD_REQUIRE(Int32, status, ZH_WORDS_GETTER("patient.field.status"), false);
	// 执行项目
	API_DTO_FIELD_REQUIRE(String, OrderContent, ZH_WORDS_GETTER("patient.field.OrderContent"), false);

};

/**
 * 定义一个租户信息分页传输对象.
 */
class PatientActPageDTO : public PageDTO<PatientActDTO::Wrapper>
{
	DTO_INIT(PatientActPageDTO, PageDTO<PatientActDTO::Wrapper>);
};
#include OATPP_CODEGEN_END(DTO)
#endif // !_PATIENTACTDTO_H_
