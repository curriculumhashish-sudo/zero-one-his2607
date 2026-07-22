#pragma once
#ifndef _HISTORY_ORDER_LIST_VO_H_
#define _HISTORY_ORDER_LIST_VO_H_

#include "../../dto/prescription4/historyOrderListDTO.h"	// 引入历史就诊记录分页DTO

#include OATPP_CODEGEN_BEGIN(DTO)	// oatpp DTO 代码生成开始宏

/**
 * 历史医嘱就诊记录分页查询返回VO
 * 封装统一Json返回格式，返回给前端
 */
class HistoryOrderListPageJsonVO : public JsonVO<HistoryOrderListPageDTO::Wrapper> {
	DTO_INIT(HistoryOrderListPageJsonVO, JsonVO<HistoryOrderListPageDTO::Wrapper>); // DTO初始化，声明父子类关系
};

#include OATPP_CODEGEN_END(DTO)	// oatpp DTO 代码生成结束宏
#endif // _HISTORY_ORDER_LIST_VO_H_	// 头文件保护宏结束
