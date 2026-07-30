#pragma once

#ifndef _SETTLE_DRAFT_STORE_H_
#define _SETTLE_DRAFT_STORE_H_

#include <map>
#include <mutex>
#include <string>
#include <vector>

/**
 * 结算草稿快照
 * 3A预结算时存入，3B正式结算时比对校验
 */
struct SettleDraftSnapshot
{
    std::string encounterId;
    std::vector<std::string> chargeItemIds;
    double totalAmount;
};

/**
 * 结算草稿内存存储（单例，线程安全）
 * 后续可替换为 Redis 持久化存储
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
        std::lock_guard<std::mutex> lock(mtx);
        store[draftId] = snapshot;
    }

    // 取出并删除草稿快照（一次性消费，防重放）
    bool take(const std::string& draftId, SettleDraftSnapshot& out)
    {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = store.find(draftId);
        if (it == store.end()) return false;
        out = it->second;
        store.erase(it);
        return true;
    }

private:
    SettleDraftStore() = default;
    std::map<std::string, SettleDraftSnapshot> store;
    std::mutex mtx;
};

#endif // !_SETTLE_DRAFT_STORE_H_
