#ifndef _FLOW_TREND_DAO_
#define _FLOW_TREND_DAO_
#include "BaseDAO.h"
#include"../../domain/query/data_statistics/TrafficTrendQuery.h"
#include "NacosClient.h"
#include "MongoClient.h"
#include <iostream>
#include <bsoncxx/json.hpp>

class TrafficTrendDAO : public BaseDAO
{
private:
	MongoClient* mc = nullptr;
public:
	std::list<std::string> findFlowTreand(const TrafficTrendQuery::Wrapper& query)
	{
		#ifdef LINUX
		// 获取Nacos中的数据库配置
		std::string nacosAddr = "";
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
		std::list<std::string>result;
		bool res = mc->execute("（access_daily_trend",
			[query,&result](mongocxx::collection* collection) {
				auto cursor = collection->find(bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("date", *(query->date.get()))));
				for (auto doc : cursor) {
					cout << bsoncxx::to_json(doc) << endl;
					result.push_back(bsoncxx::to_json(doc));
				}
			});
		delete this->mc;
		return result;
	}
};

#endif 