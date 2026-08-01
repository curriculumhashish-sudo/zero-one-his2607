#pragma once
#ifndef _PRINT_REGISTER_RECEIPT_MAPPER_
#define _PRINT_REGISTER_RECEIPT_MAPPER_

#include "Mapper.h"

/**
 * 打印挂号回执 Mapper
 * 用途：将 SQL 查询结果的第一列映射为 std::string
 * 适用场景：SELECT binding_code FROM ... 或 SELECT print_json FROM ...
 */
class PrintRegisterReceiptMapper : public Mapper<std::string> {
public:
    std::string mapper(ResultSet* resultSet) const override {
        // 使用索引 1 获取第一列数据
        // 这样做的好处是：无论是查 binding_code 还是 print_json，只要是单列查询都通用
        return resultSet->getString(1);
    }
};

#endif // !_PRINT_REGISTER_RECEIPT_MAPPER_
