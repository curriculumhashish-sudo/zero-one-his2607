/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/18 22:50:00

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
#include "DispenseService.h"
#include "../../dao/medicationcollection/DispenseDAO.h"
#ifdef USE_REDIS
#include <fstream>
#include "RedisClient.h"
#include "YamlHelper.h"
#endif

#ifdef USE_REDIS
namespace {
	// cache expire seconds (flow-chart cache step)
	constexpr int DISPENSE_CACHE_TTL = 60;

	// one lazy-connect redis client built from local conf.
	// RedisClient::execute swallows connection errors, so when redis is offline
	// every cache read degrades to a miss and logic falls through to the database.
	RedisClient& getRedisClient()
	{
		static RedisClient client = [] {
			YAML::Node config = YAML::LoadFile("./conf/data-source.yaml");
			YamlHelper yaml;
			string host = yaml.getString(&config, "spring.data.redis.host");
			string port = yaml.getString(&config, "spring.data.redis.port");
			string password = yaml.getString(&config, "spring.data.redis.password");
			return RedisClient(host, atoi(port.c_str()), password);
		}();
		return client;
	}

	// read cached string by key, empty string means miss
	string cacheGet(const string& key)
	{
		if (!std::ifstream("./conf/data-source.yaml").good())
		{
			return {};
		}
		try
		{
			return getRedisClient().execute<string>([&](Redis* redis) -> string {
				auto val = redis->get(key);
				return val ? *val : string();
			});
		}
		catch (...)
		{
			return {};
		}
	}

	// write cached string with ttl
	void cachePut(const string& key, const string& value)
	{
		if (!std::ifstream("./conf/data-source.yaml").good())
		{
			return;
		}
		try
		{
			getRedisClient().execute<bool>([&](Redis* redis) -> bool {
				redis->setex(key, DISPENSE_CACHE_TTL, value);
				return true;
			});
		}
		catch (...)
		{
			// Cache failures must not affect the database query response.
		}
	}
}
#endif

DispenseSummaryPageDTO::Wrapper DispenseService::listSummary(
	const DispenseSummaryQuery::Wrapper& query, const std::string& userId)
{
#ifdef USE_REDIS
	auto jsonMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();

	// cache key contains all filter and page dimensions
	string cacheKey = "dispense:summary:" + query->statusEnum.getValue("")
		+ ":" + query->dispenseDate.getValue("")
		+ ":" + query->destinationLocationId.getValue("")
		+ ":" + query->startDate.getValue("")
		+ ":" + query->endDate.getValue("")
		+ ":" + query->keyword.getValue("")
		+ ":" + std::to_string(query->pageIndex.getValue(1))
		+ ":" + std::to_string(query->pageSize.getValue(10));
	cacheKey += ":" + userId;

	// cache check (flow-chart step): hit -> assemble from cache directly
	string cached = cacheGet(cacheKey);
	if (!cached.empty())
	{
		try {
			return jsonMapper->readFromString<oatpp::Object<DispenseSummaryPageDTO>>(cached);
		}
		catch (...) { /* broken cache content, fall through to database */ }
	}
#endif

	// build page result object (clear template demo defaults)
	auto pages = DispenseSummaryPageDTO::createShared();
	pages->pageIndex = query->pageIndex;
	pages->pageSize = query->pageSize;
	pages->total = (v_int64)0;
	pages->pages = (v_int64)0;
	pages->rows = oatpp::List<DispenseSummaryDTO::Wrapper>::createShared();

	// cache miss -> query database (med_medication_dispense)
	DispenseDAO dao;
	uint64_t count = dao.countSummary(query, userId);
	if (count <= 0)
	{
		return pages;
	}

	// query rows with pagination
	pages->total = count;
	pages->calcPages();
	list<DispenseSummaryDO> result = dao.selectSummaryWithPage(query, userId);
	// convert DO to DTO
	for (DispenseSummaryDO& sub : result)
	{
		auto dto = DispenseSummaryDTO::createShared();
		ZO_STAR_DOMAIN_DO_TO_DTO(dto, sub,
			summaryNo, SummaryNo,
			statusEnum, StatusEnum,
			statusText, StatusText,
			desLocationId, DesLocationId,
			desLocationName, DesLocationName,
			applicantId, ApplicantId,
			applicantName, ApplicantName,
			applyTime, ApplyTime,
			receiverId, ReceiverId,
			itemCount, ItemCount,
			plannedDispenseTime, PlannedDispenseTime,
			dispenseTime, DispenseTime,
			version, Version);
		if (dto->statusEnum == "PENDING_SUBMIT")
			dto->statusText = ZH_WORDS_GETTER("business-text.medication-collection.status-pending-submit");
		else if (dto->statusEnum == "SUBMITTED")
			dto->statusText = ZH_WORDS_GETTER("business-text.medication-collection.status-submitted");
		else if (dto->statusEnum == "DISPENSED")
			dto->statusText = ZH_WORDS_GETTER("business-text.medication-collection.status-dispensed");
		else if (dto->statusEnum == "CANCELLED")
			dto->statusText = ZH_WORDS_GETTER("business-text.medication-collection.status-cancelled");
		pages->addData(dto);
	}

	// Add cache when Redis support and configuration are available.
#ifdef USE_REDIS
	cachePut(cacheKey, jsonMapper->writeToString(pages).getValue(""));
#endif

	return pages;
}

oatpp::List<oatpp::Object<DispenseDetailDTO>> DispenseService::listDetail(const std::string& summaryNo)
{
#ifdef USE_REDIS
	auto jsonMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();

	// cache key by summary bill no
	string cacheKey = "dispense:detail:" + summaryNo;

	// cache check (flow-chart step): hit -> id/name/quantity/unit_code from cache
	string cached = cacheGet(cacheKey);
	if (!cached.empty())
	{
		try {
			return jsonMapper->readFromString<oatpp::List<oatpp::Object<DispenseDetailDTO>>>(cached);
		}
		catch (...) { /* broken cache content, fall through to database */ }
	}
#endif

	auto rows = oatpp::List<oatpp::Object<DispenseDetailDTO>>::createShared();

	// cache miss -> query database:
	// med_medication_dispense (quantity/unit_code/medication_id by summary_no)
	// join med_medication_definition for medication name
	DispenseDAO dao;
	list<DispenseDetailDO> result = dao.selectDetailBySummaryNo(summaryNo);
	// convert DO to DTO
	for (DispenseDetailDO& sub : result)
	{
		auto dto = DispenseDetailDTO::createShared();
		ZO_STAR_DOMAIN_DO_TO_DTO(dto, sub,
			id, Id,
			medicationId, MedicationId,
			name, Name,
			quantity, Quantity,
			unitCode, UnitCode,
			specification, Specification,
			packageDisplay, PackageDisplay,
			unitPrice, UnitPrice,
			totalPrice, TotalPrice);
		rows->push_back(dto);
	}

	// Add cache when Redis support and configuration are available.
#ifdef USE_REDIS
	cachePut(cacheKey, jsonMapper->writeToString(rows).getValue(""));
#endif

	return rows;
}
