#include "stdafx.h"
#include "OrganizationEnumDictionary.h"

#include <atomic>
#include <mutex>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "MongoClient.h"
#include "bsoncxx/builder/basic/array.hpp"
#include "bsoncxx/builder/basic/document.hpp"
#include "bsoncxx/builder/basic/kvp.hpp"
#include "bsoncxx/types.hpp"
#include "mongocxx/collection.hpp"
#include "mongocxx/cursor.hpp"
#include "mongocxx/options/find.hpp"
#include "oatpp/core/base/Environment.hpp"

namespace
{
    enum class MongoAccessState
    {
        NOT_ATTEMPTED,
        AVAILABLE,
        UNAVAILABLE
    };

    std::atomic<MongoAccessState> mongoAccessState{
        MongoAccessState::NOT_ATTEMPTED
    };
    std::mutex firstMongoAccessMutex;

    constexpr const char MONGO_COMPONENT_NAME[] =
        "dept-organization-enum-mongo";
    // Temporary convention: organization enums belong to system tenant 1.
    // Replace this constant with auth context or configuration when needed.
    constexpr const char SYSTEM_ENUM_TENANT_ID[] = "1";
    constexpr const char ENUM_TYPE_COLLECTION[] = "EnumType";
    constexpr const char ENUM_VALUE_COLLECTION[] = "EnumValue";
    constexpr const char ORGANIZATION_TYPE_KEY[] = "OrganizationType";
    constexpr const char ORGANIZATION_CLASS_KEY[] =
        "OrganizationClassification";
    constexpr const char PUBLICATION_STATUS_KEY[] = "PublicationStatus";

    oatpp::String findText(
        const std::unordered_map<std::string, std::string>& mappings,
        const std::string& code
    )
    {
        const auto iterator = mappings.find(code);
        if (iterator == mappings.end())
        {
            return nullptr;
        }

        return iterator->second.c_str();
    }

#ifdef USE_MONGO
    std::string toString(const bsoncxx::document::element& element)
    {
        if (!element)
        {
            return "";
        }

        if (element.type() == bsoncxx::type::k_string)
        {
            const auto value = element.get_string().value;
            return std::string(value.data(), value.size());
        }

        if (element.type() == bsoncxx::type::k_oid)
        {
            return element.get_oid().value.to_string();
        }

        return "";
    }

    std::string readString(
        const bsoncxx::document::view& document,
        const char* field
    )
    {
        return toString(document[field]);
    }

    bool isRequiredTypeKey(const std::string& typeKey)
    {
        return typeKey == ORGANIZATION_TYPE_KEY ||
            typeKey == ORGANIZATION_CLASS_KEY ||
            typeKey == PUBLICATION_STATUS_KEY;
    }

    void addMapping(
        const std::string& typeKey,
        const std::string& enumKey,
        const std::string& display,
        OrganizationEnumTextMap& mappings
    )
    {
        if (typeKey == ORGANIZATION_TYPE_KEY)
        {
            mappings.typeTexts.emplace(enumKey, display);
        }
        else if (typeKey == ORGANIZATION_CLASS_KEY)
        {
            mappings.classTexts.emplace(enumKey, display);
        }
        else if (typeKey == PUBLICATION_STATUS_KEY)
        {
            mappings.statusTexts.emplace(enumKey, display);
        }
    }

    bool loadEnumTypes(
        MongoClient& client,
        std::unordered_map<std::string, std::string>& typeKeyById,
        std::vector<bsoncxx::oid>& typeIds
    )
    {
        using bsoncxx::builder::basic::kvp;
        using bsoncxx::builder::basic::make_array;
        using bsoncxx::builder::basic::make_document;

        const auto filter = make_document(
            kvp("tenantId", SYSTEM_ENUM_TENANT_ID),
            kvp(
                "enumTypeKey",
                make_document(
                    kvp(
                        "$in",
                        make_array(
                            ORGANIZATION_TYPE_KEY,
                            ORGANIZATION_CLASS_KEY,
                            PUBLICATION_STATUS_KEY
                        )
                    )
                )
            ),
            kvp("isDeleted", false)
        );

        mongocxx::options::find options;
        options.projection(
            make_document(
                kvp("_id", 1),
                kvp("enumTypeKey", 1)
            )
        );

        return client.execute(
            ENUM_TYPE_COLLECTION,
            [&filter, &options, &typeKeyById, &typeIds](
                mongocxx::collection* collection
            )
            {
                auto cursor = collection->find(filter.view(), options);

                for (const auto& document : cursor)
                {
                    const auto idElement = document["_id"];
                    if (!idElement || idElement.type() != bsoncxx::type::k_oid)
                    {
                        continue;
                    }

                    const bsoncxx::oid id = idElement.get_oid().value;
                    const std::string typeKey = readString(
                        document,
                        "enumTypeKey"
                    );

                    if (isRequiredTypeKey(typeKey))
                    {
                        typeKeyById.emplace(id.to_string(), typeKey);
                        typeIds.emplace_back(id);
                    }
                }
            }
        );
    }

    bool loadEnumValues(
        MongoClient& client,
        const std::unordered_map<std::string, std::string>& typeKeyById,
        const std::vector<bsoncxx::oid>& typeIds,
        OrganizationEnumTextMap& mappings
    )
    {
        using bsoncxx::builder::basic::array;
        using bsoncxx::builder::basic::kvp;
        using bsoncxx::builder::basic::make_document;

        array typeIdValues;
        for (const auto& typeId : typeIds)
        {
            typeIdValues.append(bsoncxx::types::b_oid{typeId});
        }

        const auto filter = make_document(
            kvp("tenantId", SYSTEM_ENUM_TENANT_ID),
            kvp(
                "enumTypeId",
                make_document(kvp("$in", typeIdValues.extract()))
            ),
            kvp("isDeleted", false)
        );

        mongocxx::options::find options;
        options.projection(
            make_document(
                kvp("enumTypeId", 1),
                kvp("enumKey", 1),
                kvp("display", 1),
                kvp("_id", 0)
            )
        );

        return client.execute(
            ENUM_VALUE_COLLECTION,
            [&filter, &options, &typeKeyById, &mappings](
                mongocxx::collection* collection
            )
            {
                auto cursor = collection->find(filter.view(), options);

                for (const auto& document : cursor)
                {
                    const std::string typeId = readString(
                        document,
                        "enumTypeId"
                    );
                    const std::string enumKey = readString(
                        document,
                        "enumKey"
                    );
                    const std::string display = readString(
                        document,
                        "display"
                    );

                    const auto type = typeKeyById.find(typeId);
                    if (type != typeKeyById.end() &&
                        !enumKey.empty() && !display.empty())
                    {
                        addMapping(
                            type->second,
                            enumKey,
                            display,
                            mappings
                        );
                    }
                }
            }
        );
    }

    bool hasAllRequiredGroups(
        const std::unordered_map<std::string, std::string>& typeKeyById,
        const OrganizationEnumTextMap& mappings
    )
    {
        std::unordered_set<std::string> typeKeys;
        for (const auto& entry : typeKeyById)
        {
            typeKeys.emplace(entry.second);
        }

        return typeKeys.count(ORGANIZATION_TYPE_KEY) != 0 &&
            typeKeys.count(ORGANIZATION_CLASS_KEY) != 0 &&
            typeKeys.count(PUBLICATION_STATUS_KEY) != 0 &&
            !mappings.typeTexts.empty() &&
            !mappings.classTexts.empty() &&
            !mappings.statusTexts.empty();
    }

    bool loadMappings(
        MongoClient& client,
        OrganizationEnumLoadResult& result
    )
    {
        std::unordered_map<std::string, std::string> typeKeyById;
        std::vector<bsoncxx::oid> typeIds;

        if (!loadEnumTypes(client, typeKeyById, typeIds))
        {
            return false;
        }

        if (!typeKeyById.empty() &&
            !loadEnumValues(client, typeKeyById, typeIds, result.mappings))
        {
            return false;
        }

        result.status = hasAllRequiredGroups(
            typeKeyById,
            result.mappings
        )
            ? OrganizationEnumLoadStatus::SUCCESS
            : OrganizationEnumLoadStatus::ENUM_DATA_INCOMPLETE;

        return true;
    }
#endif
}

oatpp::String OrganizationEnumTextMap::getTypeText(
    const std::string& code
) const
{
    return findText(typeTexts, code);
}

oatpp::String OrganizationEnumTextMap::getClassText(
    const std::string& code
) const
{
    return findText(classTexts, code);
}

oatpp::String OrganizationEnumTextMap::getStatusText(
    const std::string& code
) const
{
    return findText(statusTexts, code);
}

bool OrganizationEnumTextMap::hasTypeText(const std::string& code) const
{
    return typeTexts.find(code) != typeTexts.end();
}

bool OrganizationEnumTextMap::hasClassText(const std::string& code) const
{
    return classTexts.find(code) != classTexts.end();
}

bool OrganizationEnumTextMap::hasStatusText(const std::string& code) const
{
    return statusTexts.find(code) != statusTexts.end();
}

OrganizationEnumLoadResult OrganizationEnumDictionary::load() const
{
    OrganizationEnumLoadResult result;

#ifndef USE_MONGO
    OATPP_LOGE(
        "OrganizationEnumDictionary",
        "MongoDB support is disabled at build time"
    );
#else
    if (mongoAccessState.load() == MongoAccessState::UNAVAILABLE)
    {
        return result;
    }

    try
    {
        using MongoClientPtr = std::shared_ptr<MongoClient>;

        auto* component = static_cast<MongoClientPtr*>(
            oatpp::base::Environment::getComponent(
                typeid(MongoClientPtr).name(),
                MONGO_COMPONENT_NAME
            )
        );

        if (component == nullptr || !(*component))
        {
            throw std::runtime_error(
                "missing component dept-organization-enum-mongo"
            );
        }

        if (mongoAccessState.load() == MongoAccessState::NOT_ATTEMPTED)
        {
            std::lock_guard<std::mutex> lock(firstMongoAccessMutex);

            if (mongoAccessState.load() == MongoAccessState::UNAVAILABLE)
            {
                return result;
            }

            if (mongoAccessState.load() == MongoAccessState::NOT_ATTEMPTED)
            {
                if (!loadMappings(*(*component), result))
                {
                    mongoAccessState.store(MongoAccessState::UNAVAILABLE);
                    throw std::runtime_error("MongoDB enum query failed");
                }

                mongoAccessState.store(MongoAccessState::AVAILABLE);
            }
            else if (!loadMappings(*(*component), result))
            {
                mongoAccessState.store(MongoAccessState::UNAVAILABLE);
                throw std::runtime_error("MongoDB enum query failed");
            }
        }
        else if (!loadMappings(*(*component), result))
        {
            mongoAccessState.store(MongoAccessState::UNAVAILABLE);
            throw std::runtime_error("MongoDB enum query failed");
        }
    }
    catch (const std::exception& exception)
    {
        mongoAccessState.store(MongoAccessState::UNAVAILABLE);
        OATPP_LOGE(
            "OrganizationEnumDictionary",
            "MongoDB enum lookup failed: %s",
            exception.what()
        );
    }
    catch (...)
    {
        mongoAccessState.store(MongoAccessState::UNAVAILABLE);
        OATPP_LOGE(
            "OrganizationEnumDictionary",
            "MongoDB enum lookup failed with an unknown exception"
        );
    }
#endif

    return result;
}
