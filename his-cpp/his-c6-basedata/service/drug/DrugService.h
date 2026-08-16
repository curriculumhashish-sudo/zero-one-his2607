#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: axiao
 @Date: 2026/07/23

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
#ifndef _DRUG_SERVICE_H_
#define _DRUG_SERVICE_H_
#include "stdafx.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"
#include "domain/dto/drug/MedicineV2DTO.h"
#include "domain/query/Drug/DrugQuery.h"
#include "domain/dto/drug/DrugDetailDTO.h"
#include "domain/dto/drug/DrugListDTO.h"
#include "domain/dto/drug/DrugAddDTO.h"
#include "domain/dto/drug/DrugSelectDTO.h"
#include  "domain/query/Drug/DrugSelectQuery.h"
#include <stdexcept>

// 药品写操作可能返回的业务异常类型。
enum class DrugServiceError
{
    InvalidInput,
    NotFound,
    InvalidStatus,
    Referenced,
    CrossTenant,
    ConcurrentModification,
    DatabaseFailure
};

// 在事务边界传递可识别的药品业务异常。
class DrugServiceException : public std::runtime_error
{
  private:
    DrugServiceError error;

  public:
    explicit DrugServiceException(DrugServiceError errorCode);
    DrugServiceError getError() const noexcept;
};

/** 药品业务服务。 */
class DrugService
{
  public:
    // 按乐观锁版本部分修改药品及其价格。
    MedicineVersionItemDTO::Wrapper updateMedicineV2(const std::string &id,
                                                      const MedicineUpdateDTO::Wrapper &dto);
    // 按乐观锁版本批量启用或停用药品。
    MedicineBatchResultDTO::Wrapper updateMedicineStatusV2(const MedicineStatusUpdateDTO::Wrapper &dto);
    // 检查业务引用后按版本批量软删除药品。
    MedicineBatchResultDTO::Wrapper batchDeleteMedicinesV2(const MedicineBatchDeleteDTO::Wrapper &dto);
    DrugListPageDTO::Wrapper listAll(const DrugQuery::Wrapper& query);
    DrugDetailDTO::Wrapper getById(string id);
    string saveData(const DrugAddDTO::Wrapper& dto);
    DrugSelectPageDTO::Wrapper SelectlistAll(const DrugSelectQuery::Wrapper& query);
};

#endif // !_DRUG_SERVICE_H_
