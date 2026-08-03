#pragma once
#ifndef  _DRUGDETAILDTO_H_
#define  _DRUGDETAILDTO_H_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)
class DrugDetailDTO : public oatpp::DTO
{
	DTO_INIT(DrugDetailDTO, DTO);
	API_DTO_FIELD_REQUIRE(String, tongyongname, ZH_WORDS_GETTER("Drug.field.tongyongname"), true);
	API_DTO_FIELD_REQUIRE(String, id ,ZH_WORDS_GETTER("Drug.field.id"), true);
	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("Drug.field.shangpinname"), true);
	//名称
	API_DTO_FIELD_REQUIRE(String, kinds, ZH_WORDS_GETTER("Drug.field.kinds"), true);
	//分类
	API_DTO_FIELD_REQUIRE(String, changjia, ZH_WORDS_GETTER("Drug.field.changjia"), true);
	API_DTO_FIELD_REQUIRE(String, gongyingshang, ZH_WORDS_GETTER("Drug.field.gongyingshang"), true);
	API_DTO_FIELD_REQUIRE(String, wenhao, ZH_WORDS_GETTER("Drug.field.wenhao"), false);
	API_DTO_FIELD_REQUIRE(String, guojiabianma, ZH_WORDS_GETTER("Drug.field.guojiabianma"), false);
	API_DTO_FIELD_REQUIRE(String, yibaobianma, ZH_WORDS_GETTER("Drug.field.yibaobianma"), false);
	API_DTO_FIELD_REQUIRE(Int32, caiwuleixin, ZH_WORDS_GETTER("Drug.field.caiwuleixin"), false);
	API_DTO_FIELD_REQUIRE(String, yibaodengji, ZH_WORDS_GETTER("Drug.field.yibaodengji"), false);
	API_DTO_FIELD_REQUIRE(String, jiesuanleixin, ZH_WORDS_GETTER("Drug.field.jiesuanleixin"), false);
	API_DTO_FIELD_REQUIRE(Boolean, yibaoduima, ZH_WORDS_GETTER("Drug.field.yibaoduima"), false);
	API_DTO_FIELD_REQUIRE(String, yibaoleibie, ZH_WORDS_GETTER("Drug.field.yibaoleibie"), false);
	API_DTO_FIELD_REQUIRE(Int32, xingzhi, ZH_WORDS_GETTER("Drug.field.xingzhi"), true);
	API_DTO_FIELD_REQUIRE(String, guige, ZH_WORDS_GETTER("Drug.field.guige"), true);
	API_DTO_FIELD_REQUIRE(String, danwei, ZH_WORDS_GETTER("Drug.field.danwei"), true);
	API_DTO_FIELD_REQUIRE(Float64, beishu, ZH_WORDS_GETTER("Drug.field.beishu"), false);
	API_DTO_FIELD_REQUIRE(String, jixin, ZH_WORDS_GETTER("Drug.field.jixin"), false);
	API_DTO_FIELD_REQUIRE(Boolean, pishipanbie, ZH_WORDS_GETTER("Drug.field.pishipanbie"), false);
	API_DTO_FIELD_REQUIRE(Boolean, zhusheyaopin, ZH_WORDS_GETTER("Drug.field.zhusheyaopin"), false);
	API_DTO_FIELD_REQUIRE(Float64, yongliangxianding, ZH_WORDS_GETTER("Drug.yongliangxianding"), false);
	API_DTO_FIELD_REQUIRE(String, yongfa, ZH_WORDS_GETTER("Drug.field.yongfa"), false);
	API_DTO_FIELD_REQUIRE(String, pingci, ZH_WORDS_GETTER("Drug.field.pingci"), false);
	API_DTO_FIELD_REQUIRE(Float64, dancijiliang, ZH_WORDS_GETTER("Drug.field.dancijiliang"), false);
	API_DTO_FIELD_REQUIRE(Float64, dancizuida, ZH_WORDS_GETTER("Drug.field.dancizuida"), false);
	API_DTO_FIELD_REQUIRE(String, yongfashuoming, ZH_WORDS_GETTER("Drug.field.yongfashuoming"), false);
	API_DTO_FIELD_REQUIRE(String, shiyongfanwei, ZH_WORDS_GETTER("Drug.field.shiyongfanwei"), false);
	API_DTO_FIELD_REQUIRE(String, yaopingshuoming, ZH_WORDS_GETTER("Drug.field.yaopingshuoming"), false);
	API_DTO_FIELD_REQUIRE(String, DDDzhi, ZH_WORDS_GETTER("Drug.field.DDDzhi"), false);
	API_DTO_FIELD_REQUIRE(String, DDDdanwei, ZH_WORDS_GETTER("Drug.DDDdanwei"), false);
	API_DTO_FIELD_REQUIRE(String, caigoudanwei, ZH_WORDS_GETTER("Drug.field.caigoudanwei"), true);
	API_DTO_FIELD_REQUIRE(Float64, caigoubeishu, ZH_WORDS_GETTER("Drug.field.keywords"), true);
	API_DTO_FIELD_REQUIRE(Float64, gourujia, ZH_WORDS_GETTER("Drug.field.gourujia"), true);
	API_DTO_FIELD_REQUIRE(String, menzhengshuxing, ZH_WORDS_GETTER("Drug.field.menzhengshuxin"), true);
	API_DTO_FIELD_REQUIRE(String, kucundanwei, ZH_WORDS_GETTER("Drug.field.kucundanwei"), true);
	API_DTO_FIELD_REQUIRE(Float64, kucunbeishu, ZH_WORDS_GETTER("Drug.field.kucunbeishu"), true);
	API_DTO_FIELD_REQUIRE(String, linghsoujia, ZH_WORDS_GETTER("Drug.field.lingshoujia"), false);
	API_DTO_FIELD_REQUIRE(String, linshishuxing, ZH_WORDS_GETTER("Drug.field.linshishuxin"), true);
	API_DTO_FIELD_REQUIRE(Float64, baozhuangbeishu, ZH_WORDS_GETTER("Drug.field.baozhuangbeishu"), true);
	API_DTO_FIELD_REQUIRE(String, baozhuangdanwei, ZH_WORDS_GETTER("Drug.field.baozhuangdanwei"), true);
	API_DTO_FIELD_REQUIRE(Float64, zuigaolinshoujia, ZH_WORDS_GETTER("Drug.field.zuigaolinshoujia"), false);
	API_DTO_FIELD_REQUIRE(String, chailingdanwei, ZH_WORDS_GETTER("Drug.field.chailingdanwei"), true);
	API_DTO_FIELD_REQUIRE(String, jinjiezuigao, ZH_WORDS_GETTER("Drug.field.jinjiezuigao"), false);
	API_DTO_FIELD_REQUIRE(String, jinjiezuidi, ZH_WORDS_GETTER("Drug.field.jinjiezuidi"), false);
	API_DTO_FIELD_REQUIRE(String, yaopingyinma, ZH_WORDS_GETTER("Drug.field.yaopingyinma"), false);
	API_DTO_FIELD_REQUIRE(String, shangpingyinma, ZH_WORDS_GETTER("Drug.field.shangpingyinma"), false);
	API_DTO_FIELD_REQUIRE(String, shangwubima, ZH_WORDS_GETTER("Drug.field.shangwubima"), false);
	API_DTO_FIELD_REQUIRE(String, liujiuma, ZH_WORDS_GETTER("Drug.field.liujiuma"), false);
	API_DTO_FIELD_REQUIRE(String, yaopingstatus, ZH_WORDS_GETTER("Drug.field.yaopingstatus"), false);
	API_DTO_FIELD_REQUIRE(String, lingyu, ZH_WORDS_GETTER("Drug.field.lingyu"), false);
	API_DTO_FIELD_REQUIRE(Boolean, selfFlag, ZH_WORDS_GETTER("Drug.field.selfFlag"), false);
	API_DTO_FIELD_REQUIRE(Boolean, antibioticFlag, ZH_WORDS_GETTER("Drug.field.antibioticFlag"), false);
	API_DTO_FIELD_REQUIRE(Boolean, basicFlag, ZH_WORDS_GETTER("Drug.field.basicFlag"), false);
	API_DTO_FIELD_REQUIRE(Boolean, activeFlag, ZH_WORDS_GETTER("Drug.field.activeFlag"), false);
	API_DTO_FIELD_REQUIRE(Boolean, childrenFlag, ZH_WORDS_GETTER("Drug.field.childrenFlag"), false);
	API_DTO_FIELD_REQUIRE(Boolean, restrictedFlag, ZH_WORDS_GETTER("Drug.field.restrictedFlag"), false);
	API_DTO_FIELD_REQUIRE(Boolean, rxFlag, ZH_WORDS_GETTER("Drug.field.rxFlag"), false);


	

};
#include OATPP_CODEGEN_END(DTO);
#endif // !
