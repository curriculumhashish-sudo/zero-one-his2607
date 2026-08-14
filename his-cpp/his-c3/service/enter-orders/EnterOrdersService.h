#pragma once
#ifndef _ENTERORDERSSERVICE_H
#define _ENTERORDERSSERVICE_H
//#include "domain/vo/sample/SampleVO.h"
//#include "domain/query/sample/SampleQuery.h"
#include "../../domain/query/enter-orders/EnterOrdersQuery.h"
//#include "domain/dto/sample/SampleDTO.h"
#include "../../domain/dto/enter-orders/EnterOrdersDTO.h"

//EnterOrders
//enter-orders
//ENTERORDERS

/**
 * 示例服务实现，演示基础的示例服务实现
 */
class EnterOrdersService
{
public:
	// 分页查询所有数据
	EnterOrdersQueryPageDTO::Wrapper select(const EnterOrdersQueryById::Wrapper& query);


	// 批量保存数据
	bool batchAddTwoTable(const oatpp::List<EnterOrdersAddDTO::Wrapper>& mainDtoList,const oatpp::List<EnterOrdersAddDTO1::Wrapper>& subDtoList);
	// 修改数据
	bool updateData(const EnterOrdersModifyDTO::Wrapper& dto);
	// 通过ID删除数据
	bool removeData(const oatpp::List<oatpp::String>& ids);
};

#endif // !_SAMPLE_SERVICE_
