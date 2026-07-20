/*
 Copyright Zero One Star. All rights reserved.

 @Author: his-c4
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
#include "RedisClient.h"
#include "YamlHelper.h"

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
		return getRedisClient().execute<string>([&](Redis* redis) -> string {
			auto val = redis->get(key);
			return val ? *val : string();
		});
	}

	// write cached string with ttl
	void cachePut(const string& key, const string& value)
	{
		getRedisClient().execute<bool>([&](Redis* redis) -> bool {
			redis->setex(key, DISPENSE_CACHE_TTL, value);
			return true;
		});
	}
}

DispenseSummaryPageDTO::Wrapper DispenseService::listSummary(const DispenseSummaryQuery::Wrapper& query)
{
	auto jsonMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();

	// cache key contains all filter and page dimensions
	string cacheKey = "dispense:summary:" + query->statusEnum.getValue("")
		+ ":" + query->dispenseDate.getValue("")
		+ ":" + std::to_string(query->pageIndex.getValue(1))
		+ ":" + std::to_string(query->pageSize.getValue(10));

	// cache check (flow-chart step): hit -> assemble from cache directly
	string cached = cacheGet(cacheKey);
	if (!cached.empty())
	{
		try {
			return jsonMapper->readFromString<oatpp::Object<DispenseSummaryPageDTO>>(cached);
		}
		catch (...) { /* broken cache content, fall through to database */ }
	}

	// build page result object (clear template demo defaults)
	auto pages = DispenseSummaryPageDTO::createShared();
	pages->pageIndex = query->pageIndex;
	pages->pageSize = query->pageSize;
	pages->total = (v_int64)0;
	pages->pages = (v_int64)0;

	// cache miss -> query database (med_medication_dispense)
	DispenseDAO dao;
	uint64_t count = dao.countSummary(query);
	if (count <= 0)
	{
		return pages;
	}

	// query rows with pagination
	pages->total = count;
	pages->calcPages();
	list<DispenseSummaryDO> result = dao.selectSummaryWithPage(query);
	// convert DO to DTO
	for (DispenseSummaryDO& sub : result)
	{
		auto dto = DispenseSummaryDTO::createShared();
		ZO_STAR_DOMAIN_DO_TO_DTO(dto, sub,
			summaryNo, SummaryNo,
			statusEnum, StatusEnum,
			desLocationId, DesLocationId,
			itemCount, ItemCount,
			plannedDispenseTime, PlannedDispenseTime,
			dispenseTime, DispenseTime);
		pages->addData(dto);
	}

	// add cache (flow-chart step)
	cachePut(cacheKey, jsonMapper->writeToString(pages).getValue(""));

	return pages;
}

oatpp::List<oatpp::Object<DispenseDetailDTO>> DispenseService::listDetail(const std::string& summaryNo)
{
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
			name, Name,
			quantity, Quantity,
			unitCode, UnitCode);
		rows->push_back(dto);
	}

	// add cache (flow-chart step)
	cachePut(cacheKey, jsonMapper->writeToString(rows).getValue(""));

	return rows;
}
