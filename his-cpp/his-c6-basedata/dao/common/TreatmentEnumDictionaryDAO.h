#pragma once
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
#ifndef _TREATMENT_ENUM_DICTIONARY_DAO_H_
#define _TREATMENT_ENUM_DICTIONARY_DAO_H_

#include <map>
#include <string>
#include <vector>

/** Accesses treatment enum dictionaries in MongoDB. */
class TreatmentEnumDictionaryDAO
{
public:
    static constexpr const char* COLLECTION_NAME = "enum_dictionary";
    static constexpr const char* PUBLICATION_STATUS_ENUM = "PublicationStatus";

    /** Returns active code-to-display mappings. */
    std::map<std::string, std::string> findActiveDisplays(
        const std::string& codeSystemName,
        const std::vector<std::string>& codes) const;
};

#endif // !_TREATMENT_ENUM_DICTIONARY_DAO_H_
