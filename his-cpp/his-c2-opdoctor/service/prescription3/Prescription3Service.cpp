#include "stdafx.h"
#include "Prescription3Service.h"

#include "id/SnowFlake.h"

#include <algorithm>
#include <cctype>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

namespace
{
class TransactionGuard
{
public:
    explicit TransactionGuard(sql::Connection& connection)
        : connection_(connection),
          previousAutoCommit_(connection.getAutoCommit())
    {
        connection_.setAutoCommit(false);
    }

    ~TransactionGuard()
    {
        if (!completed_)
        {
            try
            {
                connection_.rollback();
            }
            catch (...)
            {
            }
        }

        try
        {
            connection_.setAutoCommit(previousAutoCommit_);
        }
        catch (...)
        {
        }
    }

    void commit()
    {
        connection_.commit();
        completed_ = true;
    }

private:
    sql::Connection& connection_;
    bool previousAutoCommit_;
    bool completed_ = false;
};

std::string trim(const std::string& value)
{
    const auto first = std::find_if_not(
        value.begin(), value.end(),
        [](unsigned char character) { return std::isspace(character); });
    const auto last = std::find_if_not(
        value.rbegin(), value.rend(),
        [](unsigned char character) { return std::isspace(character); }).base();
    return first < last ? std::string(first, last) : std::string();
}

std::string upper(std::string value)
{
    std::transform(
        value.begin(), value.end(), value.begin(),
        [](unsigned char character)
        {
            return static_cast<char>(std::toupper(character));
        });
    return value;
}

Prescription3OrderType parseType(const std::string& value)
{
    const auto normalized = upper(trim(value));
    if (normalized == "MEDICATION")
    {
        return Prescription3OrderType::Medication;
    }
    if (normalized == "SERVICE")
    {
        return Prescription3OrderType::Service;
    }
    if (normalized == "DEVICE")
    {
        return Prescription3OrderType::Device;
    }
    throw Prescription3BusinessException(
        PRESCRIPTION3_INVALID_REQUEST,
        "Order type must be MEDICATION, SERVICE, or DEVICE");
}

bool sameScope(
    const Prescription3OrderDO& left,
    const Prescription3OrderDO& right)
{
    return left.tenantId == right.tenantId &&
           left.patientId == right.patientId &&
           left.encounterId == right.encounterId &&
           left.requesterId == right.requesterId &&
           left.orgId == right.orgId &&
           left.locationId == right.locationId;
}

std::vector<std::string> collectGroupIds(
    const std::vector<Prescription3OrderDO>& rows)
{
    std::set<std::string> unique;
    for (const auto& row : rows)
    {
        if (!row.groupId.empty())
        {
            unique.insert(row.groupId);
        }
    }
    return {unique.begin(), unique.end()};
}

std::vector<Prescription3OrderDO> unionRows(
    const std::vector<Prescription3OrderDO>& first,
    const std::vector<Prescription3OrderDO>& second)
{
    std::vector<Prescription3OrderDO> result;
    std::unordered_set<std::string> seen;
    result.reserve(first.size() + second.size());

    for (const auto* source : {&first, &second})
    {
        for (const auto& row : *source)
        {
            if (seen.insert(row.key()).second)
            {
                result.push_back(row);
            }
        }
    }
    return result;
}

sql::Connection& serviceConnection(Prescription3DAO& dao)
{
    auto* session = dao.getSqlSession();
    if (session == nullptr || session->getConnection() == nullptr)
    {
        throw std::runtime_error("Database connection is unavailable");
    }
    return *session->getConnection();
}
}

std::vector<Prescription3OrderDO>
Prescription3Service::extractOrders(
    const oatpp::List<
        oatpp::Object<Prescription3OrderItemDTO>>& orders)
{
    if (!orders || orders->empty())
    {
        throw Prescription3BusinessException(
            PRESCRIPTION3_INVALID_REQUEST,
            "At least one order is required");
    }
    if (orders->size() > MAX_BATCH_SIZE)
    {
        throw Prescription3BusinessException(
            PRESCRIPTION3_INVALID_REQUEST,
            "A request can contain at most 200 orders");
    }

    std::vector<Prescription3OrderDO> result;
    std::unordered_set<std::string> seen;
    result.reserve(orders->size());

    for (const auto& item : *orders)
    {
        if (!item || !item->id || !item->type)
        {
            throw Prescription3BusinessException(
                PRESCRIPTION3_INVALID_REQUEST,
                "Every order requires id and type");
        }

        Prescription3OrderDO order;
        order.id = trim(*item->id);
        order.type = parseType(*item->type);
        if (order.id.empty() || order.id.size() > 64)
        {
            throw Prescription3BusinessException(
                PRESCRIPTION3_INVALID_REQUEST,
                "Order id must contain between 1 and 64 characters");
        }

        if (seen.insert(order.key()).second)
        {
            result.push_back(order);
        }
    }

    if (result.empty())
    {
        throw Prescription3BusinessException(
            PRESCRIPTION3_INVALID_REQUEST,
            "At least one distinct order is required");
    }
    return result;
}

void Prescription3Service::validateOperator(
    const std::string& operatorId)
{
    if (operatorId.empty())
    {
        throw Prescription3BusinessException(
            PRESCRIPTION3_INVALID_REQUEST,
            "The authenticated user id is missing");
    }
}

void Prescription3Service::validateRowsExist(
    const std::vector<Prescription3OrderDO>& rows,
    const std::vector<Prescription3OrderDO>& requested)
{
    std::unordered_set<std::string> found;
    for (const auto& row : rows)
    {
        found.insert(row.key());
    }
    for (const auto& order : requested)
    {
        if (found.find(order.key()) == found.end())
        {
            throw Prescription3BusinessException(
                PRESCRIPTION3_NOT_FOUND,
                "One or more selected orders do not exist");
        }
    }
}

void Prescription3Service::validateDraft(
    const std::vector<Prescription3OrderDO>& rows)
{
    for (const auto& row : rows)
    {
        if (row.isDeleted != 0)
        {
            throw Prescription3BusinessException(
                PRESCRIPTION3_NOT_FOUND,
                "One or more selected orders have been deleted");
        }
        if (row.status != "DRAFT")
        {
            throw Prescription3BusinessException(
                PRESCRIPTION3_INVALID_STATUS,
                "Only DRAFT orders can be changed");
        }
    }
}

void Prescription3Service::validateSameScope(
    const std::vector<Prescription3OrderDO>& rows)
{
    if (rows.empty())
    {
        throw Prescription3BusinessException(
            PRESCRIPTION3_NOT_FOUND,
            "No active order was found");
    }
    const auto& first = rows.front();
    for (std::size_t index = 1; index < rows.size(); ++index)
    {
        if (!sameScope(first, rows[index]))
        {
            throw Prescription3BusinessException(
                PRESCRIPTION3_SCOPE_MISMATCH,
                "Orders must have the same tenant, patient, encounter, "
                "requester, organization, and department");
        }
    }
}

void Prescription3Service::validateGroupScopes(
    const std::vector<Prescription3OrderDO>& rows)
{
    std::map<std::string, std::vector<Prescription3OrderDO>> groups;
    for (const auto& row : rows)
    {
        if (!row.groupId.empty())
        {
            groups[row.groupId].push_back(row);
        }
    }
    for (const auto& entry : groups)
    {
        validateSameScope(entry.second);
    }
}

std::vector<Prescription3OrderDO>
Prescription3Service::expandGroups(
    Prescription3DAO& dao,
    const std::vector<Prescription3OrderDO>& selected)
{
    const auto groupIds = collectGroupIds(selected);
    if (groupIds.empty())
    {
        return selected;
    }

    const auto groupRows = dao.selectByGroupIdsForUpdate(groupIds);
    std::set<std::string> foundGroups;
    for (const auto& row : groupRows)
    {
        foundGroups.insert(row.groupId);
    }
    for (const auto& groupId : groupIds)
    {
        if (foundGroups.find(groupId) == foundGroups.end())
        {
            throw Prescription3BusinessException(
                PRESCRIPTION3_INVALID_GROUP,
                "The complete selected group could not be loaded");
        }
    }
    return unionRows(selected, groupRows);
}

std::string Prescription3Service::generateGroupId()
{
    const auto value = SnowFlake::getInstance()->nextId();
    if (value == 0)
    {
        throw std::runtime_error("Failed to generate a group id");
    }
    return std::to_string(value);
}

Prescription3OperationResult Prescription3Service::batchDelete(
    const Prescription3BatchDeleteDTO::Wrapper& dto,
    const std::string& operatorId)
{
    validateOperator(operatorId);
    if (!dto)
    {
        throw Prescription3BusinessException(
            PRESCRIPTION3_INVALID_REQUEST,
            "Request body is required");
    }

    const auto requested = extractOrders(dto->orders);
    TransactionGuard transaction(serviceConnection(dao_));
    const auto selected = dao_.selectByOrdersForUpdate(requested);
    validateRowsExist(selected, requested);
    validateDraft(selected);

    const auto targets = expandGroups(dao_, selected);
    validateDraft(targets);
    validateGroupScopes(targets);

    const int affected = dao_.softDelete(targets, operatorId);
    if (affected != static_cast<int>(targets.size()))
    {
        throw Prescription3BusinessException(
            PRESCRIPTION3_CONCURRENT_CHANGE,
            "Order data changed while the delete was running");
    }

    transaction.commit();
    return {affected, ""};
}

Prescription3OperationResult Prescription3Service::mergeGroup(
    const Prescription3MergeGroupDTO::Wrapper& dto,
    const std::string& operatorId)
{
    validateOperator(operatorId);
    if (!dto)
    {
        throw Prescription3BusinessException(
            PRESCRIPTION3_INVALID_REQUEST,
            "Request body is required");
    }

    const auto requested = extractOrders(dto->orders);
    TransactionGuard transaction(serviceConnection(dao_));
    const auto selected = dao_.selectByOrdersForUpdate(requested);
    validateRowsExist(selected, requested);
    validateDraft(selected);

    std::set<std::string> groupUnits;
    std::set<std::string> singleUnits;
    for (const auto& row : selected)
    {
        if (row.groupId.empty())
        {
            singleUnits.insert(row.key());
        }
        else
        {
            groupUnits.insert(row.groupId);
        }
    }
    if (groupUnits.size() + singleUnits.size() < 2)
    {
        throw Prescription3BusinessException(
            PRESCRIPTION3_INVALID_REQUEST,
            "Merging requires at least two distinct groups or orders");
    }

    const auto targets = expandGroups(dao_, selected);
    validateDraft(targets);
    validateSameScope(targets);

    const auto groupId = generateGroupId();
    const int affected = dao_.assignGroup(
        targets, groupId, operatorId);
    if (affected != static_cast<int>(targets.size()))
    {
        throw Prescription3BusinessException(
            PRESCRIPTION3_CONCURRENT_CHANGE,
            "Order data changed while the merge was running");
    }

    transaction.commit();
    return {affected, groupId};
}

Prescription3OperationResult Prescription3Service::splitGroup(
    const Prescription3SplitGroupDTO::Wrapper& dto,
    const std::string& operatorId)
{
    validateOperator(operatorId);
    if (!dto)
    {
        throw Prescription3BusinessException(
            PRESCRIPTION3_INVALID_REQUEST,
            "Request body is required");
    }

    const auto requested = extractOrders(dto->orders);
    TransactionGuard transaction(serviceConnection(dao_));
    const auto selected = dao_.selectByOrdersForUpdate(requested);
    validateRowsExist(selected, requested);
    validateDraft(selected);

    for (const auto& row : selected)
    {
        if (row.groupId.empty())
        {
            throw Prescription3BusinessException(
                PRESCRIPTION3_INVALID_GROUP,
                "Only grouped orders can be split");
        }
    }

    const auto targets = expandGroups(dao_, selected);
    validateDraft(targets);
    validateGroupScopes(targets);

    std::map<std::string, int> groupSizes;
    for (const auto& row : targets)
    {
        if (!row.groupId.empty())
        {
            ++groupSizes[row.groupId];
        }
    }
    for (const auto& entry : groupSizes)
    {
        if (entry.second < 2)
        {
            throw Prescription3BusinessException(
                PRESCRIPTION3_INVALID_GROUP,
                "A valid merged group must contain at least two orders");
        }
    }

    const int affected = dao_.clearGroups(targets, operatorId);
    if (affected != static_cast<int>(targets.size()))
    {
        throw Prescription3BusinessException(
            PRESCRIPTION3_CONCURRENT_CHANGE,
            "Order data changed while the split was running");
    }

    transaction.commit();
    return {affected, ""};
}
