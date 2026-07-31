// service/session/SessionService.cpp
#include "stdafx.h"
#include "SessionService.h"
#include <sw/redis++/redis++.h>
#include <map>

SessionService::SessionService(std::shared_ptr<RedisClient> client)
    : m_redisClient(client) {
}

std::string SessionService::sessionKey(const std::string& userId) {
    return "user:session:" + userId;
}

std::optional<std::string> SessionService::getTenantId(const std::string& userId) {
    if (!m_redisClient) {
        return std::nullopt;
    }

    return m_redisClient->execute<std::optional<std::string>>(
        [this, &userId](sw::redis::Redis* redis) -> std::optional<std::string> {
            try {
                // hget 返回 sw::redis::OptionalString
                sw::redis::OptionalString result = redis->hget(sessionKey(userId), "tenant_id");

                // 转换为 std::optional<std::string>
                if (result) {
                    return std::optional<std::string>(*result);
                }
                return std::nullopt;

            }
            catch (const std::exception& e) {
                std::cerr << "Redis hget tenant_id failed: " << e.what() << std::endl;
                return std::nullopt;
            }
        }
    );
}

std::optional<std::string> SessionService::getOrgId(const std::string& userId) {
    if (!m_redisClient) {
        return std::nullopt;
    }

    return m_redisClient->execute<std::optional<std::string>>(
        [this, &userId](sw::redis::Redis* redis) -> std::optional<std::string> {
            try {
                // hget 返回 sw::redis::OptionalString
                sw::redis::OptionalString result = redis->hget(sessionKey(userId), "hospital_organization_id");

                // 转换为 std::optional<std::string>
                if (result) {
                    return std::optional<std::string>(*result);
                }
                return std::nullopt;

            }
            catch (const std::exception& e) {
                std::cerr << "Redis hget hospital_organization_id failed: " << e.what() << std::endl;
                return std::nullopt;
            }
        }
    );
}

std::optional<std::string> SessionService::getSessionJson(const std::string& userId) {
    if (!m_redisClient) {
        return std::nullopt;
    }

    return m_redisClient->execute<std::optional<std::string>>(
        [this, &userId](sw::redis::Redis* redis) -> std::optional<std::string> {
            try {
                std::string key = sessionKey(userId);
                std::map<std::string, std::string> vals;
                redis->hgetall(key, std::inserter(vals, vals.begin()));

                if (vals.empty()) {
                    return std::nullopt;
                }

                std::string json = "{";
                bool first = true;
                for (const auto& [k, v] : vals) {
                    if (!first) json += ",";
                    first = false;
                    json += "\"" + k + "\":\"" + v + "\"";
                }
                json += "}";

                return std::optional<std::string>(json);

            }
            catch (const std::exception& e) {
                std::cerr << "Redis hgetall session failed: " << e.what() << std::endl;
                return std::nullopt;
            }
        }
    );
}