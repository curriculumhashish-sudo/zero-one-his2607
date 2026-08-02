#pragma once

#include "domain/query/enroll/EnrollQuery.h"
#include "domain/dto/enroll/EnrollDTO.h"

/**
 * @brief 住院登记业务服务类。
 *
 * 负责组织住院登记列表与详情的业务流程，
 * 将 DAO 返回的数据库对象转换为接口 DTO。
 * Controller 不直接访问 DAO。
 */
class EnrollService
{
private:
    /**
     * @brief 将住院登记状态代码转换为页面展示文字。
     *
     * @param statusEnum 数据库中的住院登记状态代码。
     * @return 对应的展示文字；未知代码时返回原始代码。
     *
     * @details
     * 数据库存储状态代码，中文展示文字从 QQQQ 字典读取，
     * 避免在多个接口重复编写状态转换逻辑。
     */
    std::string getStatusEnumText(const std::string& statusEnum);

    /**
     * @brief 将性别代码转换为页面展示文字。
     *
     * @param genderEnum 数据库中的性别代码，例如 MALE、FEMALE。
     * @return 对应的展示文字；未知代码时返回原始代码。
     *
     * @details
     * 数据库存储稳定的代码，接口展示文字从 QQQQ 字典中读取，
     * 避免在 C++ 代码中直接写中文。
     */
    std::string getGenderEnumText(const std::string& genderEnum);

    /**
     * @brief 根据出生日期计算当前年龄。
     *
     * @param birthDate 出生日期，格式为 YYYY-MM-DD。
     * @return 已满周岁的年龄；日期为空或格式无效时返回 0。
     *
     * @details
     * 年龄会随日期变化，不应作为固定值存入数据库，
     * 因此由 Service 在返回详情时实时计算。
     */
    int calculateAge(const std::string& birthDate);

public:
    /**
     * @brief 分页查询住院登记列表。
     *
     * @param query 前端传入的列表筛选与分页条件。
     * @return 包含分页信息和住院登记列表的分页对象。
     *
     * @details
     * 查询总数与当前页数据，并将 DAO 返回的列表 DO
     * 转换为前端接口需要的列表 DTO。
     */
    EnrollPageDTO::Wrapper queryAll(const EnrollQuery::Wrapper& query);

    /**
     * @brief 根据住院登记主键查询住院登记详情。
     *
     * @param encounterId 住院登记主键，对应 adm_encounter.id。
     * @return 查询到时返回详情 DTO；未查询到时返回 nullptr。
     *
     * @details
     * 本方法调用 DAO 获取数据库原始数据，再完成状态、性别、
     * 年龄等面向接口的数据转换。
     */
    EnrollDetailDTO::Wrapper queryById(const std::string& encounterId);
};