#include <ctime>
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <iostream>

/**
 * 将 Unix 时间戳转换为 MySQL DATETIME(6) 格式字符串（含微秒）
 *
 * @param timestamp Unix 时间戳字符串，支持：
 *                  - 秒级时间戳（10位）："1785121938"
 *                  - 毫秒级时间戳（13位）："1785121938586"
 * @return MySQL DATETIME(6) 格式字符串："YYYY-MM-DD HH:MM:SS.ffffff"
 *         如果转换失败，返回空字符串
 *
 * @example
 *   ConvertTimestampToMySQLDatetime6("1785121938586")
 *     -> "2026-07-27 14:30:00.586000"
 */
inline std::string ConvertTimestampToMySQLDatetime6(const std::string& timestamp) {
    if (timestamp.empty()) {
        std::cerr << "[ConvertTimestampToMySQLDatetime6] Input timestamp is empty" << std::endl;
        return "";
    }

    try {
        long long ts = std::stoll(timestamp);

        // 提取毫秒部分（如果有）
        int milliseconds = 0;
        if (ts > 10000000000LL) {
            // 毫秒级时间戳（13位）
            milliseconds = ts % 1000;
            ts = ts / 1000;
        }

        // 验证范围
        if (ts < 0 || ts > 32503680000LL) {
            std::cerr << "[ConvertTimestampToMySQLDatetime6] Invalid timestamp range: " << ts << std::endl;
            return "";
        }

        time_t t = static_cast<time_t>(ts);
        struct tm tm_info;

#ifdef _WIN32
        if (localtime_s(&tm_info, &t) != 0) {
            std::cerr << "[ConvertTimestampToMySQLDatetime6] localtime_s failed" << std::endl;
            return "";
        }
#else
        if (localtime_r(&t, &tm_info) == nullptr) {
            std::cerr << "[ConvertTimestampToMySQLDatetime6] localtime_r failed" << std::endl;
            return "";
        }
#endif

        // 格式化日期时间部分
        char buffer[32];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm_info);

        // 构建最终字符串：YYYY-MM-DD HH:MM:SS.ffffff
        // 毫秒转微秒（乘以1000）
        int microseconds = milliseconds * 1000;

        std::ostringstream oss;
        oss << buffer << "." << std::setw(6) << std::setfill('0') << microseconds;

        return oss.str();

    }
    catch (const std::invalid_argument& e) {
        std::cerr << "[ConvertTimestampToMySQLDatetime6] Invalid timestamp format: " << timestamp
            << ", error: " << e.what() << std::endl;
        return "";
    }
    catch (const std::out_of_range& e) {
        std::cerr << "[ConvertTimestampToMySQLDatetime6] Timestamp out of range: " << timestamp
            << ", error: " << e.what() << std::endl;
        return "";
    }
    catch (const std::exception& e) {
        std::cerr << "[ConvertTimestampToMySQLDatetime6] Unexpected error: " << e.what() << std::endl;
        return "";
    }
}