#pragma once
#ifndef _WARDSERVICE_H_
#define _WARDSERVICE_H_

#include <string>
#include <unordered_map>
#include "id/SnowFlake.h"
#include "SimpleDateTimeFormat.h"
#include "TransactionManager.h"
#include "domain/dto/ward/WardLocationAddDTO.h"
#include "domain/dto/ward/WardModifyDTO.h"
#include "domain/dto/ward/WardStatusDTO.h"
#include "domain/dto/room/RoomModifyDTO.h"
#include "domain/dto/room/RoomStatusDTO.h"
#include "domain/dto/bed/BedModifyDTO.h"
#include "domain/dto/bed/BedStatusDTO.h"
#include "domain/dto/PayloadDTO.h"
#include "domain/do/ward/WardLocationDO.h"
#include "domain/vo/ward/WardNameVO.h"
#include "domain/vo/ward/WardTreeNodeVO.h"
#include "domain/query/ward/WardTreeQuery.h"

struct LocationMutationResult
{
    bool success = false;
    std::string message;
    PtrWardLocationDO data;
};

class WardService
{
public:
    oatpp::List<WardTreeNodeVO::Wrapper> GetTree(WardTreeQuery::Wrapper query);

    std::string createLocation(
        const WardLocationAddDTO::Wrapper& dto,
        const PayloadDTO& payload);

    LocationMutationResult modifyWard(
        const WardModifyDTO::Wrapper& dto,
        const PayloadDTO& payload);

    LocationMutationResult modifyRoom(
        const RoomModifyDTO::Wrapper& dto,
        const PayloadDTO& payload);

    LocationMutationResult modifyBed(
        const BedModifyDTO::Wrapper& dto,
        const PayloadDTO& payload);

    LocationMutationResult removeWard(
        const std::string& id,
        const PayloadDTO& payload);

    LocationMutationResult removeRoom(
        const std::string& id,
        const PayloadDTO& payload);

    LocationMutationResult removeBed(
        const std::string& id,
        const PayloadDTO& payload);

    LocationMutationResult changeWardStatus(
        const WardStatusDTO::Wrapper& dto,
        const PayloadDTO& payload);

    LocationMutationResult changeRoomStatus(
        const RoomStatusDTO::Wrapper& dto,
        const PayloadDTO& payload);

    LocationMutationResult changeBedStatus(
        const BedStatusDTO::Wrapper& dto,
        const PayloadDTO& payload);

private:
    LocationMutationResult modifyLocation(
        const std::string& id,
        const std::string& name,
        const std::string& organizationId,
        const std::string& parentId,
        int displayOrder,
        const std::string& pyStr,
        const std::string& wbStr,
        const std::string& typeJson,
        int version,
        const std::string& formEnum,
        const PayloadDTO& payload);

    LocationMutationResult removeLocation(
        const std::string& id,
        const std::string& formEnum,
        const PayloadDTO& payload);

    LocationMutationResult changeLocationStatus(
        const std::string& id,
        const std::string& statusEnum,
        int version,
        const std::string& formEnum,
        const PayloadDTO& payload);
};

#endif
