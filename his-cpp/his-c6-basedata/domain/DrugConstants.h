#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: axiao
 @Date: 2026/07/28

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
#ifndef _DRUG_CONSTANTS_H_
#define _DRUG_CONSTANTS_H_

#include <cstddef>

/**
 * Stable drug domain and API codes shared by DTO, Service and DAO.
 */
class DrugConstants final
{
  public:
    inline static constexpr std::size_t MAX_BATCH_SIZE = 100;

    inline static constexpr char STATUS_ACTIVE[] = "ACTIVE";
    inline static constexpr char STATUS_INACTIVE[] = "INACTIVE";
    inline static constexpr char STATUS_DISCARDED[] = "DISCARDED";

    inline static constexpr char API_STATUS_ACTIVE[] = "1";
    inline static constexpr char API_STATUS_INACTIVE[] = "2";
    inline static constexpr char API_STATUS_DISCARDED[] = "3";

    inline static constexpr char PRICE_TYPE_PURCHASE[] = "PURCHASE";
    inline static constexpr char PRICE_TYPE_SALE[] = "SALE";
    inline static constexpr char PRICE_TYPE_LIMIT[] = "LIMIT";

    inline static constexpr char UNIT_BAND_SPLIT[] = "SPLIT";
    inline static constexpr char UNIT_BAND_DOSE[] = "DOSE";
    inline static constexpr char UNIT_BAND_PURCHASE[] = "PURCHASE";
    inline static constexpr char UNIT_BAND_STOCK[] = "STOCK";
    inline static constexpr char UNIT_BAND_PACKAGE[] = "PACKAGE";

    inline static constexpr double SPLIT_UNIT_RATIO = 1.0;

  private:
    DrugConstants() = delete;
};

#endif // !_DRUG_CONSTANTS_H_
