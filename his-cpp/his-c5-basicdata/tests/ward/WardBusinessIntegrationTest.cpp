/*
 Copyright Zero One Star. All rights reserved.

 @Author: Codex
 @Date: 2026/07/28

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#include "stdafx.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "DbInit.h"
#include "YamlHelper.h"
#include "dao/ward/WardDAO.h"
#include "service/ward/WardService.h"

namespace
{
    class TestDataCleanup
    {
    public:
        void add(const std::string& id)
        {
            if (!id.empty())
            {
                ids.push_back(id);
            }
        }

        void run()
        {
            WardDAO dao;
            for (auto it = ids.rbegin(); it != ids.rend(); ++it)
            {
                dao.deleteById<WardLocationDO>(*it);
            }
            ids.clear();
        }

        ~TestDataCleanup()
        {
            try
            {
                run();
            }
            catch (...)
            {
            }
        }

    private:
        std::vector<std::string> ids;
    };

    bool expect(
        bool condition,
        const std::string& name,
        int& failures)
    {
        std::cout
            << (condition ? "[PASS] " : "[FAIL] ")
            << name
            << std::endl;

        if (!condition)
        {
            ++failures;
        }
        return condition;
    }

    WardLocationAddDTO::Wrapper makeLocation(
        const std::string& organizationId,
        const std::string& name,
        const std::string& formEnum,
        const std::string& parentId = "")
    {
        auto dto = WardLocationAddDTO::createShared();
        dto->organizationId = organizationId;
        dto->name = name;
        dto->formEnum = formEnum;
        if (!parentId.empty())
        {
            dto->parentId = parentId;
        }
        dto->statusEnum = "ACTIVE";
        dto->operationalEnum = "NOT_IN_USE";
        dto->displayOrder = 0;
        return dto;
    }

    RoomModifyDTO::Wrapper makeRoomModification(
        const std::string& id,
        const std::string& name,
        const std::string& parentId,
        const std::string& organizationId,
        int version)
    {
        auto dto = RoomModifyDTO::createShared();
        dto->id = id;
        dto->name = name;
        dto->parentId = parentId;
        dto->organizationId = organizationId;
        dto->displayOrder = 1;
        dto->pyStr = "";
        dto->wbStr = "";
        dto->version = version;
        return dto;
    }

    BedModifyDTO::Wrapper makeBedModification(
        const std::string& id,
        const std::string& name,
        const std::string& parentId,
        const std::string& organizationId,
        int version)
    {
        auto dto = BedModifyDTO::createShared();
        dto->id = id;
        dto->name = name;
        dto->parentId = parentId;
        dto->organizationId = organizationId;
        dto->displayOrder = 1;
        dto->typeJson = "{}";
        dto->version = version;
        return dto;
    }

    WardStatusDTO::Wrapper makeWardStatus(
        const std::string& id,
        const std::string& statusEnum,
        int version)
    {
        auto dto = WardStatusDTO::createShared();
        dto->id = id;
        dto->statusEnum = statusEnum;
        dto->version = version;
        return dto;
    }

    RoomStatusDTO::Wrapper makeRoomStatus(
        const std::string& id,
        const std::string& statusEnum,
        int version)
    {
        auto dto = RoomStatusDTO::createShared();
        dto->id = id;
        dto->statusEnum = statusEnum;
        dto->version = version;
        return dto;
    }

    BedStatusDTO::Wrapper makeBedStatus(
        const std::string& id,
        const std::string& statusEnum,
        int version)
    {
        auto dto = BedStatusDTO::createShared();
        dto->id = id;
        dto->statusEnum = statusEnum;
        dto->version = version;
        return dto;
    }

    class TemporaryWardDatabase
    {
    public:
        explicit TemporaryWardDatabase(
            const DBConfig& sourceConfig)
            : config(sourceConfig)
        {
            const auto stamp =
                std::chrono::high_resolution_clock::now()
                    .time_since_epoch()
                    .count();
            databaseName =
                "ward_business_test_" +
                std::to_string(stamp);

            auto driver =
                sql::mysql::get_mysql_driver_instance();
            adminConnection.reset(driver->connect(
                "tcp://" + config.host + ":" +
                    std::to_string(config.port),
                config.user,
                config.pass));

            try
            {
                auto statement =
                    std::unique_ptr<sql::Statement>(
                        adminConnection->createStatement());

                statement->execute(
                    "CREATE DATABASE `" + databaseName +
                    "` CHARACTER SET utf8mb4 "
                    "COLLATE utf8mb4_general_ci");
                databaseCreated = true;
                adminConnection->setSchema(databaseName);

                statement->execute(R"(
                    CREATE TABLE adm_organization (
                        id VARCHAR(64) NOT NULL PRIMARY KEY,
                        tenant_id VARCHAR(64),
                        is_deleted TINYINT(1) NOT NULL DEFAULT 0
                    ) ENGINE=InnoDB
                )");

                statement->execute(R"(
                    CREATE TABLE adm_location (
                        id VARCHAR(64) NOT NULL PRIMARY KEY,
                        tenant_id VARCHAR(64),
                        created_at DATETIME(6) NOT NULL,
                        updated_at DATETIME(6),
                        created_by VARCHAR(64),
                        updated_by VARCHAR(64),
                        is_deleted TINYINT(1) NOT NULL DEFAULT 0,
                        deleted_at DATETIME(6),
                        deleted_by VARCHAR(64),
                        version INT NOT NULL DEFAULT 0,
                        name VARCHAR(255) NOT NULL,
                        bus_no VARCHAR(255) NOT NULL,
                        status_enum VARCHAR(255) NOT NULL,
                        operational_enum VARCHAR(255),
                        form_enum VARCHAR(255) NOT NULL,
                        mode_enum VARCHAR(255) NOT NULL,
                        type_json VARCHAR(255),
                        organization_id VARCHAR(64) NOT NULL,
                        display_order INT,
                        py_str VARCHAR(255),
                        wb_str VARCHAR(255),
                        parent_id VARCHAR(64)
                    ) ENGINE=InnoDB
                )");

                statement->execute(
                    "INSERT INTO adm_organization "
                    "(id, tenant_id, is_deleted) VALUES "
                    "('WARD_TEST_ORG', 'WARD_TEST_TENANT', 0), "
                    "('WARD_TEST_ORG_2', 'WARD_TEST_TENANT', 0)");

                config.db = databaseName;
                if (!DbInit::initDbPool(config))
                {
                    DbInit::releasePool();
                    throw std::runtime_error(
                        "Temporary database connection failed.");
                }
                poolInitialized = true;
            }
            catch (...)
            {
                dropDatabase();
                throw;
            }
        }

        ~TemporaryWardDatabase()
        {
            if (poolInitialized)
            {
                DbInit::releasePool();
            }
            dropDatabase();
        }

        std::string organizationId() const
        {
            return "WARD_TEST_ORG";
        }

        std::string secondOrganizationId() const
        {
            return "WARD_TEST_ORG_2";
        }

    private:
        void dropDatabase()
        {
            if (!databaseCreated || !adminConnection)
            {
                return;
            }

            try
            {
                adminConnection->setSchema("information_schema");
                auto statement =
                    std::unique_ptr<sql::Statement>(
                        adminConnection->createStatement());
                statement->execute(
                    "DROP DATABASE IF EXISTS `" +
                    databaseName + "`");
                databaseCreated = false;
            }
            catch (...)
            {
            }
        }

        DBConfig config;
        std::string databaseName;
        std::unique_ptr<sql::Connection> adminConnection;
        bool databaseCreated = false;
        bool poolInitialized = false;
    };
}

int runWardBusinessIntegration()
{
    int failures = 0;

    try
    {
        YAML::Node config =
            YAML::LoadFile("./conf/data-source.yaml");
        YamlHelper yaml;
        std::string host;
        std::string database;
        int port = -1;
        yaml.parseDbConnUrl(
            yaml.getString(&config, "spring.datasource.url"),
            &host,
            &port,
            &database);

        DBConfig sourceConfig(
            yaml.getString(&config, "spring.datasource.username"),
            yaml.getString(&config, "spring.datasource.password"),
            database,
            host,
            port,
            3);

        TemporaryWardDatabase databaseScope(sourceConfig);
        const std::string organizationId =
            databaseScope.organizationId();
        const std::string secondOrganizationId =
            databaseScope.secondOrganizationId();
        expect(
            true,
            "Isolated temporary database initializes",
            failures);

        PayloadDTO payload;
        payload.setUsername("ward-business-test");
        payload.setId("ward-business-test");

        const std::string suffix =
            std::to_string(static_cast<long long>(std::time(nullptr)));
        WardService service;
        TestDataCleanup cleanup;

        const std::string wardId = service.createLocation(
            makeLocation(
                organizationId,
                "TEST_WARD_" + suffix,
                "WARD"),
            payload);
        cleanup.add(wardId);

        const std::string roomId = service.createLocation(
            makeLocation(
                organizationId,
                "TEST_ROOM_A_" + suffix,
                "HOUSE",
                wardId),
            payload);
        cleanup.add(roomId);

        const std::string secondRoomId = service.createLocation(
            makeLocation(
                organizationId,
                "TEST_ROOM_B_" + suffix,
                "HOUSE",
                wardId),
            payload);
        cleanup.add(secondRoomId);

        const std::string bedId = service.createLocation(
            makeLocation(
                organizationId,
                "TEST_BED_" + suffix,
                "BED",
                roomId),
            payload);
        cleanup.add(bedId);

        if (!expect(
            !wardId.empty() &&
            !roomId.empty() &&
            !secondRoomId.empty() &&
            !bedId.empty(),
            "Temporary ward hierarchy is created",
            failures))
        {
            cleanup.run();
            return 1;
        }

        auto wardModify = WardModifyDTO::createShared();
        wardModify->id = wardId;
        wardModify->name = "TEST_WARD_UPDATED_" + suffix;
        wardModify->organizationId = organizationId;
        wardModify->displayOrder = 1;
        wardModify->pyStr = "";
        wardModify->wbStr = "";
        wardModify->version = 0;
        auto wardModifyResult =
            service.modifyWard(wardModify, payload);
        expect(
            wardModifyResult.success &&
            wardModifyResult.data->getVersion() == 1,
            "Ward modification increments version",
            failures);

        wardModify->organizationId = secondOrganizationId;
        wardModify->version = 1;
        expect(
            !service.modifyWard(
                wardModify,
                payload).success,
            "Ward with child rooms cannot change organization",
            failures);
        wardModify->organizationId = organizationId;

        auto duplicateRoomResult = service.modifyRoom(
            makeRoomModification(
                secondRoomId,
                "TEST_ROOM_A_" + suffix,
                wardId,
                organizationId,
                0),
            payload);
        expect(
            !duplicateRoomResult.success,
            "Duplicate sibling room name is rejected",
            failures);

        auto roomModifyResult = service.modifyRoom(
            makeRoomModification(
                roomId,
                "TEST_ROOM_UPDATED_" + suffix,
                wardId,
                organizationId,
                0),
            payload);
        expect(
            roomModifyResult.success &&
            roomModifyResult.data->getVersion() == 1,
            "Room modification increments version",
            failures);

        auto staleRoomResult = service.modifyRoom(
            makeRoomModification(
                roomId,
                "TEST_ROOM_STALE_" + suffix,
                wardId,
                organizationId,
                0),
            payload);
        expect(
            !staleRoomResult.success,
            "Stale room version is rejected",
            failures);

        auto invalidParentResult = service.modifyRoom(
            makeRoomModification(
                roomId,
                "TEST_ROOM_UPDATED_" + suffix,
                bedId,
                organizationId,
                1),
            payload);
        expect(
            !invalidParentResult.success,
            "Room cannot use a bed as its parent",
            failures);

        auto bedModifyResult = service.modifyBed(
            makeBedModification(
                bedId,
                "TEST_BED_UPDATED_" + suffix,
                roomId,
                organizationId,
                0),
            payload);
        expect(
            bedModifyResult.success &&
            bedModifyResult.data->getVersion() == 1,
            "Bed modification increments version",
            failures);

        auto invalidJsonBed = makeBedModification(
            bedId,
            "TEST_BED_UPDATED_" + suffix,
            roomId,
            organizationId,
            1);
        invalidJsonBed->typeJson = "not-json";
        expect(
            !service.modifyBed(
                invalidJsonBed,
                payload).success,
            "Invalid bed type JSON is rejected",
            failures);

        expect(
            !service.modifyRoom(
                makeRoomModification(
                    roomId,
                    std::string(256, 'R'),
                    wardId,
                    organizationId,
                    1),
                payload).success,
            "Oversized room name is rejected",
            failures);

        expect(
            !service.changeBedStatus(
                makeBedStatus(bedId, "UNKNOWN", 1),
                payload).success,
            "Unknown bed status is rejected",
            failures);

        WardDAO stateDao;
        stateDao.getSqlSession()->executeUpdate(
            "UPDATE adm_location SET operational_enum = 'IN_USE' WHERE id = ?",
            "%s",
            bedId);
        auto inUseDisableResult = service.changeBedStatus(
            makeBedStatus(bedId, "DISABLED", 1),
            payload);
        expect(
            !inUseDisableResult.success,
            "In-use bed cannot be disabled",
            failures);

        expect(
            !service.removeBed(bedId, payload).success,
            "In-use bed cannot be removed",
            failures);

        stateDao.getSqlSession()->executeUpdate(
            "UPDATE adm_location SET operational_enum = 'NOT_IN_USE' WHERE id = ?",
            "%s",
            bedId);

        expect(
            !service.changeRoomStatus(
                makeRoomStatus(roomId, "DISABLED", 1),
                payload).success,
            "Room with an active bed cannot be disabled",
            failures);

        expect(
            !service.changeWardStatus(
                makeWardStatus(wardId, "DISABLED", 1),
                payload).success,
            "Ward with active rooms cannot be disabled",
            failures);

        auto disableBedResult = service.changeBedStatus(
            makeBedStatus(bedId, "DISABLED", 1),
            payload);
        expect(
            disableBedResult.success &&
            disableBedResult.data->getVersion() == 2,
            "Bed is disabled",
            failures);

        auto disableRoomResult = service.changeRoomStatus(
            makeRoomStatus(roomId, "DISABLED", 1),
            payload);
        expect(
            disableRoomResult.success &&
            disableRoomResult.data->getVersion() == 2,
            "Room is disabled after its beds",
            failures);

        expect(
            !service.changeBedStatus(
                makeBedStatus(bedId, "ACTIVE", 2),
                payload).success,
            "Bed cannot be enabled under a disabled room",
            failures);

        auto enableRoomResult = service.changeRoomStatus(
            makeRoomStatus(roomId, "ACTIVE", 2),
            payload);
        expect(
            enableRoomResult.success &&
            enableRoomResult.data->getVersion() == 3,
            "Room is enabled under an active ward",
            failures);

        auto enableBedResult = service.changeBedStatus(
            makeBedStatus(bedId, "ACTIVE", 2),
            payload);
        expect(
            enableBedResult.success &&
            enableBedResult.data->getVersion() == 3,
            "Bed is enabled under an active room",
            failures);

        expect(
            !service.removeBed(wardId, payload).success,
            "Cross-type deletion is rejected",
            failures);

        expect(
            !service.removeWard("", payload).success,
            "Empty location ID is rejected",
            failures);

        expect(
            !service.removeWard(wardId, payload).success,
            "Ward with child rooms cannot be removed",
            failures);

        expect(
            !service.removeRoom(roomId, payload).success,
            "Room with child beds cannot be removed",
            failures);

        expect(
            service.removeBed(bedId, payload).success,
            "Bed is soft-deleted",
            failures);

        expect(
            service.removeRoom(roomId, payload).success,
            "Empty room is soft-deleted",
            failures);

        expect(
            service.removeRoom(secondRoomId, payload).success,
            "Second empty room is soft-deleted",
            failures);

        expect(
            service.removeWard(wardId, payload).success,
            "Empty ward is soft-deleted",
            failures);

        cleanup.run();
    }
    catch (const std::exception& exception)
    {
        std::cerr
            << "[FAIL] Unexpected exception: "
            << exception.what()
            << std::endl;
        ++failures;
    }
    catch (...)
    {
        std::cerr
            << "[FAIL] Unexpected non-standard exception."
            << std::endl;
        ++failures;
    }

    std::cout
        << "Ward business integration failures: "
        << failures
        << std::endl;
    return failures == 0 ? 0 : 1;
}

int main()
{
    oatpp::base::Environment::init();
    const int result = runWardBusinessIntegration();
    oatpp::base::Environment::destroy();
    return result;
}
