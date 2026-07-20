#ifndef _SUPPLIER_UPDATE_DTO_H_
#define _SUPPLIER_UPDATE_DTO_H_

#include "../../GlobalInclude.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(DTO)

class SupplierUpdateDTO : public oatpp::DTO
{
    DTO_INIT(SupplierUpdateDTO, DTO);

    API_DTO_FIELD_REQUIRE(
        String,
        id,
        "Supplier ID.",
        true);

    API_DTO_FIELD_REQUIRE(
        Int32,
        version,
        "Optimistic lock version number.",
        true);

    API_DTO_FIELD_DEFAULT(
        String,
        typeEnum,
        "Supplier type: 1: Manufacturer, 2: Supplier.");

    API_DTO_FIELD_DEFAULT(
        String,
        creditCode,
        "Credit code.");

    API_DTO_FIELD_DEFAULT(
        String,
        legalRepresentative,
        "Legal representative.");

    API_DTO_FIELD_DEFAULT(
        String,
        address,
        "Supplier address.");

    API_DTO_FIELD_DEFAULT(
        String,
        pyStr,
        "Pinyin code.");

    API_DTO_FIELD_DEFAULT(
        String,
        wbStr,
        "Wubi code.");

    API_DTO_FIELD_DEFAULT(
        String,
        phone,
        "Supplier phone number.");

    API_DTO_FIELD_DEFAULT(
        String,
        email,
        "Supplier email address.");

    API_DTO_FIELD_DEFAULT(
        String,
        bankName,
        "Bank name.");

    API_DTO_FIELD_DEFAULT(
        String,
        bankAccount,
        "Bank account number.");

    API_DTO_FIELD_DEFAULT(
        String,
        orgId,
        "Organization ID.");

    API_DTO_FIELD_DEFAULT(
        Int32,
        displayOrder,
        "Display order.");

    API_DTO_FIELD_DEFAULT(
        String,
        note,
        "Remark.");
};

#include OATPP_CODEGEN_END(DTO)

#endif