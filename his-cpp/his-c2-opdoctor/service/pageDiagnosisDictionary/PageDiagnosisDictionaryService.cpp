#include "PageDiagnosisDictionaryService.h"
#include "stdafx.h"
#include <unordered_map>
#include <list>

PageDiagnosisDictionaryPageDTO::Wrapper PageDiagnosisDictionaryService::getPageDiagnosisDictionaryList(
    const PageDiagnosisDictionaryQuery::Wrapper& query,
    const std::string& tenantId,
    const std::string& practitionerId)
{
    std::unordered_map<std::string, std::string> typeCode = {
        {"1",u8"西医诊断"},
        {"2",u8"症状诊断"},
        {"3",u8"中医诊断"},
    };

    auto dataList = dao.queryDiagnosisDictionary(query, tenantId, practitionerId);

    // 创建分页 DTO
    auto pageDto = PageDiagnosisDictionaryPageDTO::createShared();
    pageDto->pageIndex = query->pageIndex;
    pageDto->pageSize = query->pageSize;
    pageDto->total = (v_int64)dataList.size();

    // rows 使用 oatpp::List
    auto rows = oatpp::List<PageDiagnosisDictionaryDTO::Wrapper>::createShared();

    for (auto& itemDO : dataList)
    {
        auto dto = PageDiagnosisDictionaryDTO::createShared();

        dto->id = itemDO->getId();
        dto->code = itemDO->getConditionCode();

        auto it = typeCode.find(itemDO->getTypeCode());
        if (it != typeCode.end())
        {
            dto->typeName = it->second;
        }
        else
        {
            dto->typeName = itemDO->getTypeCode();
        }

        dto->name = itemDO->getName();

        rows->push_back(dto);
    }

    pageDto->rows = rows;

    // 如果 PageDTO 有 calcPages()，调用它以计算 pages 字段
    // (该方法在项目其它代码里有使用，保留以兼容)
    try
    {
        pageDto->calcPages();
    }
    catch (...)
    {
        cerr << "Error in PageDTO::calcPages()" << endl;
    }

    return pageDto;
}
