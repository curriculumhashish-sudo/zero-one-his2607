#include "stdafx.h"
#include"ChinapresController1.h"
#include"service/chinapres1/ChinapresService1.h"

ChinapresPageJsonVO1::Wrapper ChinapresController1::executeQueryChinapres(const ChinapresQuery1::Wrapper& query)
{
	ChinapresService1 service;
	auto jvo = ChinapresPageJsonVO1::createShared();
	auto page = service.queryList(query);
	if (page) {
		jvo->success(page);
	}
	else {
		jvo->fail({});
	}
	return jvo;
}

ChinapresDetailJsonVO1::Wrapper ChinapresController1::executeQueryChinapresDetail(const ChinapresDetailQuery1::Wrapper& query)
{
	ChinapresService1 service;
	auto jvo = ChinapresDetailJsonVO1::createShared();
	auto detail = service.queryDetail(query);
	if (detail) {
		jvo->success(detail);
	}
	else {
		jvo->fail({});
	}
	return jvo;
}
