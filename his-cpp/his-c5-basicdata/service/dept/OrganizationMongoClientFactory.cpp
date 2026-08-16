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

    struct MongoConnectionConfig
    {
        std::string host;
        std::string port;
        std::string database;
        std::string username;
        std::string password;
        std::string legacyUri;
    };

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

    std::string readRequiredScalar(
        const YAML::Node& node,
        const char* key,
        const char* fullPath
    )
    {
        const auto valueNode = node[key];
        if (!valueNode || !valueNode.IsScalar())
        {
            throw std::runtime_error(
                std::string("missing ") + fullPath
            );
        }

        const std::string value = valueNode.as<std::string>();
        if (value.empty())
        {
            throw std::runtime_error(
                std::string(fullPath) + " is empty"
            );
        }

        return value;
    }

    void validateMongoPort(const std::string& port)
    {
        std::size_t parsedLength = 0;
        int portNumber = 0;

        try
        {
            portNumber = std::stoi(port, &parsedLength);
        }
        catch (const std::exception&)
        {
            throw std::runtime_error(
                "spring.data.mongodb.port is not a valid port"
            );
        }

        if (parsedLength != port.size() ||
            portNumber < 1 || portNumber > 65535)
        {
            throw std::runtime_error(
                "spring.data.mongodb.port is not a valid port"
            );
        }
    }

    MongoConnectionConfig readMongoConfig(const YAML::Node& root)
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

        if (!mongodb || !mongodb.IsMap())
        {
            throw std::runtime_error(
                "missing spring.data.mongodb"
            );
        }

        const bool hasSeparatedField =
            static_cast<bool>(mongodb["host"]) ||
            static_cast<bool>(mongodb["port"]) ||
            static_cast<bool>(mongodb["database"]) ||
            static_cast<bool>(mongodb["username"]) ||
            static_cast<bool>(mongodb["password"]);

        MongoConnectionConfig config;
        if (hasSeparatedField)
        {
            config.host = readRequiredScalar(
                mongodb,
                "host",
                "spring.data.mongodb.host"
            );
            config.port = readRequiredScalar(
                mongodb,
                "port",
                "spring.data.mongodb.port"
            );
            config.database = readRequiredScalar(
                mongodb,
                "database",
                "spring.data.mongodb.database"
            );
            config.username = readRequiredScalar(
                mongodb,
                "username",
                "spring.data.mongodb.username"
            );
            config.password = readRequiredScalar(
                mongodb,
                "password",
                "spring.data.mongodb.password"
            );

            validateMongoPort(config.port);
            return config;
        }

        // Keep compatibility with the existing Windows development config.
        config.legacyUri = readRequiredScalar(
            mongodb,
            "uri",
            "spring.data.mongodb.uri"
        );
        return config;
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
        const auto config = readMongoConfig(loadDataSourceConfig());
        if (!config.legacyUri.empty())
        {
            client = std::make_shared<MongoClient>(config.legacyUri);
        }
        else
        {
            client = std::make_shared<MongoClient>(
                config.username,
                config.password,
                config.host,
                config.database,
                config.port
            );
        }
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
