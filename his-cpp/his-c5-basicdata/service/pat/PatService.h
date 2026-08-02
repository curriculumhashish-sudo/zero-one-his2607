#pragma once
#include "../../domain/dto/pat/PatDTO.h"
#include <list>
#include <string>

class PatService
{
public:
    /**
     * 校验删除参数
     */
    std::string validateRemove(const oatpp::List<oatpp::String>& ids);

    bool removeData(const oatpp::List<oatpp::String>& ids);
};