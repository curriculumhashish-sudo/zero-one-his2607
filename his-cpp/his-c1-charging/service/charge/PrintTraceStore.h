#pragma once

#ifndef _PRINT_TRACE_STORE_H_
#define _PRINT_TRACE_STORE_H_

#include <mutex>
#include <sstream>
#include <string>
#include "RedisClient.h"

/**
 * 打印追踪快照
 * 4B准备打印时存入，4C确认打印时比对校验
 */
struct PrintTraceSnapshot
{
    std::string busNo;
    std::string scenarioCode;
    int templateVersion;

    // 格式: busNo|scenarioCode|templateVersion
    std::string serialize() const
    {
        std::ostringstream oss;
        oss << busNo << "|" << scenarioCode << "|" << templateVersion;
        return oss.str();
    }

    void deserialize(const std::string& data)
    {
        size_t p1 = data.find('|');
        size_t p2 = data.find('|', p1 + 1);
        busNo = data.substr(0, p1);
        scenarioCode = data.substr(p1 + 1, p2 - p1 - 1);
        templateVersion = std::stoi(data.substr(p2 + 1));
    }
};

/**
 * 打印追踪Redis存储（单例，线程安全）
 * 使用 SET+GET+DEL 实现一次性消费，防重放
 */
class PrintTraceStore
{
public:
    static PrintTraceStore& getInstance()
    {
        static PrintTraceStore instance;
        return instance;
    }

    void put(const std::string& traceId, const PrintTraceSnapshot& snapshot)
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _rc.execute<bool>([&](Redis* r) {
            std::string key = DEFAULT_NS + traceId;
            r->set(key, snapshot.serialize());
            r->expire(key, TIME_OUT);
            return true;
        });
    }

    bool take(const std::string& traceId, PrintTraceSnapshot& out)
    {
        std::lock_guard<std::mutex> lock(_mtx);
        std::string key = DEFAULT_NS + traceId;
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
    PrintTraceStore() : _rc("8.163.102.33", 6380, "Lin@@2830564901zero-one-his") {}
    std::mutex _mtx;
    RedisClient _rc;
    static constexpr const char* DEFAULT_NS = "his-c1-charging-charge-print-trace-store:";
    static constexpr const long long TIME_OUT = 1800;
};

#endif // !_PRINT_TRACE_STORE_H_
