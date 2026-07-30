#include "stdafx.h"
#include "OrganizationMongoClientFactory.h"

#include <stdexcept>
#include <string>

#include "NacosClient.h"
#include "ServerInfo.h"

namespace
{
    constexpr const char DATA_SOURCE_CONFIG[] = "data-source.yaml";
    constexpr const char WINDOWS_DATA_SOURCE_CONFIG[] =
        "./conf/data-source.yaml";
    constexpr const char MONGO_URI_KEY[] = "uri";

    YAML::Node loadDataSourceConfig()
    {
#ifdef LINUX
        NacosClient nacosClient(
            ServerInfo::getInstance().getNacosAddr(),
            ServerInfo::getInstance().getNacosNs(),
            ServerInfo::getInstance().getNacosUser(),
            ServerInfo::getInstance().getNacosPwd()
        );
        return nacosClient.getConfig(DATA_SOURCE_CONFIG);
#else
        return YAML::LoadFile(WINDOWS_DATA_SOURCE_CONFIG);
#endif
    }

    std::string readMongoUri(const YAML::Node& root)
    {
        if (!root || !root.IsMap())
        {
            throw std::runtime_error("data-source config is not a map");
        }

        const auto spring = root["spring"];
        const auto data = spring && spring.IsMap()
            ? spring["data"]
            : YAML::Node();
        const auto mongodb = data && data.IsMap()
            ? data["mongodb"]
            : YAML::Node();
        const auto uri = mongodb && mongodb.IsMap()
            ? mongodb[MONGO_URI_KEY]
            : YAML::Node();

        if (!uri || !uri.IsScalar())
        {
            throw std::runtime_error(
                "missing spring.data.mongodb.uri"
            );
        }

        const std::string value = uri.as<std::string>();
        if (value.empty())
        {
            throw std::runtime_error(
                "spring.data.mongodb.uri is empty"
            );
        }

        return value;
    }

}

std::shared_ptr<MongoClient> OrganizationMongoClientFactory::create()
{
#ifndef USE_MONGO
    OATPP_LOGE(
        "OrganizationMongoClientFactory",
        "MongoDB support is disabled at build time"
    );
    return nullptr;
#else
    std::shared_ptr<MongoClient> client;

    try
    {
        client = std::make_shared<MongoClient>(
            readMongoUri(loadDataSourceConfig())
        );
    }
    catch (const std::exception& exception)
    {
        OATPP_LOGE(
            "OrganizationMongoClientFactory",
            "MongoDB client configuration failed: %s",
            exception.what()
        );
        return nullptr;
    }
    catch (...)
    {
        OATPP_LOGE(
            "OrganizationMongoClientFactory",
            "MongoDB client configuration failed with an unknown exception"
        );
        return nullptr;
    }

    // MongoClient initializes its pool lazily on the first enum query. Avoid
    // connecting during component registration so an unavailable MongoDB
    // cannot delay HTTP server startup.
    OATPP_LOGI(
        "OrganizationMongoClientFactory",
        "MongoDB client created; enum connection is initialized on first query"
    );
    return client;
#endif
}
