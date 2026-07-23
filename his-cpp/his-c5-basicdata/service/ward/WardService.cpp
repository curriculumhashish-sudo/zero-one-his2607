#include "stdafx.h"
#include "WardService.h"
#include "../../dao/ward/WardDAO.h"
#include "domain/do/ward/WardLocationDO.h"
#include "id/SnowFlake.h"
#include "SimpleDateTimeFormat.h"
#include "TransactionManager.h"
 oatpp::List<WardNameVO::Wrapper> 
     WardService::listNames(const std::string& organizationId)
{
    WardDAO dao;

    auto rows = dao.selectNameList(organizationId);

    auto result =
        oatpp::List<WardNameVO::Wrapper>::createShared();

    for (const auto& row : rows)
    {
        auto vo = WardNameVO::createShared();

        vo->id = row.getId();
        vo->name = row.getName();

        result->push_back(vo);
    }

    return result;
}


 oatpp::List<WardItemVO::Wrapper>
     WardService::listWards(const WardListQuery::Wrapper& query)
 {
     auto rows = WardDAO().selectWardList(query);
     auto result = oatpp::List<WardItemVO::Wrapper>::createShared();

     for (const auto& row : rows)
     {
         auto vo = WardItemVO::createShared();
         vo->id = row.getId();
         vo->name = row.getName();
         vo->status = row.getStatusEnum();
         vo->displayOrder = row.getDisplayOrder();
         result->push_back(vo);
     }

     return result;
 }

 std::string WardService::addLocation(
     const WardLocationAddDTO::Wrapper& dto,
     const PayloadDTO& payload)
 {
     if (!dto)
         return "";

     const std::string validationError =
         dto->validate();

     if (!validationError.empty())
         return "";

     const std::string organizationId =
         dto->organizationId.getValue("");

     const std::string name =
         dto->name.getValue("");

     const std::string formEnum =
         dto->formEnum.getValue("");

     const std::string statusEnum =
         dto->statusEnum.getValue("ACTIVE");

     const std::string operationalEnum =
         dto->operationalEnum.getValue(
             "NOT_IN_USE"
         );

     std::string parentId;
     std::string parentBusNo;
     std::string tenantId;

     WardDAO dao;
     TransactionManager transaction(&dao);

     const auto rollbackAndFail =
         [&transaction]() -> std::string
         {
             transaction.rollback();
             return "";
         };

     try
     {
         if (formEnum == "WARD")
         {
             tenantId =
                 dao.selectOrganizationTenantIdForUpdate(
                     organizationId
                 );

             if (tenantId.empty())
                 return rollbackAndFail();
         }
         else
         {
             parentId =
                 dto->parentId.getValue("");

             auto parent =
                 dao.selectLocationByIdForUpdate(
                     parentId
                 );

             if (!parent)
                 return rollbackAndFail();

             if (parent->getOrganizationId() !=
                 organizationId)
             {
                 return rollbackAndFail();
             }

             const std::string expectedParentForm =
                 formEnum == "HOUSE"
                 ? "WARD"
                 : "HOUSE";

             if (parent->getFormEnum() !=
                 expectedParentForm)
             {
                 return rollbackAndFail();
             }

             tenantId = parent->getTenantId();
             parentBusNo = parent->getBusNo();

             if (tenantId.empty())
                 return rollbackAndFail();
         }

         if (dao.existsSiblingName(
             organizationId,
             formEnum,
             parentId,
             name))
         {
             return rollbackAndFail();
         }

         const uint64_t snowflakeId =
             SnowFlake::getInstance()->nextId();

         if (snowflakeId == 0)
             return rollbackAndFail();

         const std::string id =
             std::to_string(snowflakeId);

         const std::string busNo =
             dao.generateLocationBusNo(
                 tenantId,
                 parentBusNo
             );

         if (busNo.empty())
             return rollbackAndFail();

         WardLocationDO data;

         data.setId(id);
         data.setTenantId(tenantId);
         data.setCreatedAt(
             SimpleDateTimeFormat::format()
         );
         data.setCreatedBy(payload.getId());
         data.setIsDeleted(false);
         data.setVersion(0);

         data.setName(name);
         data.setBusNo(busNo);
         data.setStatusEnum(statusEnum);
         data.setOperationalEnum(
             operationalEnum
         );
         data.setFormEnum(formEnum);
         data.setModeEnum("INSTANCE");
         data.setTypeJson("{}");
         data.setOrganizationId(
             organizationId
         );
         data.setDisplayOrder(
             dto->displayOrder.getValue(0)
         );

         if (formEnum != "WARD")
         {
             data.setParentId(parentId);
         }

         const int rows =
             dao.insertLocation(data);

         if (rows != 1)
             return rollbackAndFail();

         transaction.commit();
         return id;
     }
     catch (...)
     {
         transaction.rollback();
         return "";
     }
 }
// std::string WardService::addLocation(
//     const WardLocationAddDTO::Wrapper& dto,
//     const PayloadDTO& payload)
// {
//     WardDAO dao;
//
//     const std::string organizationId =
//         dto->organizationId.getValue("");
//
//     const std::string formEnum =
//         dto->formEnum.getValue("");
//
//     const std::string tenantId =
//         dao.selectTenantIdByOrganizationId(
//             organizationId
//         );
//
//     if (tenantId.empty())
//         return "";
//
//     std::string parentId;
//     std::string parentBusNo;
//
//     if (formEnum != "WARD")
//     {
//         parentId = dto->parentId.getValue("");
//
//       const std::string parentId =
//    dto->parentId.getValue("");
//
//auto parent =
//    dao.selectLocationById(parentId);
//
//         if (!parent)
//             return "";
//         if (parent->getOrganizationId() !=
//             organizationId)
//         {
//             return "";
//         }
//
//         if (parent->getTenantId() != tenantId)
//         {
//             return "";
//         }
//         const std::string expectedParentForm =
//             formEnum == "HOUSE"
//             ? "WARD"
//             : "HOUSE";
//
//         if (parent->getFormEnum() !=
//             expectedParentForm)
//         {
//             return "";
//         }
//
//         if (parent->getTenantId() != tenantId)
//             return "";
//
//         parentBusNo = parent->getBusNo();
//     }
//
//     const std::string busNo =
//         dao.generateLocationBusNo(
//             tenantId,
//             parentBusNo
//         );
//
//     if (busNo.empty())
//         return "";
//
//     const uint64_t snowflakeId =
//         SnowFlake::getInstance()->nextId();
//
//     if (snowflakeId == 0)
//         return "";
//
//     const std::string id =
//         std::to_string(snowflakeId);
//
//     WardLocationDO data;
//
//     data.setId(id);
//     data.setTenantId(tenantId);
//     data.setCreatedAt(
//         SimpleDateTimeFormat::format()
//     );
//     data.setCreatedBy(payload.getId());
//     data.setIsDeleted(false);
//     data.setVersion(0);
//
//     data.setName(dto->name.getValue(""));
//     data.setBusNo(busNo);
//     data.setStatusEnum(
//         dto->statusEnum.getValue("ACTIVE")
//     );
//     data.setOperationalEnum(
//         dto->operationalEnum.getValue(
//             "NOT_IN_USE"
//         )
//     );
//     data.setFormEnum(formEnum);
//     data.setModeEnum("INSTANCE");
//     data.setTypeJson("{}");
//     data.setOrganizationId(organizationId);
//     data.setDisplayOrder(
//         dto->displayOrder.getValue(0)
//     );
//
//     if (formEnum != "WARD")
//     {
//         data.setParentId(parentId);
//     }
//
//     const int rows =
//         dao.insertLocation(data);
//
//     return rows == 1 ? id : "";
// }