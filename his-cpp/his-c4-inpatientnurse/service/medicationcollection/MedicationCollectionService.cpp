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
#include "MedicationCollectionService.h"
#include "dao/medicationcollection/MedicationCollectionDAO.h"
#include "id/UuidFacade.h"
PagePendingDispenseSummaryLinesDTO::Wrapper MedicationCollectionService::listPending(const PagePendingDispenseSummaryLinesQuery::Wrapper&q){auto page=PagePendingDispenseSummaryLinesDTO::createShared();page->rows=oatpp::List<NurseStationDispenseSummaryPatientGroupDTO::Wrapper>::createShared();page->patientGroups=oatpp::List<NurseStationDispenseSummaryPatientGroupDTO::Wrapper>::createShared();page->pageIndex=q->pageIndex;page->pageSize=q->pageSize;MedicationCollectionDAO dao;page->total=dao.countPending(q);page->calcPages();std::map<std::string,NurseStationDispenseSummaryPatientGroupDTO::Wrapper> patients;std::map<std::string,NurseStationDispenseSummaryOrderRowDTO::Wrapper> orders;for(auto&r:dao.selectPending(q)){auto&g=patients[r.encounterId];if(!g){g=NurseStationDispenseSummaryPatientGroupDTO::createShared();g->encounterId=r.encounterId;g->patientId=r.patientId;g->patientName=r.patientName;g->bedDisplay=r.bedName;g->orderRows=oatpp::List<NurseStationDispenseSummaryOrderRowDTO::Wrapper>::createShared();page->patientGroups->push_back(g);page->rows->push_back(g);}auto key=r.encounterId+":"+r.medReqId;auto&o=orders[key];if(!o){o=NurseStationDispenseSummaryOrderRowDTO::createShared();o->medReqId=r.medReqId;o->medicationId=r.medicationId;o->medicationName=r.medicationName;o->specificationText=r.specification;o->groupId=r.groupId;o->therapyEnum=r.therapy;o->therapyEnumText=r.therapy=="LONG_TERM"?"长期":"临时";o->dosageInstruction=r.dosage;o->methodCode=r.method;o->methodCodeText=r.method;o->frequencyCode=r.frequency;o->frequencyCodeText=r.frequency;o->locationId=r.locationId;o->locationName=r.locationName;o->timePoints=oatpp::List<NurseStationDispenseSummaryTimePointDTO::Wrapper>::createShared();g->orderRows->push_back(o);}auto t=NurseStationDispenseSummaryTimePointDTO::createShared();t->dispenseId=r.dispenseId;t->procedureId=r.procedureId;t->plannedDispenseTime=r.plannedTime;t->quantity=r.quantity;t->unitCode=r.unitCode;t->unitCodeText=r.unitCode;t->chargeUnitPrice=r.price;o->timePoints->push_back(t);}return page;}
CreateMedicationSummaryResponseDTO::Wrapper MedicationCollectionService::summarize(const CreateMedicationSummaryDTO::Wrapper&dto,const std::string&op){auto out=CreateMedicationSummaryResponseDTO::createShared();out->summaryNos=oatpp::List<oatpp::String>::createShared();out->message="汇总失败";if(!dto||!dto->dispenseItems||dto->dispenseItems->empty()||dto->dispenseItems->size()>500)return out;std::vector<std::string> ids;for(auto&i:*dto->dispenseItems)if(i&&i->dispenseId)ids.push_back(i->dispenseId);MedicationCollectionDAO dao;dao.getSqlSession()->beginTransaction();try{auto rows=dao.lockSummarizable(ids);if(rows.size()!=ids.size())throw std::runtime_error("already summarized");std::map<std::string,std::string> noByLocation;UuidFacade uf;for(auto&r:rows){auto&no=noByLocation[r.locationId];if(no.empty()){no="MS"+uf.genUuid();out->summaryNos->push_back(no);}if(dao.attachSummary(r.id,no,op)!=1||dao.insertSupplyLine(uf.genUuid(),no,r,op)!=1)throw std::runtime_error("summarize failed");}dao.getSqlSession()->commitTransaction();out->message="汇总领药成功";}catch(...){dao.getSqlSession()->rollbackTransaction();out->summaryNos=oatpp::List<oatpp::String>::createShared();}return out;}
