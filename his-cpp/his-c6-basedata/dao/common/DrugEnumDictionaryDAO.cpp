/*
 Copyright Zero One Star. All rights reserved.

 @Author: axiao
 @Date: 2026/08/01

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#include "stdafx.h"
#include "DrugEnumDictionaryDAO.h"

#include "Macros.h"
#include "MongoClient.h"
#include "NacosClient.h"
#include "ServerInfo.h"

#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <stdexcept>

std::map<std::string, std::string> DrugEnumDictionaryDAO::findActiveDisplays(
    const std::string& codeSystemName,
    const std::vector<std::string>& codes) const
{
    std::map<std::string, std::string> result;
    if (codeSystemName.empty() || codes.empty())
    {
        return result;
    }

    bsoncxx::builder::basic::array codeArray;
    for (const auto& code : codes)
    {
        codeArray.append(code);
    }
    using bsoncxx::builder::basic::kvp;
    using bsoncxx::builder::basic::make_document;
    const auto filter = make_document(
        kvp("codeSystemName", codeSystemName),
        kvp("code", make_document(kvp("$in", codeArray.extract()))),
        kvp("isActive", true));

    ZO_CREATE_MONGO_CLIENT(client);
    const bool executed = client.execute(
        COLLECTION_NAME,
        [&](mongocxx::collection* collection)
        {
            auto cursor = collection->find(filter.view());
            for (const auto& item : cursor)
            {
                const auto codeElement = item["code"];
                const auto displayElement = item["display"];
                if (!codeElement || !displayElement ||
                    codeElement.type() != bsoncxx::type::k_string ||
                    displayElement.type() != bsoncxx::type::k_string)
                {
                    continue;
                }
                const auto codeValue = codeElement.get_value().get_string().value;
                const auto displayValue = displayElement.get_value().get_string().value;
                result.emplace(
                    std::string(codeValue.data(), codeValue.size()),
                    std::string(displayValue.data(), displayValue.size()));
            }
        });
    if (!executed)
    {
        throw std::runtime_error("failed to query enum dictionary");
    }
    return result;
}
