// service/ClientManager.cpp
#include "stdafx.h"
#include "ClientManager.h"
#include "YamlHelper.h"
#include "ServerInfo.h"
#include "../dao/order/EnumDAO.h"
#include "../dao/order/OrderAuditDAO1.h"
#include <NacosClient.h>

#ifdef USE_MONGO
#include <mongocxx/instance.hpp>
#include <memory>
// 在此文件范围内持有唯一的 mongocxx::instance
static std::unique_ptr<mongocxx::instance> s_mongo_instance;
#endif

// 静态成员定义
std::shared_ptr<RedisClient> ClientManager::m_redisClient = nullptr;
std::shared_ptr<MongoClient> ClientManager::m_mongoClient = nullptr;
bool ClientManager::m_initialized = false;

void ClientManager::init() {
    if (m_initialized) {
        return;
    }

    std::cout << "[ClientManager] Initializing..." << std::endl;

    try {
        YAML::Node config;

#ifdef LINUX
        // Linux: 从 Nacos 获取配置
        NacosClient nacosClient(
            ServerInfo::getInstance().getNacosAddr(),
            ServerInfo::getInstance().getNacosNs(),
            ServerInfo::getInstance().getNacosUser(),
            ServerInfo::getInstance().getNacosPwd()
        );
        config = nacosClient.getConfig("data-source.yaml");
#else
        // Windows: 从本地文件获取配置
        config = YAML::LoadFile("./conf/data-source.yaml");
#endif

        YamlHelper yaml;

#ifdef USE_MONGO
        // 在创建任何 MongoClient 之前，确保在此处创建唯一的 mongocxx::instance
        if (!s_mongo_instance) {
            try {
                s_mongo_instance = std::make_unique<mongocxx::instance>();
            }
            catch (const std::exception& e) {
                // 如果别处已创建 instance，这里会抛出，记录但继续（不会影响后续使用）
                std::cerr << "[ClientManager] mongocxx::instance init warning: " << e.what() << std::endl;
            }
        }
#endif

        // ===== 初始化 Redis =====
        try {
            std::string redisHost = yaml.getString(&config, "spring.data.redis.host");
            int redisPort = std::stoi(yaml.getString(&config, "spring.data.redis.port"));
            std::string redisPassword = yaml.getString(&config, "spring.data.redis.password");

            m_redisClient = std::make_shared<RedisClient>(redisHost, redisPort, redisPassword);
            std::cout << "[ClientManager] Redis client initialized ("
                << redisHost << ":" << redisPort << ")" << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "[ClientManager] Redis init failed: " << e.what() << std::endl;
            m_redisClient = nullptr;
        }

        // ===== 初始化 MongoDB =====
        try {
            std::string mongoUri = yaml.getString(&config, "spring.data.mongodb.uri");
            m_mongoClient = std::make_shared<MongoClient>(mongoUri);
            std::cout << "[ClientManager] MongoDB client initialized" << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "[ClientManager] MongoDB init failed: " << e.what() << std::endl;
            m_mongoClient = nullptr;
        }

        // ===== 创建 MongoDB 索引 =====
        if (m_mongoClient) {
            try {
                // 枚举集合索引
                EnumDAO enumDao(m_mongoClient);
                enumDao.ensureCollectionAndIndexes();

                // 审计日志集合索引
                OrderAuditDAO1 auditDao(m_mongoClient);
                auditDao.ensureCollectionAndIndexes();

                std::cout << "[ClientManager] MongoDB indexes created." << std::endl;
            }
            catch (const std::exception& e) {
                std::cerr << "[ClientManager] MongoDB index creation failed: " << e.what() << std::endl;
            }
        }

        m_initialized = true;
        std::cout << "[ClientManager] Initialization completed." << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "[ClientManager] Init failed: " << e.what() << std::endl;
        m_initialized = false;
    }
}

void ClientManager::destroy() {
    m_redisClient = nullptr;
    m_mongoClient = nullptr;
#ifdef USE_MONGO
    s_mongo_instance.reset();
#endif
    m_initialized = false;
    std::cout << "[ClientManager] Destroyed." << std::endl;
}