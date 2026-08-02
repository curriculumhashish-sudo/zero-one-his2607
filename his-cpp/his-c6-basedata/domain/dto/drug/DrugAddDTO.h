#pragma once
#ifndef  _DRUGADDDTO_H_
#define  _DRUGADDDTO_H_
#include "DrugDetailDTO.h"
#include OATPP_CODEGEN_BEGIN(DTO)
class DrugAddDTO : public DrugDetailDTO
{
	DTO_INIT(DrugAddDTO, DrugDetailDTO);
	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
public:
	// 杅擂苺桄
	std::string validate()
	{
		if (!name || name->empty())
			return "name invalidate.";
		if (!tongyongname || tongyongname->empty())
			return "tongyongname invalidate.";
		if (!kinds || kinds->empty())
			return "kinds invalidate.";
		if (!changjia || changjia->empty())
			return "changjia invalidate.";
		if (!gongyingshang || gongyingshang->empty())
			return "gongyingshang invalidate.";
		if (!caiwuleixin || caiwuleixin->empty())
			return "caiwuleixin invalidate.";
		if (!xingzhi || xingzhi->empty())
			return "xingzhi invalidate.";
		if (!guige || guige->empty())
			return "guige invalidate.";
		if (!danwei || danwei->empty())
			return "danwei invalidate.";
		if (!beishu || beishu->empty())
			return "beishu invalidate.";
		if (!jixin || jixin->empty())
			return "jixin invalidate.";
		if (!caigoudanwei || caigoudanwei->empty())
			return " caigoudanwei invalidate.";
		if (!caigoubeishu || caigoubeishu->empty())
			return " caigoubeishu invalidate.";
		if (!gourujia || gourujia->empty())
			return "gourujia invalidate.";
		if (!menzhengshuxing || menzhengshuxing->empty())
			return "menzhengshuxing invalidate.";
		if (!kucundanwei || kucundanwei->empty())
			return "kucundanwei, invalidate.";
		if (!kucunbeishu || kucunbeishu->empty())
			return " kucunbeishu invalidate.";
		if (!linghsoujia || linghsoujia->empty())
			return "linghsoujia invalidate.";
		if (!linshishuxing || linshishuxing->empty())
			return "linshishuxing invalidate.";
		if (!baozhuangdanwei || baozhuangdanwei->empty())
			return "baozhuangdanwei invalidate.";
		if (!baozhuangbeishu || baozhuangbeishu->empty())
			return "baozhuangbeishu invalidate.";
		if (!baozhuangbeishu || baozhuangbeishu->empty())
			return "baozhuangbeishu invalidate.";
		if (!chailingdanwei || chailingdanwei->empty())
			return "chailingdanwei invalidate.";
		return "";

	};

};

#include OATPP_CODEGEN_END(DTO);
#endif // !
