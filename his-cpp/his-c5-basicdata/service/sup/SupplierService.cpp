#include "stdafx.h"
#include "SupplierService.h"

#include "SimpleDateTimeFormat.h"
#include "TransactionManager.h"
#include "dao/sup/SupplierDAO.h"
#include "domain/sup/SupplierStatus.h"
#include "id/SnowFlake.h"

namespace
{
    SupplierOperationResult successResult(const std::string& id)
    {
        return SupplierOperationResult(true, id, RS_SUCCESS, 200);
    }

    SupplierOperationResult failResult(const ResultStatus& status, int httpStatus)
    {
        return SupplierOperationResult(false, "", status, httpStatus);
    }

    std::string valueOrEmpty(const oatpp::String& value)
    {
        return value ? value->c_str() : "";
    }
}

SupplierOperationResult SupplierService::add(const SupplierAddDTO::Wrapper& dto)
{
    SupplierDAO dao;
    TransactionManager transaction(&dao);

    try
    {
        // Resolve the caller's identity from the request payload.
        const PayloadDTO* payload = dto->getPayload();
        if (!payload)
        {
            transaction.rollback();
            return failResult(SupplierStatus::forbidden(), 403);
        }
        const std::string userId = payload->getId();
        const std::string tenantId = dao.selectTenantIdByUserId(userId);
        if (tenantId.empty())
        {
            transaction.rollback();
            return failResult(SupplierStatus::forbidden(), 403);
        }

        // Reject duplicate names within the same tenant.
        const std::string name = valueOrEmpty(dto->name);
        if (dao.existsName(tenantId, name))
        {
            transaction.rollback();
            return failResult(SupplierStatus::duplicateName(), 409);
        }

        // Generate a snowflake id and assemble the DO with audit + defaults.
        const std::string id = std::to_string(SnowFlake::getInstance()->nextId());

        SupplierDO data;
        data.setId(id);
        data.setTenantId(tenantId);
        data.setCreatedAt(SimpleDateTimeFormat::format());
        data.setCreatedBy(userId);
        const std::string busNo =
            (dto->busNo && !dto->busNo->empty()) ? dto->busNo->c_str() : ("SUPP" + id);
        data.setBusNo(busNo);
        data.setStatusEnum("ACTIVE");

        ZO_STAR_DOMAIN_DTO_TO_DO(
            data, dto,
            Name, name,
            TypeEnum, typeEnum,
            CreditCode, creditCode,
            LegalRepresentative, legalRepresentative,
            Address, address,
            PyStr, pyStr,
            WbStr, wbStr,
            Phone, phone,
            Email, email,
            BankName, bankName,
            BankAccount, bankAccount,
            OrgId, orgId,
            DisplayOrder, displayOrder,
            Note, note);

        // py_str/wb_str are NOT NULL columns; fall back to name when empty.
        if (data.getPyStr().empty())
            data.setPyStr(name);
        if (data.getWbStr().empty())
            data.setWbStr(name);

        if (dao.insertSupplier(data) != 1)
        {
            transaction.rollback();
            return failResult(SupplierStatus::databaseFailure(), 500);
        }

        transaction.commit();
        return successResult(id);
    }
    catch (...)
    {
        transaction.rollback();
        return failResult(SupplierStatus::databaseFailure(), 500);
    }
}

SupplierOperationResult SupplierService::modify(const SupplierUpdateDTO::Wrapper& dto)
{
    SupplierDAO dao;
    TransactionManager transaction(&dao);

    try
    {
        // Resolve the caller's identity from the request payload.
        const PayloadDTO* payload = dto->getPayload();
        if (!payload)
        {
            transaction.rollback();
            return failResult(SupplierStatus::forbidden(), 403);
        }
        const std::string userId = payload->getId();
        const std::string tenantId = dao.selectTenantIdByUserId(userId);
        if (tenantId.empty())
        {
            transaction.rollback();
            return failResult(SupplierStatus::forbidden(), 403);
        }

        const std::string id = valueOrEmpty(dto->id);
        auto current = dao.selectByIdForUpdate(id);
        if (!current)
        {
            transaction.rollback();
            return failResult(SupplierStatus::notFound(), 404);
        }
        // Tenant isolation: the row must belong to the caller's tenant.
        if (current->getTenantId() != tenantId)
        {
            transaction.rollback();
            return failResult(SupplierStatus::forbidden(), 403);
        }

        // Optimistic lock: reject when the client version is stale.
        if (current->getVersion() != dto->version.getValue(-1))
        {
            transaction.rollback();
            return failResult(SupplierStatus::updateConflict(), 409);
        }

        // Name uniqueness, excluding the row being modified.
        const std::string effectiveName =
            dto->name ? dto->name->c_str() : current->getName();
        if (dao.existsName(tenantId, effectiveName, id))
        {
            transaction.rollback();
            return failResult(SupplierStatus::duplicateName(), 409);
        }

        // Normalize empty py_str/wb_str to the effective name (NOT NULL columns).
        if (dto->pyStr && dto->pyStr->empty())
            dto->pyStr = oatpp::String(effectiveName);
        if (dto->wbStr && dto->wbStr->empty())
            dto->wbStr = oatpp::String(effectiveName);

        // The UPDATE re-checks the version in WHERE as a second guard.
        if (dao.updatePatch(*dto.get(), userId, current->getVersion()) != 1)
        {
            transaction.rollback();
            return failResult(SupplierStatus::updateConflict(), 409);
        }

        transaction.commit();
        return successResult(id);
    }
    catch (...)
    {
        transaction.rollback();
        return failResult(SupplierStatus::databaseFailure(), 500);
    }
}
