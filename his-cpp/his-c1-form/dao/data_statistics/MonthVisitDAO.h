#ifndef _MONTH_VISIT_DAO_
#define _MONTH_VISIT_DAO_
#include "BaseDAO.h"
#include"../../domain/query/data_statistics/MonthVisitQuery.h"
#include "NacosClient.h"
#include "MongoClient.h"
#include <iostream>
#include <bsoncxx/json.hpp>

class MonthVisitDAO : public BaseDAO
{
private:
	MongoClient* mc = nullptr;
public:
	std::string findMonthVisit(const MonthVisitQuery::Wrapper& query)
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
		std::string result;
		bool res = mc->execute("（access_monthly_vector",
			[query, &result](mongocxx::collection* collection) {
				mongocxx::options::find opt{};
				opt.projection(bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("totalCount", 1)));
				auto cursor = collection->find(bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("yearMonth", *(query->month.get()))), opt);
				for (auto doc : cursor) {
					cout << bsoncxx::to_json(doc) << endl;
					result=(bsoncxx::to_json(doc));
				}
			});
		delete this->mc;
		return result;
	}
};

#endif 