#ifndef _ORGANIZATIONMONGOCLIENTFACTORY_H_
#define _ORGANIZATIONMONGOCLIENTFACTORY_H_

#include <memory>

#include "MongoClient.h"

/**
 * Creates the process-wide MongoDB client used by organization enum lookups.
 */
class OrganizationMongoClientFactory
{
public:
    static std::shared_ptr<MongoClient> create();
};

#endif // !_ORGANIZATIONMONGOCLIENTFACTORY_H_
