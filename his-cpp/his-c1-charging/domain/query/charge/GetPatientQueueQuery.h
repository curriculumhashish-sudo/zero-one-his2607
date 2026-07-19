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
	DTO_FIELD(String, keyword);
	DTO_FIELD_INFO(keyword)
	{
		info->description = ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueQuery.keyword");
	}

	//开始日期
	DTO_FIELD(String, startDate);
	DTO_FIELD_INFO(startDate)
	{
		info->description = ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueQuery.startDate");
	}

	//结束日期
	DTO_FIELD(String, endDate);
	DTO_FIELD_INFO(endDate)
	{
		info->description = ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueQuery.endDate");
	}

	//收费状态
	DTO_FIELD(String, queueTabCode);
	DTO_FIELD_INFO(queueTabCode)
	{
		info->description = ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueueQuery.queueTabCode");
	}
};


#include OATPP_CODEGEN_END(DTO)

#endif //!_GET_PATIENT_QUEUE_QUERY_