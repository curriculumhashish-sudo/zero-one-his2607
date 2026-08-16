#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/08/06 00:00:00

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

#ifndef _INPATIENT_NURSE_REQUEST_VALIDATION_H_
#define _INPATIENT_NURSE_REQUEST_VALIDATION_H_

#include <algorithm>
#include <cctype>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <limits>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

namespace inpatientnurse::validation
{
	inline std::string trim(const std::string& value)
	{
		auto begin = std::find_if_not(value.begin(), value.end(),
			[](unsigned char ch) { return std::isspace(ch) != 0; });
		auto end = std::find_if_not(value.rbegin(), value.rend(),
			[](unsigned char ch) { return std::isspace(ch) != 0; }).base();
		return begin < end ? std::string(begin, end) : std::string();
	}

	inline bool present(const oatpp::String& value)
	{
		return value && !trim(value->c_str()).empty();
	}

	inline bool allDigits(const std::string& value)
	{
		return !value.empty() && std::all_of(value.begin(), value.end(),
			[](unsigned char ch) { return std::isdigit(ch) != 0; });
	}

	inline bool signedInteger(const std::string& value)
	{
		return allDigits(value) || (value.size() > 1 && value.front() == '-'
			&& allDigits(value.substr(1)));
	}

	inline bool floatingPoint(const std::string& value)
	{
		if (value.empty()) return false;
		try
		{
			std::size_t used = 0;
			const auto parsed = std::stod(value, &used);
			return used == value.size() && std::isfinite(parsed);
		}
		catch (...) { return false; }
	}

	inline bool validPage(const oatpp::UInt64& pageIndex, const oatpp::UInt64& pageSize)
	{
		const auto index = pageIndex.getValue(0);
		const auto size = pageSize.getValue(0);
		return index >= 1 && index <= 1000000 && size >= 1 && size <= 200;
	}

	inline bool validDate(const oatpp::String& value, bool allowEmpty = true)
	{
		if (!present(value)) return allowEmpty;
		const auto text = trim(value->c_str());
		if (text.size() != 10 || text[4] != '-' || text[7] != '-'
			|| !allDigits(text.substr(0, 4)) || !allDigits(text.substr(5, 2))
			|| !allDigits(text.substr(8, 2))) return false;
		const int year = std::stoi(text.substr(0, 4));
		const int month = std::stoi(text.substr(5, 2));
		const int day = std::stoi(text.substr(8, 2));
		if (month < 1 || month > 12 || day < 1) return false;
		static const int days[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
		int maximum = days[month];
		if (month == 2 && (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)))
			maximum = 29;
		return day <= maximum;
	}

	inline bool validDateTime(const oatpp::String& value, bool allowEmpty = true)
	{
		if (!present(value)) return allowEmpty;
		const auto text = trim(value->c_str());
		if (text.size() != 19 || (text[10] != ' ' && text[10] != 'T')
			|| text[13] != ':' || text[16] != ':'
			|| !allDigits(text.substr(11, 2)) || !allDigits(text.substr(14, 2))
			|| !allDigits(text.substr(17, 2))) return false;
		return validDate(oatpp::String(text.substr(0, 10).c_str()), false)
			&& std::stoi(text.substr(11, 2)) <= 23
			&& std::stoi(text.substr(14, 2)) <= 59
			&& std::stoi(text.substr(17, 2)) <= 59;
	}

	inline bool validTime(const oatpp::String& value, bool allowEmpty = true)
	{
		if (!present(value)) return allowEmpty;
		const auto text = trim(value->c_str());
		return text.size() == 5 && text[2] == ':'
			&& allDigits(text.substr(0, 2)) && allDigits(text.substr(3, 2))
			&& std::stoi(text.substr(0, 2)) <= 23
			&& std::stoi(text.substr(3, 2)) <= 59;
	}

	inline bool validWindowRange(const oatpp::String& value)
	{
		if (!present(value)) return false;
		const auto text = trim(value->c_str());
		return text == "3d" || text == "7d" || text == "14d";
	}

	inline bool validHourSlot(const oatpp::String& value)
	{
		if (!present(value)) return false;
		const auto text = trim(value->c_str());
		return text.size() <= 2 && allDigits(text) && std::stoi(text) <= 23;
	}

	inline bool validDateRange(const oatpp::String& start, const oatpp::String& end)
	{
		if (!validDateTime(start) || !validDateTime(end)) return false;
		if (!present(start) || !present(end)) return true;
		return trim(start->c_str()) <= trim(end->c_str());
	}

	inline bool validDateOnlyRange(const oatpp::String& start, const oatpp::String& end)
	{
		if (!validDate(start) || !validDate(end)) return false;
		if (!present(start) || !present(end)) return true;
		return trim(start->c_str()) <= trim(end->c_str());
	}

	inline bool validDateTimeRangeDays(const oatpp::String& start,
		const oatpp::String& end, int maximumDays)
	{
		if (!validDateRange(start, end)) return false;
		if (!present(start) || !present(end)) return true;
		auto parse = [](const std::string& value)
		{
			auto normalized = value;
			if (normalized.size() > 10 && normalized[10] == 'T')
				normalized[10] = ' ';
			std::tm result = {};
			std::istringstream stream(normalized);
			stream >> std::get_time(&result, "%Y-%m-%d %H:%M:%S");
			if (stream.fail()) return static_cast<std::time_t>(-1);
			result.tm_isdst = -1;
			return std::mktime(&result);
		};
		const auto from = parse(trim(start->c_str()));
		const auto to = parse(trim(end->c_str()));
		return from != static_cast<std::time_t>(-1)
			&& to != static_cast<std::time_t>(-1)
			&& std::difftime(to, from) <= maximumDays * 24.0 * 60.0 * 60.0;
	}

	inline bool validCsv(const oatpp::String& value, std::size_t maximum,
		bool required = false)
	{
		if (!present(value)) return !required;
		std::set<std::string> unique;
		std::string text = value->c_str();
		std::size_t begin = 0;
		while (begin <= text.size())
		{
			auto end = text.find(',', begin);
			auto item = trim(text.substr(begin,
				end == std::string::npos ? std::string::npos : end - begin));
			if (item.empty() || !unique.insert(item).second || unique.size() > maximum)
				return false;
			if (end == std::string::npos) break;
			begin = end + 1;
		}
		return !unique.empty();
	}

	template<typename T>
	inline bool oneOf(const T& value, std::initializer_list<std::string> options,
		bool allowEmpty = true)
	{
		if (!present(value)) return allowEmpty;
		const auto actual = trim(value->c_str());
		return std::find(options.begin(), options.end(), actual) != options.end();
	}
}

/* Strict conversion rejects signs for unsigned values and invalid booleans. */
#undef API_HANDLER_QUERY_PARAM
#define API_HANDLER_QUERY_PARAM(__VAR__, __TYPE__, __PARAMS__) \
auto __VAR__ = __TYPE__::createShared(); \
for (auto& __c4_param : __PARAMS__.getAll()) { \
	auto __c4_data = __c4_param.second.getMemoryHandle().get(); \
	const std::string __c4_raw = *__c4_data; \
	auto __c4_type = __VAR__[__c4_param.first.toString()].getValueType(); \
	if (__c4_type == oatpp::String::Class::getType()) \
		__VAR__[__c4_param.first.toString()] = oatpp::String(URIUtil::urlDecode(__c4_data->c_str())); \
	else if (__c4_type == oatpp::Int32::Class::getType()) { \
		if (!inpatientnurse::validation::signedInteger(__c4_raw)) throw std::invalid_argument("int32"); \
		__VAR__[__c4_param.first.toString()] = oatpp::Int32(std::stoi(__c4_raw)); \
	} else if (__c4_type == oatpp::UInt32::Class::getType()) { \
		if (!inpatientnurse::validation::allDigits(__c4_raw)) throw std::invalid_argument("uint32"); \
		const auto __c4_unsigned = std::stoull(__c4_raw); \
		if (__c4_unsigned > std::numeric_limits<v_uint32>::max()) throw std::out_of_range("uint32"); \
		__VAR__[__c4_param.first.toString()] = oatpp::UInt32(static_cast<v_uint32>(__c4_unsigned)); \
	} else if (__c4_type == oatpp::Int64::Class::getType()) { \
		if (!inpatientnurse::validation::signedInteger(__c4_raw)) throw std::invalid_argument("int64"); \
		__VAR__[__c4_param.first.toString()] = oatpp::Int64(std::stoll(__c4_raw)); \
	} else if (__c4_type == oatpp::UInt64::Class::getType()) { \
		if (!inpatientnurse::validation::allDigits(__c4_raw)) throw std::invalid_argument("uint64"); \
		__VAR__[__c4_param.first.toString()] = oatpp::UInt64(std::stoull(__c4_raw)); \
	} else if (__c4_type == oatpp::Float32::Class::getType()) { \
		if (!inpatientnurse::validation::floatingPoint(__c4_raw)) throw std::invalid_argument("float32"); \
		__VAR__[__c4_param.first.toString()] = oatpp::Float32(std::stof(__c4_raw)); \
	} else if (__c4_type == oatpp::Float64::Class::getType()) { \
		if (!inpatientnurse::validation::floatingPoint(__c4_raw)) throw std::invalid_argument("float64"); \
		__VAR__[__c4_param.first.toString()] = oatpp::Float64(std::stod(__c4_raw)); \
	} else if (__c4_type == oatpp::Boolean::Class::getType()) { \
		if (__c4_raw != "true" && __c4_raw != "false" && __c4_raw != "1" && __c4_raw != "0") \
			throw std::invalid_argument("boolean"); \
		__VAR__[__c4_param.first.toString()] = oatpp::Boolean(__c4_raw == "true" || __c4_raw == "1"); \
	} \
}

/*
 * Keep the regular DTO type in endpoint metadata while reading the raw body
 * first. This prevents a JSON null root from reaching oatpp's DTO decoder.
 */
#define C4_REQUIRED_BODY_DTO(TYPE, ...) \
OATPP_MACRO_API_CONTROLLER_PARAM(C4_MACRO_REQUIRED_BODY_DTO, \
	OATPP_MACRO_API_CONTROLLER_BODY_DTO_INFO, TYPE, (__VA_ARGS__))

#define C4_MACRO_REQUIRED_BODY_DTO(TYPE, PARAM_LIST) \
if (!getDefaultObjectMapper()) { \
	return ApiController::handleError(Status::CODE_500, "Object mapper is unavailable."); \
} \
TYPE OATPP_MACRO_FIRSTARG PARAM_LIST; \
try { \
	const auto __c4_raw_body = __request->readBodyToString(); \
	const std::string __c4_body_text = __c4_raw_body ? \
		inpatientnurse::validation::trim(__c4_raw_body->c_str()) : std::string(); \
	if (__c4_body_text.empty() || __c4_body_text == "null") { \
		return ApiController::handleError(Status::CODE_400, "A JSON object body is required."); \
	} \
	OATPP_MACRO_FIRSTARG PARAM_LIST = \
		getDefaultObjectMapper()->readFromString<TYPE>(__c4_body_text.c_str()); \
} catch (const std::exception&) { \
	return ApiController::handleError(Status::CODE_400, "The JSON body is invalid."); \
} catch (...) { \
	return ApiController::handleError(Status::CODE_400, "The JSON body is invalid."); \
} \
if (!OATPP_MACRO_FIRSTARG PARAM_LIST) { \
	return ApiController::handleError(Status::CODE_400, "A valid JSON object body is required."); \
}

/* Convert malformed numeric query parameters into a client error. */
#undef API_HANDLER_ENDPOINT_QUERY_AUTH
#define API_HANDLER_ENDPOINT_QUERY_AUTH(_METHOD_, _PATH_, __ENDPOINT_FUN_NAME__, _QUERY_TYPE_, _EXECUTE_) \
ENDPOINT(_METHOD_, _PATH_, __ENDPOINT_FUN_NAME__, QUERIES(QueryParams, queryParams), API_HANDLER_AUTH_PARAME) { \
	_QUERY_TYPE_::Wrapper query; \
	try { \
		API_HANDLER_QUERY_PARAM(__c4_parsed_query, _QUERY_TYPE_, queryParams); \
		query = __c4_parsed_query; \
	} catch (const std::exception&) { \
		return ApiController::handleError(Status::CODE_400, "A query parameter has an invalid type."); \
	} catch (...) { \
		return ApiController::handleError(Status::CODE_400, "A query parameter has an invalid type."); \
	} \
	API_HANDLER_RESP_VO(_EXECUTE_); \
}

#endif // !_INPATIENT_NURSE_REQUEST_VALIDATION_H_
