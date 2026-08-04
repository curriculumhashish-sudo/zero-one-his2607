#include "stdafx.h"

#include <iostream>
#include <string>

#include "domain/dto/pat/PatientDTO.h"
#include "domain/query/pat/PatientQuery.h"

namespace
{
    bool expect(bool condition, const std::string& name, int& failures)
    {
        std::cout << (condition ? "[PASS] " : "[FAIL] ") << name << std::endl;
        if (!condition) ++failures;
        return condition;
    }
}

int main()
{
    int failures = 0;

    auto add = PatientAddDTO::createShared();
    expect(!add->validate().empty(), "add rejects missing required fields", failures);
    add->name = "Alice";
    add->genderEnum = "FEMALE";
    add->organizationId = "org-1";
    const std::string addValidation = add->validate();
    expect(addValidation.empty(), "add applies valid database defaults: " + addValidation, failures);
    expect(add->statusEnum == "ACTIVE", "add defaults statusEnum", failures);
    expect(add->activeFlag.getValue(false), "add defaults activeFlag", failures);
    expect(!add->tempFlag.getValue(true), "add defaults tempFlag", failures);

    auto update = PatientUpdateDTO::createShared();
    update->id = "patient-1";
    expect(update->validate() == "version is required.", "update requires optimistic-lock version", failures);
    update->version = 0;
    update->activeFlag = false;
    update->tempFlag = false;
    expect(update->validate().empty(), "update accepts id, version and explicit false flags", failures);
    expect(update->activeFlag.get() != nullptr && !update->activeFlag.getValue(true),
        "update preserves explicit false activeFlag", failures);
    expect(update->tempFlag.get() != nullptr && !update->tempFlag.getValue(true),
        "update preserves explicit false tempFlag", failures);
    update->name = "";
    expect(update->validate() == "name must not be empty.", "update protects non-null name", failures);

    auto detail = PatientDetailQuery::createShared();
    expect(detail->validate() == "id is required.", "detail requires id", failures);
    detail->id = "patient-1";
    expect(detail->validate().empty(), "detail accepts a valid string id", failures);

    return failures == 0 ? 0 : 1;
}
