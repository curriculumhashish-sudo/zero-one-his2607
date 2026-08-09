#ifndef _FLOW_TREND_DAO_
#define _FLOW_TREND_DAO_
#include "BaseDAO.h"
#include"../../domain/query/data_statistics/TrafficTrendQuery.h"
#include "NacosClient.h"
#include "MongoClient.h"
#include <iostream>
#include <bsoncxx/json.hpp>
#include"domain/dto/data_statistics/TrafficTrendDTO.h"
class TrafficTrendDAO : public BaseDAO
{
private:
	MongoClient* mc = nullptr;

public:
	TrafficTrendDAO()
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

	uint64_t count(const TrafficTrendQuery::Wrapper& query)
	{
		uint64_t result;
		bool res = mc->execute("access_daily_trend",
			[query, &result](mongocxx::collection* collection) {
				auto dateFrom = query->dateFrom.getValue("");
				auto dateTo = query->dateTo.getValue("");
				auto filter = bsoncxx::builder::basic::make_document(
					bsoncxx::builder::basic::kvp("date", bsoncxx::builder::basic::make_document(
						bsoncxx::builder::basic::kvp("$gte", dateFrom),
						bsoncxx::builder::basic::kvp("$lte", dateTo)
					)),
					bsoncxx::builder::basic::kvp("hospitalId",query->organizationId.getValue(""))
				);
				result = collection->count_documents(filter.view());
			});
		return result;
	}

	std::list<TrafficTrendDTO::Wrapper> findFlowTreand(const TrafficTrendQuery::Wrapper& query)
	{
		std::list<TrafficTrendDTO::Wrapper> result;
		bool res = mc->execute("access_daily_trend",
			[query,&result](mongocxx::collection* collection) {
				mongocxx::options::find opts{};
				auto dateFrom = query->dateFrom.getValue("");
				auto dateTo = query->dateTo.getValue("");
				opts.projection(bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("_id", 0),bsoncxx::builder::basic::kvp("hourlyCounts", 1)));
				opts.skip((query->pageIndex - 1) * query->pageSize);
				opts.limit(query->pageSize);
				opts.sort(bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("date", 1)));
				auto filter = bsoncxx::builder::basic::make_document(
					bsoncxx::builder::basic::kvp("date", bsoncxx::builder::basic::make_document(
						bsoncxx::builder::basic::kvp("$gte", dateFrom),
						bsoncxx::builder::basic::kvp("$lte", dateTo)
					)),
					bsoncxx::builder::basic::kvp("hospitalId",query->organizationId.getValue(""))
				);
				auto cursor = collection->find(filter.view(), opts);
				for (auto&& doc : cursor) {
					auto dto= TrafficTrendDTO::createShared();
					dto->visit_list->clear();
					cout << bsoncxx::to_json(doc) << endl;
					auto hourlyCounts = doc["hourlyCounts"];
					if (hourlyCounts && hourlyCounts.type() == bsoncxx::type::k_array) {
						auto arr = hourlyCounts.get_array().value;
						for (auto && filed:arr)
						{
							dto->visit_list->push_back(filed.get_int64().value);
						}
					}
					result.push_back(dto);
				}
				});
		return result;
	}

	~TrafficTrendDAO(){
		delete this->mc;
	}
};

#endif 