#pragma once

#ifndef _SETTLE_DRAFT_STORE_H_
#define _SETTLE_DRAFT_STORE_H_

#include <mutex>
#include <sstream>
#include <string>
#include <vector>
#include "RedisClient.h"

/**
 * 结算草稿快照
 * 3A预结算时存入，3B正式结算时比对校验
 */
struct SettleDraftSnapshot
{
    std::string encounterId;
    std::vector<std::string> chargeItemIds;
    double totalAmount;
    //序列化
    std::string serialize() const
    {
        ostringstream oss;
        oss << encounterId << "|"
            << totalAmount << "|";
        if (!chargeItemIds.empty())
        {
            for (size_t i = 0; i < chargeItemIds.size(); i++)
            {
                if (i > 0)
                    oss << ",";
                oss << chargeItemIds[i];
            }
        }
        return oss.str();
    }
    //反序列化
    void deserialize(const std::string& data)
    {
        size_t p1 = data.find("|",0);
        size_t p2 = data.find("|",p1+1);
        encounterId = data.substr(0,p1);
        totalAmount = std::stod(data.substr(p1+1,p2-p1-1));
        std::string ids =  data.substr(p2+1);
        chargeItemIds.clear();
        if (!ids.empty())
        {
            std::istringstream iss(ids);
            std::string id;
            while (std::getline(iss,id,','))
            {
                chargeItemIds.push_back(id);
            }
        }
    }
};

/**
 * 结算草稿redis存储
 */
class SettleDraftStore
{
public:
    static SettleDraftStore& getInstance()
    {
        static SettleDraftStore instance;
        return instance;
    }

    // 存入草稿快照
    void put(const std::string& draftId, const SettleDraftSnapshot& snapshot)
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _rc.execute<bool>([&](Redis* r) {
            std::string key = DEFAULT_NS + draftId;
            r->set(key, snapshot.serialize());
            r->expire(key, TIME_OUT);
            return true;
        });
    }

    // 取出并删除草稿快照（一次性消费，防重放）
    bool take(const std::string& draftId, SettleDraftSnapshot& out)
    {
        std::lock_guard<std::mutex> lock(_mtx);
        //读取键值对
        std::string key = DEFAULT_NS + draftId;
        OptionalString getr = _rc.execute<OptionalString>([&](Redis* r) {
            return r->get(key);
        });
        if (getr)
        {
            out.deserialize(getr.value());
            _rc.execute<long long>([&](Redis* r) {
                return r->del(key);
            });
        }
        else
        {
            return false;
        }
        return true;
    }

private:
    SettleDraftStore():_rc("8.163.102.33", 6380, "Lin@@2830564901zero-one-his") {}
    std::mutex _mtx;
    RedisClient _rc;
	static constexpr const char* DEFAULT_NS = "his-c1-charging-charge-settle-draft-store:";
    static constexpr const long long TIME_OUT = 1800;
};

#endif // !_SETTLE_DRAFT_STORE_H_
