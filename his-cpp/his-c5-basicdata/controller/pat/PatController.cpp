#include "stdafx.h"
#include "PatController.h"

ListJsonVO<String>::Wrapper PatController::execRemovePat(const List<String>& ids)
{
	// 定义返回数据对象
	auto jvo = ListJsonVO<String>::createShared();
	// 参数校验
	if (ids->empty())
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

}