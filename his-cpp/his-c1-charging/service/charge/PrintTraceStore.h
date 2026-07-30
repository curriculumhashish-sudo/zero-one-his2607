#pragma once

#ifndef _PRINT_TRACE_STORE_H_
#define _PRINT_TRACE_STORE_H_

#include <map>
#include <mutex>
#include <string>

/**
 * 打印追踪快照
 * 4B准备打印时存入，4C确认打印时比对校验
 */
struct PrintTraceSnapshot
{
    std::string busNo;
    std::string scenarioCode;
    int templateVersion;
};

/**
 * 打印追踪内存存储（单例，线程安全）
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
        std::lock_guard<std::mutex> lock(mtx);
        store[traceId] = snapshot;
    }

    bool take(const std::string& traceId, PrintTraceSnapshot& out)
    {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = store.find(traceId);
        if (it == store.end()) return false;
        out = it->second;
        store.erase(it);
        return true;
    }

private:
    PrintTraceStore() = default;
    std::map<std::string, PrintTraceSnapshot> store;
    std::mutex mtx;
};

#endif // !_PRINT_TRACE_STORE_H_
