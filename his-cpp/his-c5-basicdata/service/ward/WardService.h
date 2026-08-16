#pragma once
#ifndef _WARDSERVICE_H_
#define _WARDSERVICE_H_

#include <string>
#include <unordered_map>
#include "id/SnowFlake.h"
#include "SimpleDateTimeFormat.h"
#include "TransactionManager.h"
#include "domain/dto/ward/WardDTO.h"
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
#include "domain/do/ward/WardDO.h"

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

    // ============================================================
    // 根据ID查询病房/病床 — 只读查询，不加行锁
    // 返回nullptr表示记录不存在或类型不匹配
    // ============================================================

    // 根据病房ID查询病房名称及所属病区
    // 校验formEnum=="HOUSE"，返回{id, name, wardAreaId=parent_id}
    WardRoomDTO::Wrapper queryRoomById(const std::string& id);

    // 根据病床ID查询病床名称、状态及所属病房
    // 校验formEnum=="BED"，返回{id, name, status, wardRoomId=parent_id}
    // status映射: DISABLED→2, IN_USE→1, 其他→0
    WardBedDTO::Wrapper queryBedById(const std::string& id);

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

    // ============================================================
    // Ward/Bed query methods (c5-ward-pb)
    // ============================================================

    std::string validateQueryRoomById(const oatpp::String& id);
    PtrWardDO getRoomById(const std::string& id);

    std::string validateQueryBedById(const oatpp::String& id);
    PtrBedDO getBedById(const std::string& id);

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
