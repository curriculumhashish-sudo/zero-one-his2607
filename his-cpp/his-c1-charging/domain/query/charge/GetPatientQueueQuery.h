#pragma once

#ifndef _GET_PATIENT_QUEUE_QUERY_
#define _GET_PATIENT_QUEUE_QUERY_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)
/*
	获取患者队列分页查询对象
*/
class GetPatientQueueQuery : public PageQuery {
	DTO_INIT(GetPatientQueueQuery, PageQuery);

	//关键字:姓名/就诊号
	API_DTO_FIELD(String, keyword,
		ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueQuery.keyword.description"),
		false, "");

	//开始日期
	API_DTO_FIELD(String, startDate,
		ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueQuery.startDate.description"),
		false, "");

	//结束日期
	API_DTO_FIELD(String, endDate,
		ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueQuery.endDate.description"),
		false, "");

	//收费状态
	API_DTO_FIELD(String, queueTabCode,
		ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueQuery.queueTabCode.description"),
		true,"PENDING");

	//科室/机构 ID 筛选（可选，为空查全部）
	API_DTO_FIELD(String, organizationId,
		ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueQuery.organizationId.description"),
		false, "");
};


#include OATPP_CODEGEN_END(DTO)

#endif //!_GET_PATIENT_QUEUE_QUERY_
