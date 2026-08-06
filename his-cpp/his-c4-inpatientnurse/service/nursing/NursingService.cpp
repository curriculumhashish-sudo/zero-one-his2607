/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/28 00:00:00

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
#include "NursingService.h"
#include "dao/nursing/NursingDAO.h"
#include "domain/do/nursing/VitalSignDO.h"
#include "domain/do/nursing/VitalSignDetailDO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"
#include "TransactionManager.h"
#include <algorithm>
#include <map>
#include <sstream>


static void splitDateTime(const std::string& dt, std::string& date, std::string& time)
{
    if (dt.length() >= 16)
    {
        date = dt.substr(0, 10);
        time = dt.substr(11, 5);
    }
    else if (dt.length() >= 10)
    {
        date = dt.substr(0, 10);
    }
}


static std::string buildDateTime(const std::string& date, const std::string& time)
{
    if (date.length() >= 10)
        return date.substr(5) + " " + (time.empty() ? "00:00" : time);
    return date + " " + (time.empty() ? "00:00" : time);
}


static NursingRecordPaperHeaderDTO::Wrapper buildPaperHeader(const PaperHeaderData& d)
{
    auto h = NursingRecordPaperHeaderDTO::createShared();
    h->inpatientNo = d.inpatientNo;
    h->name        = d.patientName;
    h->gender      = d.gender;
    h->age         = std::to_string(d.age);
    h->admitAt     = d.admitAt;
    h->dischargeAt = d.dischargeAt;
    h->wardName    = d.wardName;
    h->bedNo       = d.bedNo;
    return h;
}


static void mapDetailToRow(const VitalSignDetailDO& d, NursingRecordRowDTO::Wrapper& row)
{
    const std::string& code = d.getItemCode();
    const std::string& val = d.getItemValue();

    if (code == "temperature")            row->temperature = val;
    else if (code == "pulse")             row->pulse = val;
    else if (code == "heart_rate")        row->heartRate = val;
    else if (code == "respiration")       row->respiration = val;
    else if (code == "blood_pressure")    row->bloodPressure = val;
    else if (code == "consciousness")     row->consciousness = val;
    else if (code == "pupil_left")        row->pupilLeft = val;
    else if (code == "pupil_right")       row->pupilRight = val;
    else if (code == "observation")       row->observation = val;
    else if (code == "intake_name")       row->intakeName = val;
    else if (code == "intake_volume")     row->intakeVolume = val;
    else if (code == "output_name")       row->outputName = val;
    else if (code == "output_volume")     row->outputVolume = val;
    else if (code == "output_color")      row->outputColor = val;
    else if (code == "nurse_signature")   row->nurseSignature = val;
}


static void copySaveItemToRow(const NursingRecordSaveItemDTO::Wrapper& src,
                              NursingRecordRowDTO::Wrapper& dst)
{
    dst->dateTime          = src->dateTime;
    dst->vitalSignId       = src->vitalSignId;
    dst->recordDate        = src->recordDate;
    dst->recordTime        = src->recordTime;
    dst->temperature       = src->temperature;
    dst->pulse             = src->pulse;
    dst->heartRate         = src->heartRate;
    dst->respiration       = src->respiration;
    dst->bloodPressure     = src->bloodPressure;
    dst->consciousness     = src->consciousness;
    dst->pupilLeft         = src->pupilLeft;
    dst->pupilRight        = src->pupilRight;
    dst->intakeName        = src->intakeName;
    dst->intakeVolume      = src->intakeVolume;
    dst->outputName        = src->outputName;
    dst->outputVolume      = src->outputVolume;
    dst->outputColor       = src->outputColor;
    dst->observation       = src->observation;
    dst->nurseSignature    = src->nurseSignature;
}


static void addDetailItem(std::list<VitalSignDetailDO>& details,
                          const std::string& vitalSignId,
                          const oatpp::String& val,
                          const char* itemCode, const char* units,
                          const std::string& createdBy, const std::string& createdAt,
                          UuidFacade& uf)
{
    if (!val || val->empty()) return;
    VitalSignDetailDO& detail = details.emplace_back();
    detail.setId(uf.genUuid());
    detail.setVitalSignId(vitalSignId);
    detail.setTermCode("vital_signs_code");
    detail.setItemCode(itemCode);
    detail.setItemValue(val.getValue(""));
    if (units) detail.setUnits(units);
    detail.setCreatedBy(createdBy);
    detail.setCreatedAt(createdAt);
    detail.setVersion(0);
    detail.setIsDeleted(0);
}


NursingRecordListResponseDTO::Wrapper NursingService::listRecords(const NursingRecordQuery::Wrapper& query)
{
    auto dto = NursingRecordListResponseDTO::createShared();

    NursingDAO dao;
    auto vitalSigns = dao.selectByCondition(query);


    std::string encId = query->encounterId ? query->encounterId.getValue("") : "";
    std::string patId = query->patientId ? query->patientId.getValue("") : "";
    auto paperData = dao.selectPaperHeader(encId, patId);
    dto->paperHeader = buildPaperHeader(*paperData);

    if (vitalSigns.empty())
    {
        dto->rows = oatpp::List<NursingRecordRowDTO::Wrapper>::createShared();
        return dto;
    }


    std::list<std::string> ids;
    for (auto& vs : vitalSigns)
        ids.push_back(vs.getId());


    auto allDetails = dao.selectByVitalSignIds(ids);


    std::map<std::string, std::list<VitalSignDetailDO>> detailMap;
    for (auto& d : allDetails)
        detailMap[d.getVitalSignId()].push_back(std::move(d));

    auto rows = oatpp::List<NursingRecordRowDTO::Wrapper>::createShared();
    for (auto& vs : vitalSigns)
    {
        auto row = NursingRecordRowDTO::createShared();
        row->vitalSignId = vs.getId();
		row->version = static_cast<v_int64>(vs.getVersion());
		row->createdBy = vs.getCreatedBy();
		row->createdAt = vs.getCreatedAt();
		row->updatedBy = vs.getUpdatedBy();
		row->updatedAt = vs.getUpdatedAt();

  
        std::string datePart, timePart;
        splitDateTime(vs.getRecordedAt(), datePart, timePart);
        row->recordDate = datePart;
        row->recordTime = timePart;
        row->dateTime = buildDateTime(datePart, timePart);

        auto& details = detailMap[vs.getId()];
        for (auto& d : details)
            mapDetailToRow(d, row);

        rows->push_back(row);
    }

    dto->rows = rows;
    return dto;
}


std::string NursingService::generateBusNo(NursingDAO& dao)
{
    std::string today = SimpleDateTimeFormat::format();
    std::string datePart = today.substr(0, 10);
    // Remove dashes: "2026-07-28" → "20260728"
    datePart.erase(std::remove(datePart.begin(), datePart.end(), '-'), datePart.end());


    uint64_t cnt = dao.countByBusNoPrefix("NR" + datePart);

    std::stringstream busNo;
    busNo << "NR" << datePart << std::setw(3) << std::setfill('0') << (cnt + 1);
    return busNo.str();
}


NursingRecordSaveResponseDTO::Wrapper NursingService::saveRecords(const NursingRecordSaveRequestDTO::Wrapper& dto)
{
    auto resp = NursingRecordSaveResponseDTO::createShared();

    std::string encId = dto->encounterId ? dto->encounterId.getValue("") : "";
    std::string patId = dto->patientId ? dto->patientId.getValue("") : "";

    std::string createdBy = "system";
    std::string createdAt = SimpleDateTimeFormat::format();

    UuidFacade uf(1);
    NursingDAO dao;


    TransactionManager tm(&dao);

    int savedCount = 0;
    int busNoSeq = 0; 
    auto savedRows = oatpp::List<NursingRecordRowDTO::Wrapper>::createShared();

    try
    {
        for (auto& item : *dto->records.get())
        {
            std::string recordDate = item->recordDate ? item->recordDate.getValue("") : "";
            std::string recordTime = item->recordTime ? item->recordTime.getValue("") : "";
            std::string vitalSignId = item->vitalSignId ? item->vitalSignId.getValue("") : "";


            std::string recordedAt = recordDate;
            if (!recordTime.empty())
                recordedAt += " " + recordTime + ":00";
            else if (!recordDate.empty())
                recordedAt += " 00:00:00";


            if (!vitalSignId.empty())
            {

                dao.deleteByVitalSignId(vitalSignId);

                dao.softDeleteById(vitalSignId);
            }
            else
            {

                auto existing = dao.selectBySlot(encId, patId, recordDate, recordTime);
                if (!existing.empty())
                {
                    vitalSignId = existing.front().getId();
                    dao.deleteByVitalSignId(vitalSignId);
                    dao.softDeleteById(vitalSignId);
                }
                else
                {
                    vitalSignId = uf.genUuid();
                }
            }


            VitalSignDO data;
            data.setId(vitalSignId);
            data.setPatientId(patId);
            data.setEncounterId(encId);
            data.setRecordedAt(recordedAt);
            data.setTypeEnum("NURSING_RECORD");


            std::string busNo = generateBusNo(dao);

            if (busNoSeq > 0)
            {
                busNo += std::to_string(busNoSeq);
            }
            busNoSeq++;
            data.setBusNo(busNo);

            data.setCreatedBy(createdBy);
            data.setCreatedAt(createdAt);
            data.setVersion(0);
            data.setIsDeleted(0);


            int rows = dao.insert(data);
            if (rows != 1) continue;


            std::list<VitalSignDetailDO> details;
            addDetailItem(details, vitalSignId, item->temperature,    "temperature",     "C",            createdBy, createdAt, uf);
            addDetailItem(details, vitalSignId, item->pulse,          "pulse",           "bpm",          createdBy, createdAt, uf);
            addDetailItem(details, vitalSignId, item->heartRate,      "heart_rate",      "bpm",          createdBy, createdAt, uf);
            addDetailItem(details, vitalSignId, item->respiration,    "respiration",     "breaths/min",  createdBy, createdAt, uf);
            addDetailItem(details, vitalSignId, item->bloodPressure,  "blood_pressure",  "mmHg",         createdBy, createdAt, uf);
            addDetailItem(details, vitalSignId, item->consciousness,  "consciousness",   nullptr,        createdBy, createdAt, uf);
            addDetailItem(details, vitalSignId, item->pupilLeft,      "pupil_left",      "mm",           createdBy, createdAt, uf);
            addDetailItem(details, vitalSignId, item->pupilRight,     "pupil_right",     "mm",           createdBy, createdAt, uf);
            addDetailItem(details, vitalSignId, item->intakeName,     "intake_name",     nullptr,        createdBy, createdAt, uf);
            addDetailItem(details, vitalSignId, item->intakeVolume,   "intake_volume",   "ml",           createdBy, createdAt, uf);
            addDetailItem(details, vitalSignId, item->outputName,     "output_name",     nullptr,        createdBy, createdAt, uf);
            addDetailItem(details, vitalSignId, item->outputVolume,   "output_volume",   "ml",           createdBy, createdAt, uf);
            addDetailItem(details, vitalSignId, item->outputColor,    "output_color",    nullptr,        createdBy, createdAt, uf);
            addDetailItem(details, vitalSignId, item->observation,    "observation",     nullptr,        createdBy, createdAt, uf);
            addDetailItem(details, vitalSignId, item->nurseSignature, "nurse_signature", nullptr,        createdBy, createdAt, uf);

            if (!details.empty())
            {
                int detailRows = dao.insertBatch(details);
                if (detailRows != static_cast<int>(details.size()))
                    continue;
            }


            auto savedRow = NursingRecordRowDTO::createShared();
            savedRow->vitalSignId = vitalSignId;
            savedRow->recordDate  = recordDate;
            savedRow->recordTime  = recordTime;
            savedRow->dateTime    = buildDateTime(recordDate, recordTime);
            copySaveItemToRow(item, savedRow);
            savedRows->push_back(savedRow);
            savedCount++;
        }


        tm.commit();
    }
    catch (...)
    {
        tm.rollback();
        resp->savedCount = 0;
        resp->rows = oatpp::List<NursingRecordRowDTO::Wrapper>::createShared();
        return resp;
    }

    resp->savedCount = savedCount;
    resp->rows = savedRows;
    return resp;
}

NursingRecordDeleteResponseDTO::Wrapper NursingService::deleteRecord(
    const NursingRecordDeleteRequestDTO::Wrapper& dto, const std::string& operatorId)
{
    auto response = NursingRecordDeleteResponseDTO::createShared();
    response->success = false;
    response->vitalSignId = dto->vitalSignId;
    response->statusEnum = "UNCHANGED";
    NursingDAO dao;
    TransactionManager transaction(&dao);
    try
    {
		const int changed = dao.softDeleteOwnedRecord(dto->vitalSignId.getValue(""),
			dto->encounterId.getValue(""), dto->patientId.getValue(""),
			static_cast<int>(dto->expectedVersion.getValue(-1)), operatorId,
			dto->reason.getValue(""));
        if (changed != 1)
        {
            transaction.rollback();
            return response;
        }
		dao.softDeleteDetails(dto->vitalSignId.getValue(""), operatorId);
        transaction.commit();
        response->success = true;
        response->statusEnum = "DELETED";
        return response;
    }
    catch (...)
    {
        transaction.rollback();
        return response;
    }
}
