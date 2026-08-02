#include "stdafx.h"
#include "PatService.h"
#include "../../dao/pat/PatDAO.h"

std::string PatService::validateRemove(const oatpp::List<oatpp::String>&ids)
{
    if (!ids || ids->empty())
    {
        return "ids empty.";
    }
    
    for (auto one : *ids.get())
    {
        if (!one || one->empty())
        {
            return "one of ids is empty.";
        }
    }
    return "";
}

bool PatService::removeData(const oatpp::List<oatpp::String>& ids)
{
    std::list<std::string> datas;
    for (auto one : *ids.get())
    {
        datas.emplace_back(one.getValue({}));
    }

    // 调用 DAO 执行删除
    PatDAO dao;
    int rows = dao.deleteByIds(datas);

    // 如果影响行数等于传入数量则认为删除成功
    return rows == (int)ids->size();
}