// dao/order/EnumDAO.h
#pragma once
#ifndef _ENUM_DAO_H_
#define _ENUM_DAO_H_

#include "MongoClient.h"
#include <string>
#include <vector>

/**
 * 枚举值结构体
 */
struct EnumValue {
    std::string code;
    std::string name;
    int sortOrder;
    bool isDefault;
    bool active;

    EnumValue() : sortOrder(0), isDefault(false), active(true) {}
};

/**
 * MongoDB 枚举数据访问层
 */
class EnumDAO {
private:
    std::shared_ptr<MongoClient> m_mongoClient;
    static constexpr const char* COLLECTION_NAME = "enum_definitions";

public:
    explicit EnumDAO(std::shared_ptr<MongoClient> client);
    ~EnumDAO() = default;

    /**
     * 根据枚举类型获取所有枚举值
     */
    std::vector<EnumValue> getEnumValues(const std::string& enumType);

    /**
     * 根据枚举类型和编码获取单个枚举值
     * @param out 输出参数，存储查询结果
     * @return true 表示找到，false 表示未找到
     */
    bool getEnumValue(const std::string& enumType, const std::string& code, EnumValue& out);

    /**
     * 获取枚举值名称（用于前端展示）
     */
    std::string getEnumName(const std::string& enumType, const std::string& code);

    /**
     * 获取默认枚举值
     * @param out 输出参数，存储查询结果
     * @return true 表示找到，false 表示未找到
     */
    bool getDefaultEnumValue(const std::string& enumType, EnumValue& out);

    /**
     * 确保集合存在并创建索引
     */
    bool ensureCollectionAndIndexes();
};

#endif // _ENUM_DAO_H_