#include "stdafx.h"
#include "PractitionerOptionDAO.h"
#include "PractitionerOptionMapper.h"

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

    Connection* requireConnection(SqlSession* session)
    {
        if (!session || !session->getConnection())
        {
            throw std::runtime_error("Database connection is unavailable");
        }

        return session->getConnection();
    }
}

std::string PractitionerOptionDAO::selectTenantIdByUserId(
    const std::string& userId
)
{
    const std::string sql =
        "SELECT tenant_id FROM whale_user_login_session "
        "WHERE user_id = ? AND tenant_id IS NOT NULL "
        "ORDER BY updated_at DESC LIMIT 1";

    ScopedPreparedStatement statement(
        requireConnection(sqlSession)->prepareStatement(sql)
    );
    statement->setString(1, userId);

    ScopedResultSet resultSet(statement->executeQuery());
    if (!resultSet->next() || resultSet->isNull("tenant_id"))
    {
        return "";
    }

    return resultSet->getString("tenant_id");
}

std::list<PractitionerOptionDTO::Wrapper>
PractitionerOptionDAO::selectDoctorOptions(const std::string& tenantId)
{
    const std::string sql =
        "SELECT DISTINCT p.id, p.name, p.status_enum "
        "FROM adm_practitioner p "
        "INNER JOIN adm_pr_assign a "
        "ON a.practitioner_id = p.id "
        "INNER JOIN whale_roles r "
        "ON r.id = a.role_id "
        "WHERE p.tenant_id = ? "
        "AND p.is_deleted = 0 "
        "AND p.active_flag = 1 "
        "AND a.tenant_id = ? "
        "AND a.is_deleted = 0 "
        "AND a.active_flag = 1 "
        "AND r.code = 'ROLE_DOCTOR' "
        "AND r.is_deleted = 0 "
        "ORDER BY p.name ASC, p.id ASC";

    ScopedPreparedStatement statement(
        requireConnection(sqlSession)->prepareStatement(sql)
    );
    statement->setString(1, tenantId);
    statement->setString(2, tenantId);

    ScopedResultSet resultSet(statement->executeQuery());
    std::list<PractitionerOptionDTO::Wrapper> options;
    PractitionerOptionMapper mapper;

    while (resultSet->next())
    {
        options.push_back(mapper.mapper(resultSet.get()));
    }

    return options;
}
