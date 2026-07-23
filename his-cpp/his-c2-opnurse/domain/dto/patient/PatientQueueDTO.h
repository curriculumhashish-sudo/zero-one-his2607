#ifndef _PATIENTACTDTO_H_
#define _PATIENTACTDTO_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PatientQueueDTO : public oatpp::DTO
{
	DTO_INIT(PatientQueueDTO, DTO);
	// 姓名
	API_DTO_FIELD_REQUIRE(string, name, ZH_WORDS_GETTER("patient.field.status"), true);
	// 性别
	API_DTO_FIELD_REQUIRE(string, orderContent, ZH_WORDS_GETTER("patient.field.orderContent"), true);
	// 年龄
	API_DTO_FIELD_REQUIRE(string, age, ZH_WORDS_GETTER("patient.field.age"), true);
	// 就诊时间
	API_DTO_FIELD_REQUIRE(String, visitTime, ZH_WORDS_GETTER("patientqueue.field.visitTime"), true);
	// 序号队列
	API_DTO_FIELD_REQUIRE(String, queueNumber, ZH_WORDS_GETTER("patientqueue.field.queueNumber"), true);
	// 付款方式
	API_DTO_FIELD_REQUIRE(String, paymentType, ZH_WORDS_GETTER("patientqueue.field.paymentType"), true);
};
