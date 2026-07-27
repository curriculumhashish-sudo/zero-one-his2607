/*
 Copyright Zero One Star. All rights reserved.
 @Author: sqdxy
 @Date: 2026/07/20
*/
#include "stdafx.h"
#include "PrintJobService.h"

PrintJobPageDTO::Wrapper PrintJobService::listAll(const PrintJobQuery::Wrapper& query)
{
	auto pages = PrintJobPageDTO::createShared();
	pages->pageIndex = query->pageIndex;
	pages->pageSize = query->pageSize;

	PrintJobDAO dao;
	uint64_t count = dao.count(query);
	if (count <= 0)
	{
		return pages;
	}

	pages->total = count;
	pages->calcPages();
	list<PrintJobDO> result = dao.selectWithPage(query);

	for (PrintJobDO& sub : result)
	{
		auto dto = PrintJobDTO::createShared();
		ZO_STAR_DOMAIN_DO_TO_DTO(dto, sub,
			docType, DocType,
			name, PatientName,
			item, TreatmentItem,
			dept, ExecDept,
			doctor, DoctorName,
			otime, OrderTime,
			etime, ExecTime,
			executor, Executor,
			status, Status,
			printCount, PrintCount,
			remark, Remark,
			id, Id
		);
		pages->addData(dto);
	}
	return pages;
}
