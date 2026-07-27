#pragma once
#ifndef  _DRUGDETAILDTO_H_
#define  _DRUGDETAILDTO_H_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)
class DrugDetailDTO : public oatpp::DTO
{
	DTO_INIT(DrugDetailDTO, DTO);
	API_DTO_FIELD_DEFAULT(String, tongyongname, ZH_WORDS_GETTER("Drug.field.tongyongname"), true);
	API_DTO_FIELD_DEFAULT(String, id ,ZH_WORDS_GETTER("Drug.field.id"), true);
	//关键词
	API_DTO_FIELD_DEFAULT(String, name, ZH_WORDS_GETTER("Drug.field.shangpinname"), true);
	//名称
	API_DTO_FIELD_DEFAULT(String, kinds, ZH_WORDS_GETTER("Drug.field.kinds"), true);
	//分类
	API_DTO_FIELD_DEFAULT(String, changjia, ZH_WORDS_GETTER("Drug.field.changjia"),true);
	API_DTO_FIELD_DEFAULT(String, gongyingshang, ZH_WORDS_GETTER("Drug.field.gongyingshang"),true);
	API_DTO_FIELD_DEFAULT(String, wenhao, ZH_WORDS_GETTER("Drug.field.wenhao"), false);
	API_DTO_FIELD_DEFAULT(String, guojiabianma, ZH_WORDS_GETTER("Drug.field.guojiabianma"), false);
	API_DTO_FIELD_DEFAULT(String, yibaobianma, ZH_WORDS_GETTER("Drug.field.yibaobianma"), false);
	API_DTO_FIELD_DEFAULT(String,caiwuleixin, ZH_WORDS_GETTER("Drug.field.caiwuleixin"), false);
	API_DTO_FIELD_DEFAULT(String, yibaodengji, ZH_WORDS_GETTER("Drug.field.yibaodengji"), false);
	API_DTO_FIELD_DEFAULT(String, jiesuanleixin, ZH_WORDS_GETTER("Drug.field.jiesuanleixin"), false);
	API_DTO_FIELD_DEFAULT(String, yibaoduima, ZH_WORDS_GETTER("Drug.field.yibaoduima"), false);
	API_DTO_FIELD_DEFAULT(String, yibaoleibie, ZH_WORDS_GETTER("Drug.field.yibaoleibie"), false);
	API_DTO_FIELD_DEFAULT(String, xingzhi, ZH_WORDS_GETTER("Drug.field.xingzhi"), true);
	API_DTO_FIELD_DEFAULT(String, guige, ZH_WORDS_GETTER("Drug.field.guige"), true);
	API_DTO_FIELD_DEFAULT(String, danwei, ZH_WORDS_GETTER("Drug.field.danwei"), true);
	API_DTO_FIELD_DEFAULT(String, beishu, ZH_WORDS_GETTER("Drug.field.beishu"), false);
	API_DTO_FIELD_DEFAULT(String, jixin, ZH_WORDS_GETTER("Drug.field.jixin"), false);
	API_DTO_FIELD_DEFAULT(String, pishipanbie, ZH_WORDS_GETTER("Drug.field.pishipanbie"), false);
	API_DTO_FIELD_DEFAULT(String,zhusheyaopin, ZH_WORDS_GETTER("Drug.field.zhusheyaopin"), false);
	API_DTO_FIELD_DEFAULT(String,yongliangxianding, ZH_WORDS_GETTER("Drug.yongliangxianding"), false);
	API_DTO_FIELD_DEFAULT(String, yongfa, ZH_WORDS_GETTER("Drug.field.yongfa"), false);
	API_DTO_FIELD_DEFAULT(String, pingci, ZH_WORDS_GETTER("Drug.field.pingci"), false);
	API_DTO_FIELD_DEFAULT(String, dancijiliang, ZH_WORDS_GETTER("Drug.field.dancijiliang"), false);
	API_DTO_FIELD_DEFAULT(String, dancizuida, ZH_WORDS_GETTER("Drug.field.dancizuida"), false);
	API_DTO_FIELD_DEFAULT(String, yongfashuoming, ZH_WORDS_GETTER("Drug.field.yongfashuoming"), false);
	API_DTO_FIELD_DEFAULT(String, shiyongfanwei, ZH_WORDS_GETTER("Drug.field.shiyongfanwei"), false);
	API_DTO_FIELD_DEFAULT(String, yaopingshuoming, ZH_WORDS_GETTER("Drug.field.yaopingshuoming"), false);
	API_DTO_FIELD_DEFAULT(String, DDDzhi, ZH_WORDS_GETTER("Drug.field.DDD值"), false);
	API_DTO_FIELD_DEFAULT(String, DDDdanwei, ZH_WORDS_GETTER("Drug.DDDdanwei"), false);
	API_DTO_FIELD_DEFAULT(String, caigoudanwei, ZH_WORDS_GETTER("Drug.field.caigoudanwei"), true);
	API_DTO_FIELD_DEFAULT(String, caigoubeishu, ZH_WORDS_GETTER("Drug.field.keywords"), true);
	API_DTO_FIELD_DEFAULT(String, gourujia, ZH_WORDS_GETTER("Drug.field.gourujia"), true);
	API_DTO_FIELD_DEFAULT(String, menzhengshuxing, ZH_WORDS_GETTER("Drug.field.menzhengshuxin"),true);
	API_DTO_FIELD_DEFAULT(String, kucundanwei, ZH_WORDS_GETTER("Drug.field.kucundanwei"), true);
	API_DTO_FIELD_DEFAULT(String, kucunbeishu, ZH_WORDS_GETTER("Drug.field.kucunbeishu"), true);
	API_DTO_FIELD_DEFAULT(String,linghsoujia, ZH_WORDS_GETTER("Drug.field.lingshoujia"),false);
	API_DTO_FIELD_DEFAULT(String, linshishuxing, ZH_WORDS_GETTER("Drug.field.linshishuxin"), true);
	API_DTO_FIELD_DEFAULT(String, baozhuangbeishu, ZH_WORDS_GETTER("Drug.field.baozhuangbeishu"),true);
	API_DTO_FIELD_DEFAULT(String, baozhuangdanwei, ZH_WORDS_GETTER("Drug.field.baozhuangdanwei"), true);
	API_DTO_FIELD_DEFAULT(String, zuigaolinshoujia, ZH_WORDS_GETTER("Drug.field.zuigaolinshoujia"),false);
	API_DTO_FIELD_DEFAULT(String, chailingdanwei, ZH_WORDS_GETTER("Drug.field.chailingdanwei"), true);
	API_DTO_FIELD_DEFAULT(String, jinjiezuigao, ZH_WORDS_GETTER("Drug.field.jinjiezuigao"), false);
	API_DTO_FIELD_DEFAULT(String, jinjiezuidi, ZH_WORDS_GETTER("Drug.field.jinjiezuidi"), false);
	API_DTO_FIELD_DEFAULT(String,yaopingyinma, ZH_WORDS_GETTER("Drug.field.yaopingyinma"), false);
	API_DTO_FIELD_DEFAULT(String, shangpingyinma, ZH_WORDS_GETTER("Drug.field.shangpingyinma"), false);
	API_DTO_FIELD_DEFAULT(String, shangwubima, ZH_WORDS_GETTER("Drug.field.shangwubima"), false);
	API_DTO_FIELD_DEFAULT(String, liujiuma, ZH_WORDS_GETTER("Drug.field.liujiuma"), false);
	API_DTO_FIELD_DEFAULT(String, yaopingstatus, ZH_WORDS_GETTER("Drug.field.yaopingstatus"), false);
	API_DTO_FIELD_DEFAULT(String, lingyu, ZH_WORDS_GETTER("Drug.field.lingyu"), false);



	

};
#include OATPP_CODEGEN_END(DTO);
#endif // !
