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
#include "TemperatureService.h"
#include "dao/temperature/TemperatureDAO.h"
#include "domain/do/nursing/VitalSignDO.h"
#include "domain/do/nursing/VitalSignDetailDO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"
#include "TransactionManager.h"
#include <algorithm>
#include <iomanip>
#include <map>
#include <set>
#include <sstream>
#include <vector>


static const std::vector<std::string> TIME_SLOTS = {
    "02", "06", "10", "14", "18", "22"
};


static const std::set<std::string> PREV_DAY_ITEMS = {
    "STOOL", "URINE_FREQ", "INTAKE", "URINE", "OTHER"
};


static std::pair<std::string, std::string> computeDateRange(const std::string& focusDate,
                                                             const std::string& windowRange)
{

    std::string focus = focusDate.empty() ? SimpleDateTimeFormat::format().substr(0, 10) : focusDate;


    int days = 7;
    std::string digits;
    for (char c : windowRange)
    {
        if (c >= '0' && c <= '9') digits += c;
        else break;
    }
    if (!digits.empty()) days = std::stoi(digits);


    int half = (days - 1) / 2;
    if (half < 0) half = 0;

    int y = std::stoi(focus.substr(0, 4));
    int m = std::stoi(focus.substr(5, 2));
    int d = std::stoi(focus.substr(8, 2));


    auto daysInMonth = [](int year, int month) -> int {
        static const int mdays[] = {31,28,31,30,31,30,31,31,30,31,30,31};
        int d = mdays[month - 1];
        if (month == 2 && (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))) d = 29;
        return d;
    };


    int dy = y, dm = m, dd = d - half;
    while (dd < 1) { dm--; if (dm < 1) { dm = 12; dy--; } dd += daysInMonth(dy, dm); }

    std::stringstream dateFrom;
    dateFrom << dy << "-" << std::setw(2) << std::setfill('0') << dm
             << "-" << std::setw(2) << std::setfill('0') << dd;

    int ay = y, am = m, ad = d + half;
    while (ad > daysInMonth(ay, am)) { ad -= daysInMonth(ay, am); am++; if (am > 12) { am = 1; ay++; } }

    std::stringstream dateTo;
    dateTo << ay << "-" << std::setw(2) << std::setfill('0') << am
           << "-" << std::setw(2) << std::setfill('0') << ad;

    return { dateFrom.str(), dateTo.str() };
}


static std::string extractHour(const std::string& recordedAt)
{
    if (recordedAt.length() >= 13)
        return recordedAt.substr(11, 2);
    return "";
}


static std::string extractDate(const std::string& recordedAt)
{
    if (recordedAt.length() >= 10)
        return recordedAt.substr(0, 10);
    return recordedAt;
}


static TemperatureSheetDetailItemDTO::Wrapper mapDetailToItem(const VitalSignDetailDO& d)
{
    auto item = TemperatureSheetDetailItemDTO::createShared();
    item->itemCode  = d.getItemCode();
    item->itemValue = d.getItemValue();
    item->remark    = d.getRemark();
    item->termCode  = d.getTermCode();
    return item;
}


static TemperatureSheetTemperaturePointDTO::Wrapper buildTemperaturePoint(const VitalSignDO& vs,
                                                                           const std::list<VitalSignDetailDO>& details)
{
    auto pt = TemperatureSheetTemperaturePointDTO::createShared();
    pt->vitalSignId = vs.getId();
    pt->recordedAt  = vs.getRecordedAt();

    // Find the TEMP detail item
    for (auto& d : details)
    {
        if (d.getItemCode() == "TEMP")
        {
            pt->temperature = d.getItemValue();
            break;
        }
    }
    return pt;
}

static std::string computePrevDate(const std::string& dateStr)
{
    if (dateStr.length() < 10) return dateStr;
    int y = std::stoi(dateStr.substr(0, 4));
    int m = std::stoi(dateStr.substr(5, 2));
    int d = std::stoi(dateStr.substr(8, 2));

    auto daysInMonth = [](int year, int month) -> int {
        static const int mdays[] = {31,28,31,30,31,30,31,31,30,31,30,31};
        int d = mdays[month - 1];
        if (month == 2 && (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))) d = 29;
        return d;
    };

    d--;
    if (d < 1) { m--; if (m < 1) { m = 12; y--; } d = daysInMonth(y, m); }

    std::stringstream ss;
    ss << y << "-" << std::setw(2) << std::setfill('0') << m
       << "-" << std::setw(2) << std::setfill('0') << d;
    return ss.str();
}


std::string TemperatureService::generateBusNo(TemperatureDAO& dao)
{
    std::string today = SimpleDateTimeFormat::format();
    std::string datePart = today.substr(0, 10);
    // Remove dashes: "2026-07-28" → "20260728"
    datePart.erase(std::remove(datePart.begin(), datePart.end(), '-'), datePart.end());

    // Count today's records to derive the sequence number
    uint64_t cnt = dao.countByBusNoPrefix("TS" + datePart);

    std::stringstream busNo;
    busNo << "TS" << datePart << std::setw(3) << std::setfill('0') << (cnt + 1);
    return busNo.str();
}


TemperatureSheetGetDataResponseDTO::Wrapper TemperatureService::getTemperatureData(
    const TemperatureSheetGetDataRequestDTO::Wrapper& query)
{
    auto resp = TemperatureSheetGetDataResponseDTO::createShared();

    std::string encId    = query->encounterId    ? query->encounterId.getValue("")    : "";
    std::string patId    = query->patientId      ? query->patientId.getValue("")      : "";
    std::string focus    = query->focusRecordDate ? query->focusRecordDate.getValue("") : "";
    std::string winRange = query->windowRange     ? query->windowRange.getValue("7d")  : "7d";
    int pageIndex        = query->pageIndex       ? query->pageIndex.getValue(1)       : 1;

    auto range = computeDateRange(focus, winRange);
    std::string dateFrom = range.first;
    std::string dateTo   = range.second;
    std::string dateToQuery = dateTo + " 23:59:59"; 

    TemperatureDAO dao;
    auto vitalSigns = dao.selectByCondition(encId, patId, dateFrom, dateToQuery);

    std::list<std::string> ids;
    for (auto& vs : vitalSigns)
        ids.push_back(vs.getId());

    auto allDetails = dao.selectByVitalSignIds(ids);

    std::map<std::string, std::list<VitalSignDetailDO>> detailMap;
    for (auto& d : allDetails)
        detailMap[d.getVitalSignId()].push_back(std::move(d));


    std::map<std::string, std::map<std::string, std::string>> grid;
    struct PrevDayEntry { std::string vsId; std::string origDate; std::string slot; };
    std::map<std::string, std::map<std::string, std::vector<PrevDayEntry>>> prevDayGrid;

    auto tempPoints = oatpp::List<TemperatureSheetTemperaturePointDTO::Wrapper>::createShared();
    std::set<std::string> dateSet;

    for (auto& vs : vitalSigns)
    {
        std::string date = extractDate(vs.getRecordedAt());
        std::string hour = extractHour(vs.getRecordedAt());
        dateSet.insert(date);
        grid[date][hour] = vs.getId();

        auto& details = detailMap[vs.getId()];
        for (auto& d : details)
        {
            if (PREV_DAY_ITEMS.count(d.getItemCode()))
            {
                std::string prevDate = computePrevDate(date);
                prevDayGrid[prevDate][hour].push_back({ vs.getId(), date, hour });
                break; // one entry per vsId is enough
            }
        }

        auto pt = buildTemperaturePoint(vs, details);
        tempPoints->push_back(pt);
    }

    auto gridRows = oatpp::List<TemperatureSheetGridRowDTO::Wrapper>::createShared();
    auto fillSlots = oatpp::List<TemperatureSheetFillSlotDTO::Wrapper>::createShared();

    for (auto& datePair : grid)
    {
        auto row = TemperatureSheetGridRowDTO::createShared();
        row->recordDate = datePair.first;
        auto slotCells = oatpp::List<TemperatureSheetGridSlotCellDTO::Wrapper>::createShared();

        for (auto& slotPair : datePair.second)
        {
            const std::string& slotHour  = slotPair.first;
            const std::string& vsId      = slotPair.second;
            auto& details                = detailMap[vsId]; 

            auto detailItems = oatpp::List<TemperatureSheetDetailItemDTO::Wrapper>::createShared();
            for (auto& d : details)
            {
                if (!PREV_DAY_ITEMS.count(d.getItemCode()))
                    detailItems->push_back(mapDetailToItem(d));
            }

            if (prevDayGrid.count(datePair.first) && prevDayGrid[datePair.first].count(slotHour))
            {
                for (auto& entry : prevDayGrid[datePair.first][slotHour])
                {
                    auto& srcDetails = detailMap[entry.vsId];
                    for (auto& d : srcDetails)
                    {
                        if (PREV_DAY_ITEMS.count(d.getItemCode()))
                            detailItems->push_back(mapDetailToItem(d));
                    }
                }
            }

            auto cell = TemperatureSheetGridSlotCellDTO::createShared();
            cell->timeSlot    = slotHour;
            cell->vitalSignId = vsId;
            cell->detailItems = detailItems;
            slotCells->push_back(cell);

            auto fill = TemperatureSheetFillSlotDTO::createShared();
            fill->timeSlot    = slotHour;
            fill->vitalSignId = vsId;
            fill->detailItems = detailItems;
            fillSlots->push_back(fill);
        }

        row->slotCells = slotCells;
        gridRows->push_back(row);
    }

    for (auto& prevDatePair : prevDayGrid)
    {
        bool dateExists = grid.count(prevDatePair.first) > 0;
        for (auto& slotPair : prevDatePair.second)
        {
            for (auto& entry : slotPair.second)
            {
                if (dateExists && grid[prevDatePair.first].count(slotPair.first))
                    continue; 

                auto detailItems = oatpp::List<TemperatureSheetDetailItemDTO::Wrapper>::createShared();
                auto& srcDetails = detailMap[entry.vsId];
                for (auto& d : srcDetails)
                {
                    if (PREV_DAY_ITEMS.count(d.getItemCode()))
                        detailItems->push_back(mapDetailToItem(d));
                }

                auto fill = TemperatureSheetFillSlotDTO::createShared();
                fill->timeSlot    = slotPair.first;
                fill->vitalSignId = entry.vsId;
                fill->detailItems = detailItems;
                fillSlots->push_back(fill);
            }
        }
    }


    auto pagination = TemperatureSheetPaginationDTO::createShared();
    pagination->dateFrom   = dateFrom;
    pagination->dateTo     = dateTo;
    pagination->pageIndex  = pageIndex;
    pagination->totalPages = 1; // Single-page for temperature sheet

    auto meta = TemperatureSheetDerivedMetaDTO::createShared();
    meta->hospitalizationDaysByDate  = "";
    meta->postoperativeDaysByDate    = "";

    resp->derivedMeta       = meta;
    resp->gridRows          = gridRows;
    resp->temperaturePoints = tempPoints;
    resp->fillSlots         = fillSlots;
    resp->pagination        = pagination;
    return resp;
}


TemperatureSheetSaveResponseDTO::Wrapper TemperatureService::saveTemperatureRecord(
    const TemperatureSheetSaveRequestDTO::Wrapper& dto)
{
    auto resp = TemperatureSheetSaveResponseDTO::createShared();

    std::string encId      = dto->encounterId  ? dto->encounterId.getValue("")  : "";
    std::string patId      = dto->patientId    ? dto->patientId.getValue("")    : "";
    std::string recordDate = dto->recordDate   ? dto->recordDate.getValue("")   : "";
    std::string timeSlot   = dto->timeSlot     ? dto->timeSlot.getValue("")     : "";
    std::string vitalSignId = dto->vitalSignId ? dto->vitalSignId.getValue("")  : "";

    std::string createdBy  = "system";
    std::string createdAt  = SimpleDateTimeFormat::format();
    std::string busNo;
    UuidFacade uf(1);

    TemperatureDAO dao;

    TransactionManager tm(&dao);

    try
    {
        std::string recordedAt = recordDate + " " + timeSlot + ":00:00";

        if (!vitalSignId.empty())
        {
            dao.deleteByVitalSignId(vitalSignId);
        }
        else
        {
            auto existing = dao.selectBySlot(encId, patId, recordDate, timeSlot);
            if (!existing.empty())
            {
                vitalSignId = existing.front().getId();
                dao.deleteByVitalSignId(vitalSignId);
            }
            else
            {
                VitalSignDO data;
                data.setId(uf.genUuid());
                vitalSignId = data.getId();
                data.setPatientId(patId);
                data.setEncounterId(encId);
                data.setRecordedAt(recordedAt);
                data.setTypeEnum("TEMPERATURE_SHEET");
                busNo = generateBusNo(dao);
                data.setBusNo(busNo);
                data.setCreatedBy(createdBy);
                data.setCreatedAt(createdAt);
                data.setVersion(0);
                data.setIsDeleted(0);

                if (dao.insert(data) != 1)
                {
                    tm.rollback();
                    resp->vitalSignId = "";
                    resp->busNo = "";
                    resp->nextTimeSlot = "";
                    return resp;
                }
            }
        }

        if (busNo.empty())
        {
            auto slotRecords = dao.selectBySlot(encId, patId, recordDate, timeSlot);
            if (!slotRecords.empty())
                busNo = slotRecords.front().getBusNo();
        }

        std::list<VitalSignDetailDO> details;
        if (dto->detailItems)
        {
            for (auto& item : *dto->detailItems.get())
            {
                if (!item) continue;
                VitalSignDetailDO& detail = details.emplace_back();
                detail.setId(uf.genUuid());
                detail.setVitalSignId(vitalSignId);
                detail.setTermCode(item->termCode ? item->termCode.getValue("") : "vital_signs_code");
                detail.setItemCode(item->itemCode ? item->itemCode.getValue("") : "");
                detail.setItemValue(item->itemValue ? item->itemValue.getValue("") : "");
                detail.setRemark(item->remark ? item->remark.getValue("") : "");
                detail.setCreatedBy(createdBy);
                detail.setCreatedAt(createdAt);
                detail.setVersion(0);
                detail.setIsDeleted(0);
            }
        }

        if (!details.empty())
            dao.insertBatch(details);

        tm.commit();
    }
    catch (...)
    {
        tm.rollback();
        resp->vitalSignId = "";
        resp->busNo = "";
        resp->nextTimeSlot = "";
        return resp;
    }

    std::string nextSlot;
    for (size_t i = 0; i < TIME_SLOTS.size(); i++)
    {
        if (TIME_SLOTS[i] == timeSlot && i + 1 < TIME_SLOTS.size())
        {
            nextSlot = TIME_SLOTS[i + 1];
            break;
        }
    }

    resp->vitalSignId  = vitalSignId;
    resp->busNo        = busNo;
    resp->nextTimeSlot = nextSlot;
    return resp;
}
