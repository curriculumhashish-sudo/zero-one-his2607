/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/25 00:00:00

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
#include "MedicalOrderVerificationService.h"
#include "dao/medicalorderverification/MedicalOrderVerificationDAO.h"

oatpp::List<MedicalOrderVerificationDetailDTO::Wrapper> MedicalOrderVerificationService::list(const MedicalVerificationQuery::Wrapper& q){auto out=oatpp::List<MedicalOrderVerificationDetailDTO::Wrapper>::createShared();for(auto& x:MedicalOrderVerificationDAO().selectItems(q))out->push_back(x);return out;}
VerifyPassResponseDTO::Wrapper MedicalOrderVerificationService::verify(const VerifyPassRequestDTO::Wrapper& dto,const std::string& op){auto out=VerifyPassResponseDTO::createShared();out->processedCount=0;out->success=false;if(!dto||!dto->items||dto->items->empty())return out;MedicalOrderVerificationDAO dao;dao.getSqlSession()->beginTransaction();try{for(auto& i:*dto->items){if(!i||!i->requestId||!i->orderType||dao.verify(i->orderType,i->requestId,op)!=1)throw std::runtime_error("order status changed");out->processedCount=out->processedCount+1;}dao.getSqlSession()->commitTransaction();out->success=true;}catch(...){dao.getSqlSession()->rollbackTransaction();out->processedCount=0;}return out;}
RejectOrdersResponseDTO::Wrapper MedicalOrderVerificationService::reject(const RejectOrdersRequestDTO::Wrapper& dto,const std::string& op){auto out=RejectOrdersResponseDTO::createShared();out->processedCount=0;out->success=false;if(!dto||!dto->items||dto->items->empty()||!dto->reason||dto->reason->empty())return out;MedicalOrderVerificationDAO dao;dao.getSqlSession()->beginTransaction();try{for(auto& i:*dto->items){if(!i||dao.reject(i->orderType,i->requestId,dto->reason,op)!=1)throw std::runtime_error("order status changed");out->processedCount=out->processedCount+1;}dao.getSqlSession()->commitTransaction();out->success=true;}catch(...){dao.getSqlSession()->rollbackTransaction();out->processedCount=0;}return out;}
