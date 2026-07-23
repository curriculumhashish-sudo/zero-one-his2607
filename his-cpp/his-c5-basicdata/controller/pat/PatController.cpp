#include "stdafx.h"
#include "PatController.h"


PatPageJsonVO::Wrapper PatController::execQueryPatPage(const PatPageQuery::Wrapper& query, const PayloadDTO& payload)
{
    // 创建分页载体，模板填 PatDTO::Wrapper
    auto pageData = PageDTO<PatDTO::Wrapper>::createShared();
    pageData->pageIndex = query->pageIndex;
    pageData->pageSize = query->pageSize;
    pageData->calcPages();

    // 外层标准响应
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
    // 定义返回数据对象
    auto jvo = ListJsonVO<String>::createShared();
    // 参数校验
    if (ids->empty())
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }

}