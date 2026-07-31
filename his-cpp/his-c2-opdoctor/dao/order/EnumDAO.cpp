// dao/order/EnumDAO.cpp
#include "stdafx.h"
#include "EnumDAO.h"
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>

EnumDAO::EnumDAO(std::shared_ptr<MongoClient> client)
    : m_mongoClient(client) {
}

// ========== 辅助函数：安全获取 int 值（兼容 int32 / int64 / double） ==========
static int getIntValue(bsoncxx::document::element elem) {
    if (!elem) {
        return 0;
    }

    bsoncxx::type t = elem.type();
    if (t == bsoncxx::type::k_int32) {
        return elem.get_int32().value;
    }
    else if (t == bsoncxx::type::k_int64) {
        return static_cast<int>(elem.get_int64().value);
    }
    else if (t == bsoncxx::type::k_double) {
        return static_cast<int>(elem.get_double().value);
    }
    return 0;
}

// ========== 辅助函数：安全获取 bool 值 ==========
static bool getBoolValue(bsoncxx::document::element elem) {
    if (!elem) {
        return false;
    }

    bsoncxx::type t = elem.type();
    if (t == bsoncxx::type::k_bool) {
        return elem.get_bool().value;
    }
    return false;
}

// ========== 辅助函数：安全获取字符串值 ==========
static std::string getStringValue(bsoncxx::document::element elem) {
    if (!elem) {
        return "";
    }

    bsoncxx::type t = elem.type();
    if (t == bsoncxx::type::k_string) {
        auto sv = elem.get_string().value;
        return std::string(sv.data(), sv.size());
    }
    return "";
}

// ========== 获取所有枚举值 ==========
std::vector<EnumValue> EnumDAO::getEnumValues(const std::string& enumType) {
    std::vector<EnumValue> result;

    if (!m_mongoClient) {
        std::cerr << "EnumDAO: MongoClient is null" << std::endl;
        return result;
    }

    using bsoncxx::builder::stream::document;
    using bsoncxx::builder::stream::finalize;

    auto filter = document{}
        << "enumType" << enumType
        << "active" << true
        << finalize;

    auto sort = document{} << "sortOrder" << 1 << finalize;

    m_mongoClient->execute(COLLECTION_NAME,
        [&](mongocxx::collection* collection) {
            try {
                mongocxx::options::find opts;
                opts.sort(sort.view());
                auto cursor = collection->find(filter.view(), opts);

                for (auto&& doc : cursor) {
                    EnumValue val;
                    // 关键修复：doc 本身就是 document::view，直接使用，不调用 .view()
                    // 不能写 auto docView = doc.view(); 

                    val.code = getStringValue(doc["code"]);
                    val.name = getStringValue(doc["name"]);
                    val.sortOrder = getIntValue(doc["sortOrder"]);
                    val.isDefault = getBoolValue(doc["isDefault"]);
                    val.active = getBoolValue(doc["active"]);

                    result.push_back(val);
                }
            }
            catch (const std::exception& e) {
                std::cerr << "EnumDAO::getEnumValues failed: " << e.what() << std::endl;
            }
        }
    );

    return result;
}

// ========== 根据类型和编码获取单个枚举值 ==========
bool EnumDAO::getEnumValue(const std::string& enumType, const std::string& code, EnumValue& out) {
    if (!m_mongoClient) {
        std::cerr << "EnumDAO: MongoClient is null" << std::endl;
        return false;
    }

    if (code.empty()) {
        return false;
    }

    using bsoncxx::builder::stream::document;
    using bsoncxx::builder::stream::finalize;

    auto filter = document{}
        << "enumType" << enumType
        << "code" << code
        << "active" << true
        << finalize;

    bool found = false;

    m_mongoClient->execute(COLLECTION_NAME,
        [&](mongocxx::collection* collection) {
            try {
                auto doc = collection->find_one(filter.view());
                if (doc) {
                    // find_one 返回 optional<document::value>，需要用 ->view()
                    auto docView = doc->view();  // 这里需要 .view()

                    out.code = getStringValue(docView["code"]);
                    out.name = getStringValue(docView["name"]);
                    out.sortOrder = getIntValue(docView["sortOrder"]);
                    out.isDefault = getBoolValue(docView["isDefault"]);
                    out.active = getBoolValue(docView["active"]);

                    found = true;
                }
            }
            catch (const std::exception& e) {
                std::cerr << "EnumDAO::getEnumValue failed: " << e.what() << std::endl;
            }
        }
    );

    return found;
}

// ========== 获取枚举名称 ==========
std::string EnumDAO::getEnumName(const std::string& enumType, const std::string& code) {
    EnumValue val;
    if (getEnumValue(enumType, code, val)) {
        return val.name;
    }
    return code;
}

// ========== 获取默认枚举值 ==========
bool EnumDAO::getDefaultEnumValue(const std::string& enumType, EnumValue& out) {
    if (!m_mongoClient) {
        std::cerr << "EnumDAO: MongoClient is null" << std::endl;
        return false;
    }

    using bsoncxx::builder::stream::document;
    using bsoncxx::builder::stream::finalize;

    auto filter = document{}
        << "enumType" << enumType
        << "isDefault" << true
        << "active" << true
        << finalize;

    auto sort = document{} << "sortOrder" << 1 << finalize;

    bool found = false;

    m_mongoClient->execute(COLLECTION_NAME,
        [&](mongocxx::collection* collection) {
            try {
                mongocxx::options::find opts;
                opts.sort(sort.view());
                auto doc = collection->find_one(filter.view(), opts);
                if (doc) {
                    // find_one 返回 optional<document::value>，需要用 ->view()
                    auto docView = doc->view();  // 这里需要 .view()

                    out.code = getStringValue(docView["code"]);
                    out.name = getStringValue(docView["name"]);
                    out.sortOrder = getIntValue(docView["sortOrder"]);
                    out.isDefault = getBoolValue(docView["isDefault"]);
                    out.active = getBoolValue(docView["active"]);

                    found = true;
                }
            }
            catch (const std::exception& e) {
                std::cerr << "EnumDAO::getDefaultEnumValue failed: " << e.what() << std::endl;
            }
        }
    );

    return found;
}

// ========== 创建索引 ==========
bool EnumDAO::ensureCollectionAndIndexes() {
    if (!m_mongoClient) {
        std::cerr << "EnumDAO: MongoClient is null" << std::endl;
        return false;
    }

    return m_mongoClient->execute(COLLECTION_NAME,
        [&](mongocxx::collection* collection) {
            try {
                // 1. 唯一索引：enumType + code
                bsoncxx::builder::stream::document index1;
                index1 << "enumType" << 1 << "code" << 1;
                collection->create_index(index1.view(),
                    mongocxx::options::index{}.unique(true));

                // 2. 普通索引：enumType + isDefault + active
                bsoncxx::builder::stream::document index2;
                index2 << "enumType" << 1 << "isDefault" << 1 << "active" << 1;
                collection->create_index(index2.view());

                // 3. 普通索引：enumType + active + sortOrder
                bsoncxx::builder::stream::document index3;
                index3 << "enumType" << 1 << "active" << 1 << "sortOrder" << 1;
                collection->create_index(index3.view());

                std::cout << "[MongoDB] enum_definitions indexes created." << std::endl;
                return true;
            }
            catch (const std::exception& e) {
                std::cerr << "[MongoDB] create indexes failed: " << e.what() << std::endl;
                return false;
            }
        }
    );
}