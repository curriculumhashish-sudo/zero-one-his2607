#ifndef _MONTH_VISIT_DAO_
#define _MONTH_VISIT_DAO_
#include "BaseDAO.h"
#include"../../domain/query/data_statistics/MonthVisitQuery.h"
#include "NacosClient.h"
#include "MongoClient.h"
#include <iostream>
#include <bsoncxx/json.hpp>
#include"domain/dto/data_statistics/MonthVisitDTO.h"

class MonthVisitDAO : public BaseDAO
{
private:
	MongoClient* mc = nullptr;
public:
	MonthVisitDAO()
	{
		#ifdef LINUX
				// 获取Nacos中的数据库配置
				std::string nacosAddr = "121.43.236.176";
				std::string nacosNs = "";
				std::string nacosUsername = "nacos";
				std::string nacosPassword = "nacos";
				NacosClient nacosClient(nacosAddr, nacosNs, nacosUsername, nacosPassword);
				YAML::Node node = nacosClient.getConfig("data-source.yaml");
		#else
				// 获取配置文件中的数据库配置
				YAML::Node node = YAML::LoadFile("./conf/data-source.yaml");
		#endif
				// 获取mongodb相关配置
				YamlHelper yaml;
				string uri = yaml.getString(&node, "spring.data.mongodb.uri");

				// 创建连接对象
				this->mc = new MongoClient(uri);
	}


	u_int64 count(const MonthVisitQuery::Wrapper& query)
	{
		u_int64 result;
		bool res = mc->execute("access_monthly_vector",
			[query, &result](mongocxx::collection* collection) {
				auto monthFrom = query->monthFrom.get();
				auto monthTo = query->monthTo.get();
				auto filter = bsoncxx::builder::basic::make_document(
					bsoncxx::builder::basic::kvp("yearMonth", bsoncxx::builder::basic::make_document(
						bsoncxx::builder::basic::kvp("$gte", *monthFrom),
						bsoncxx::builder::basic::kvp("$lte", *monthTo)
					)),
					bsoncxx::builder::basic::kvp("hospitalId", *(query->organizationId.get()))
				);
				result = collection->count_documents(filter.view());
			});
		return result;
	}

	std::list<MonthVisitDTO::Wrapper> findMonthVisit(const MonthVisitQuery::Wrapper& query)
	{
		std::list<MonthVisitDTO::Wrapper> result;
		bool res = mc->execute("（access_monthly_vector",
			[query, &result](mongocxx::collection* collection) {
				auto monthFrom = query->monthFrom.get();
				auto monthTo = query->monthTo.get();
				auto filter = bsoncxx::builder::basic::make_document(
					bsoncxx::builder::basic::kvp("yearMonth", bsoncxx::builder::basic::make_document(
						bsoncxx::builder::basic::kvp("$gte", *monthFrom),
						bsoncxx::builder::basic::kvp("$lte", *monthTo)
					)),
					bsoncxx::builder::basic::kvp("hospitalId", *(query->organizationId.get()))
				);
				mongocxx::options::find opts{};
				opts.projection(bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("_id", 0), bsoncxx::builder::basic::kvp("dailyCounts", 1), bsoncxx::builder::basic::kvp("totalCount", 1)));
				opts.skip((query->pageIndex - 1) * query->pageSize);
				opts.limit(query->pageSize);
				auto cursor = collection->find(filter.view(), opts);
				for (auto &&doc : cursor) {
					cout << bsoncxx::to_json(doc) << endl;
					auto dto = MonthVisitDTO::createShared();
					auto dailyCounts = doc["dailyCounts"];
					auto totalCount = doc["totalCount"];
					if(dailyCounts&& dailyCounts.type() == bsoncxx::type::k_array)
					{
						auto arr = dailyCounts.get_array().value;
						for(auto daily: arr)
						{
							dto->daytList->push_back(daily.get_int64().value);
						}
					}
					if(totalCount)
					{
						dto->totalVisit = totalCount.get_int64().value;
					}
					result.push_back(dto);
				}
			});
		delete this->mc;
		return result;
	}

	~MonthVisitDAO()
	{
		delete this->mc;
	}
};

#endif 