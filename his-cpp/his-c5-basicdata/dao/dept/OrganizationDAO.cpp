#include "stdafx.h"
#include "OrganizationDAO.h"
#include "OrganizationMapper.h"

#include <memory>
#include <stdexcept>

namespace
{
    template<typename T>
    class SqlObjectDeleter
    {
    public:
        void operator()(T* object) const noexcept
        {
            if (!object)
            {
                return;
            }

            try
            {
                object->close();
            }
            catch (...)
            {
            }

            delete object;
        }
    };

    using ScopedPreparedStatement =
        std::unique_ptr<PreparedStatement, SqlObjectDeleter<PreparedStatement>>;
    using ScopedResultSet =
        std::unique_ptr<ResultSet, SqlObjectDeleter<ResultSet>>;

    Connection* requireConnection(SqlSession* sqlSession)
    {
        if (!sqlSession || !sqlSession->getConnection())
        {
            throw std::runtime_error("Database connection is unavailable");
        }

        return sqlSession->getConnection();
    }
}

std::list<PtrAdmOrganizationDO> OrganizationDAO::selectAllNotDeleted()
{
    std::string sql =
        "SELECT "
        "id, name, bus_no, parent_id, type_enum, class_enum, status_enum, "
        "def_doctor_id, caty, "
        "yb_no, yb_name, medins_id, medins_admdvs, medins_type, medins_lv, "
        "py_str, wb_str, display_order "
        "FROM adm_organization "
        "WHERE is_deleted = 0 "
        "ORDER BY display_order ASC, id ASC";

    ScopedPreparedStatement statement(
        requireConnection(sqlSession)->prepareStatement(sql)
    );
    ScopedResultSet resultSet(statement->executeQuery());

    std::list<PtrAdmOrganizationDO> organizations;
    OrganizationMapper mapper;

    while (resultSet->next())
    {
        organizations.push_back(mapper.mapper(resultSet.get()));
    }

    return organizations;
}

PtrAdmOrganizationDO OrganizationDAO::selectById(const std::string& id)
{
    std::string sql =
        "SELECT "
        "id, name, bus_no, parent_id, type_enum, class_enum, status_enum, "
        "def_doctor_id, caty, "
        "yb_no, yb_name, medins_id, medins_admdvs, medins_type, medins_lv, "
        "py_str, wb_str, display_order "
        "FROM adm_organization "
        "WHERE id = ? "
        "AND is_deleted = 0";

    ScopedPreparedStatement statement(
        requireConnection(sqlSession)->prepareStatement(sql)
    );
    statement->setString(1, id);

    ScopedResultSet resultSet(statement->executeQuery());

    if (!resultSet->next())
    {
        return nullptr;
    }

    OrganizationMapper mapper;
    auto organization = mapper.mapper(resultSet.get());

    if (resultSet->next())
    {
        throw std::runtime_error(
            "Organization detail query returned more than one row"
        );
    }

    return organization;
}
