#include "stdafx.h"
#include "PatController.h"
#include "../../service/pat/PatService.h"


PatPageJsonVO::Wrapper PatController::execQueryPatPage(const PatPageQuery::Wrapper& query, const PayloadDTO& payload)
{
  
    auto pageData = PageDTO<PatDTO::Wrapper>::createShared();
    pageData->pageIndex = query->pageIndex;
    pageData->pageSize = query->pageSize;
    pageData->calcPages();

  
    auto respVo = PatPageJsonVO::createShared();
    respVo->success(pageData);
    respVo->message = ZH_WORDS_GETTER("pat.page.success");
    return respVo;
}

PatSelectPageJsonVO::Wrapper PatController::execQuerySelectPatPage(const PatSelectPageQuery::Wrapper& query, const PayloadDTO& payload)
{
    // 入参校验，设置默认值
    if (!query->pageIndex || query->pageIndex < 1)
    {
        query->pageIndex = 1;
    }
    if (!query->pageSize || query->pageSize < 1)
    {
        query->pageSize = 20;
    }

    // 调用Service层
    PatService patService;
    auto pageData = patService.querySelectPatPage(query);

    // 组装返回VO
    auto respVo = PatSelectPageJsonVO::createShared();
    respVo->success(pageData);
    respVo->message = ZH_WORDS_GETTER("pat.select.page.success");
    return respVo;
}

ListJsonVO<String>::Wrapper PatController::execRemovePat(const List<String>& ids)
{
   
    auto jvo = ListJsonVO<String>::createShared();
    
    if (ids->empty())
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }

    return jvo;
}