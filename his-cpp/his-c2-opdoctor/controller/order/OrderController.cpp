#include "stdafx.h"
#include "OrderController.h"


OrderDetailPageJsonVO::Wrapper OrderController::execQueryOrder(const OrderQuery::Wrapper& query)
{
	return {};
}

StringJsonVO::Wrapper OrderController::execAddOrder(const OrderAddDTO::Wrapper& dto, const PayloadDTO& patload)
{
	return {};
}

StringJsonVO::Wrapper OrderController::execModifySample(const OrderModifyDTO::Wrapper& dto, const PayloadDTO& patload)
{
	return {};
}

CatalogItemPageJsonVO::Wrapper OrderController::execQueryCatalog(const CatalogQuery::Wrapper& query)
{
	return {};
}
