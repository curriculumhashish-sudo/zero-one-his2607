#include "stdafx.h"
#include "Patcontroller.h"


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
    auto pageData = PageDTO<PatSelectDTO::Wrapper>::createShared();
    pageData->pageIndex = query->pageIndex;
    pageData->pageSize = query->pageSize;
    pageData->calcPages();

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