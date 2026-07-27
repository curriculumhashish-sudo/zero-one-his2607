#pragma once
#ifndef _RECEPTIONPATIENTQUERY_H_
#define _RECEPTIONPATIENTQUERY_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class RecePatientQuery : public oatpp::DTO
{
	DTO_INIT(RecePatientQuery, DTO);

	//æÕ’Ô ±º‰
	API_DTO_FIELD_REQUIRE(String, visitTime, ZH_WORDS_GETTER("recp.query.data"), true);

};



#include OATPP_CODEGEN_END(DTO)

#endif // !_RECEPTIONPATIENTQUERY_H_
