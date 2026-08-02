#ifndef _ORGANIZATIONENUMDICTIONARY_H_
#define _ORGANIZATIONENUMDICTIONARY_H_

#include <string>
#include <unordered_map>

#include "oatpp/core/Types.hpp"

/**
 * Text lookup tables built from the organization enum collections in MongoDB.
 */
struct OrganizationEnumTextMap
{
    std::unordered_map<std::string, std::string> typeTexts;
    std::unordered_map<std::string, std::string> classTexts;
    std::unordered_map<std::string, std::string> statusTexts;

    oatpp::String getTypeText(const std::string& code) const;
    oatpp::String getClassText(const std::string& code) const;
    oatpp::String getStatusText(const std::string& code) const;

    bool hasTypeText(const std::string& code) const;
    bool hasClassText(const std::string& code) const;
    bool hasStatusText(const std::string& code) const;
};

enum class OrganizationEnumLoadStatus
{
    SUCCESS,
    MONGO_UNAVAILABLE,
    ENUM_DATA_INCOMPLETE
};

/**
 * Result of one uncached MongoDB enum lookup.
 */
struct OrganizationEnumLoadResult
{
    OrganizationEnumLoadStatus status =
        OrganizationEnumLoadStatus::MONGO_UNAVAILABLE;
    OrganizationEnumTextMap mappings;
};

class OrganizationEnumDictionary
{
public:
    /**
     * Loads the three organization enum groups once for the current request.
     * The result status distinguishes MongoDB access failures from incomplete
     * enum data.
     */
    OrganizationEnumLoadResult load() const;
};

#endif // !_ORGANIZATIONENUMDICTIONARY_H_
