#pragma once
#ifndef _HISTORY_ORDER_DETAIL_VO_H_
#define _HISTORY_ORDER_DETAIL_VO_H_

#include "../../dto/historyOrder/historyOrderDetailDTO.h"	// 引入历史医嘱明细分页DTO

#include OATPP_CODEGEN_BEGIN(DTO)	// oatpp DTO 代码生成开始宏

/**
 * 历史医嘱明细分页查询返回VO
 * 封装统一Json返回格式，返回给前端
 */
class HistoryOrderDetailPageJsonVO : public JsonVO<HistoryOrderDetailPageDTO::Wrapper> {
	DTO_INIT(HistoryOrderDetailPageJsonVO, JsonVO<HistoryOrderDetailPageDTO::Wrapper>); // DTO初始化，声明父子类关系
};

#include OATPP_CODEGEN_END(DTO)	// oatpp DTO 代码生成结束宏
#endif // _HISTORY_ORDER_DETAIL_VO_H_	// 头文件保护宏结束
