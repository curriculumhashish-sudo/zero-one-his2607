#include "stdafx.h"
#include "DeptService.h"

#include "SimpleDateTimeFormat.h"
#include "TransactionManager.h"
#include "dao/dept/DeptDAO.h"
#include "domain/dept/DeptStatus.h"
#include "id/SnowFlake.h"

namespace
{
    DeptOperationResult successResult(const std::string& id)
    {
        return DeptOperationResult(true, id, RS_SUCCESS, 200);
    }

    DeptOperationResult failResult(const ResultStatus& status, int httpStatus)
    {
        return DeptOperationResult(false, "", status, httpStatus);
    }

    std::string valueOrEmpty(const oatpp::String& value)
    {
        return value ? value->c_str() : "";
    }

    std::string effectiveString(
        const oatpp::String& patchValue,
        const std::string& currentValue)
    {
        return patchValue ? patchValue->c_str() : currentValue;
    }
}

DeptOperationResult DeptService::add(
    const DeptAddDTO::Wrapper& dto)
{
    DeptDAO dao;
    TransactionManager transaction(&dao);

    try
    {
        const PayloadDTO* payload = dto->getPayload();
        if (!payload)
        {
            transaction.rollback();
            return failResult(DeptStatus::forbidden(), 403);
        }
        const std::string userId = payload->getId();
        const std::string tenantId = dao.selectTenantIdByUserId(userId);
        if (tenantId.empty())
        {
            transaction.rollback();
            return failResult(DeptStatus::forbidden(), 403);
        }

        const std::string name = valueOrEmpty(dto->name);
        if (dao.existsName(tenantId, name))
        {
            transaction.rollback();
            return failResult(DeptStatus::duplicateName(), 409);
        }

        const std::string typeEnum = valueOrEmpty(dto->typeEnum);
        const std::string parentId = valueOrEmpty(dto->parentId);
        if (typeEnum == DeptEnum::DEPARTMENT)
        {
            auto parent = dao.selectByIdForUpdate(parentId);
            if (!parent || parent->getTenantId() != tenantId ||
                parent->getTypeEnum() != DeptEnum::HOSPITAL)
            {
                transaction.rollback();
                return failResult(DeptStatus::invalidParent(), 400);
            }
        }

        const std::string id =
            std::to_string(SnowFlake::getInstance()->nextId());
        DeptDO data;
        data.setId(id);
        data.setTenantId(tenantId);
        data.setCreatedAt(SimpleDateTimeFormat::format());
        data.setCreatedBy(userId);
        data.setBusNo("ORG" + id);
        ZO_STAR_DOMAIN_DTO_TO_DO(
            data,
            dto,
            Name, name,
            ParentId, parentId,
            TypeEnum, typeEnum,
            ClassEnum, classEnum,
            StatusEnum, statusEnum,
            DisplayOrder, displayOrder,
            DefDoctorId, defDoctorId,
            PyStr, pyStr,
            WbStr, wbStr,
            YbNo, ybNo,
            YbName, ybName,
            MedinsId, medinsId,
            MedinsAdmdvs, medinsAdmdvs,
            MedinsType, medinsType,
            MedinsLv, medinsLv,
            Caty, caty);

        if (dao.insertDept(data) != 1)
        {
            transaction.rollback();
            return failResult(DeptStatus::databaseFailure(), 500);
        }

        transaction.commit();
        return successResult(id);
    }
    catch (...)
    {
        transaction.rollback();
        return failResult(DeptStatus::databaseFailure(), 500);
    }
}

DeptOperationResult DeptService::modify(
    const DeptModifyDTO::Wrapper& dto)
{
    DeptDAO dao;
    TransactionManager transaction(&dao);

    try
    {
        const PayloadDTO* payload = dto->getPayload();
        if (!payload)
        {
            transaction.rollback();
            return failResult(DeptStatus::forbidden(), 403);
        }
        const std::string userId = payload->getId();
        const std::string tenantId = dao.selectTenantIdByUserId(userId);
        if (tenantId.empty())
        {
            transaction.rollback();
            return failResult(DeptStatus::forbidden(), 403);
        }

        const std::string id = valueOrEmpty(dto->id);
        auto current = dao.selectByIdForUpdate(id);
        if (!current)
        {
            transaction.rollback();
            return failResult(DeptStatus::notFound(), 404);
        }
        if (current->getTenantId() != tenantId)
        {
            transaction.rollback();
            return failResult(DeptStatus::forbidden(), 403);
        }

        const std::string name = effectiveString(dto->name, current->getName());
        const std::string typeEnum = effectiveString(dto->typeEnum, current->getTypeEnum());
        std::string classEnum = effectiveString(dto->classEnum, current->getClassEnum());
        std::string parentId = effectiveString(dto->parentId, current->getParentId());
        const std::string doctorId = effectiveString(
            dto->defDoctorId, current->getDefDoctorId());

        if (dao.existsName(tenantId, name, id))
        {
            transaction.rollback();
            return failResult(DeptStatus::duplicateName(), 409);
        }

        bool clearOrganizationFields = false;
        if (typeEnum == DeptEnum::HOSPITAL)
        {
            if (current->getTypeEnum() == DeptEnum::HOSPITAL &&
                ((dto->classEnum && !classEnum.empty()) ||
                    (dto->parentId && !parentId.empty())))
            {
                transaction.rollback();
                return failResult(DeptStatus::invalid(
                    "HOSPITAL must not specify classEnum or parentId."), 400);
            }

            clearOrganizationFields = true;
            classEnum.clear();
            parentId.clear();
        }
        else
        {
            if (!DeptEnum::isClass(classEnum) || parentId.empty())
            {
                transaction.rollback();
                return failResult(DeptStatus::invalid(
                    "DEPARTMENT requires classEnum and parentId."), 400);
            }
            if (parentId == id)
            {
                transaction.rollback();
                return failResult(DeptStatus::invalidParent(), 400);
            }

            auto parent = dao.selectByIdForUpdate(parentId);
            if (!parent || parent->getTenantId() != tenantId ||
                parent->getTypeEnum() != DeptEnum::HOSPITAL)
            {
                transaction.rollback();
                return failResult(DeptStatus::invalidParent(), 400);
            }
        }

        if (current->getTypeEnum() == DeptEnum::HOSPITAL &&
            typeEnum == DeptEnum::DEPARTMENT && dao.hasChildren(id))
        {
            transaction.rollback();
            return failResult(DeptStatus::hasChildren(), 409);
        }

        if (dto->defDoctorId && !doctorId.empty() &&
            !dao.existsDoctor(tenantId, doctorId))
        {
            transaction.rollback();
            return failResult(DeptStatus::doctorNotFound(), 400);
        }

        if (dao.updatePatch(*dto.get(), userId, clearOrganizationFields) != 1)
        {
            transaction.rollback();
            return failResult(DeptStatus::updateConflict(), 409);
        }

        transaction.commit();
        return successResult(id);
    }
    catch (...)
    {
        transaction.rollback();
        return failResult(DeptStatus::databaseFailure(), 500);
    }
}

DeptOperationResult DeptService::remove(
    const DeptDeleteQuery::Wrapper& query)
{
    DeptDAO dao;
    TransactionManager transaction(&dao);

    try
    {
        const PayloadDTO* payload = query->getPayload();
        if (!payload)
        {
            transaction.rollback();
            return failResult(DeptStatus::forbidden(), 403);
        }
        const std::string id = valueOrEmpty(query->id);
        const std::string userId = payload->getId();
        const std::string tenantId = dao.selectTenantIdByUserId(userId);
        if (tenantId.empty())
        {
            transaction.rollback();
            return failResult(DeptStatus::forbidden(), 403);
        }

        auto current = dao.selectByIdForUpdate(id);
        if (!current)
        {
            transaction.rollback();
            return failResult(DeptStatus::notFound(), 404);
        }
        if (current->getTenantId() != tenantId)
        {
            transaction.rollback();
            return failResult(DeptStatus::forbidden(), 403);
        }
        if (current->getStatusEnum() != DeptEnum::STATUS_DISABLED)
        {
            transaction.rollback();
            return failResult(DeptStatus::enabledDeleteDenied(), 409);
        }
        if (current->getTypeEnum() == DeptEnum::HOSPITAL && dao.hasChildren(id))
        {
            transaction.rollback();
            return failResult(DeptStatus::hasChildren(), 409);
        }
        if (dao.hasBusinessReferences(id))
        {
            transaction.rollback();
            return failResult(DeptStatus::businessReferenced(), 409);
        }
        if (dao.logicalDelete(id, tenantId, userId) != 1)
        {
            transaction.rollback();
            return failResult(DeptStatus::updateConflict(), 409);
        }

        transaction.commit();
        return successResult(id);
    }
    catch (...)
    {
        transaction.rollback();
        return failResult(DeptStatus::databaseFailure(), 500);
    }
}

DeptOperationResult DeptService::batchStatus(
    const DeptBatchStatusDTO::Wrapper& dto)
{
    DeptDAO dao;
    TransactionManager transaction(&dao);

    try
    {
        const PayloadDTO* payload = dto->getPayload();
        if (!payload)
        {
            transaction.rollback();
            return failResult(DeptStatus::forbidden(), 403);
        }
        std::vector<std::string> ids;
        ids.reserve(dto->deptIdList->size());
        for (const auto& deptId : *dto->deptIdList)
            ids.emplace_back(deptId->c_str());
        const int targetStatus = dto->targetStatus.getValue(-1);
        const std::string userId = payload->getId();
        const std::string tenantId = dao.selectTenantIdByUserId(userId);
        if (tenantId.empty())
        {
            transaction.rollback();
            return failResult(DeptStatus::forbidden(), 403);
        }

        for (const auto& id : ids)
        {
            auto current = dao.selectByIdForUpdate(id);
            if (!current)
            {
                transaction.rollback();
                return failResult(DeptStatus::notFound(), 404);
            }
            if (current->getTenantId() != tenantId)
            {
                transaction.rollback();
                return failResult(DeptStatus::forbidden(), 403);
            }
        }

        const std::string statusEnum = targetStatus == 1
            ? DeptEnum::STATUS_ENABLED
            : DeptEnum::STATUS_DISABLED;
        if (dao.updateBatchStatus(ids, tenantId, userId, statusEnum) !=
            static_cast<int>(ids.size()))
        {
            transaction.rollback();
            return failResult(DeptStatus::updateConflict(), 409);
        }

        transaction.commit();
        return successResult("");
    }
    catch (...)
    {
        transaction.rollback();
        return failResult(DeptStatus::databaseFailure(), 500);
    }
}
