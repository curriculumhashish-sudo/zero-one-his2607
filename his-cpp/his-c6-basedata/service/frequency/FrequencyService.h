#pragma once
#ifndef FREQUENCY_SERVICE_H
#define FREQUENCY_SERVICE_H

#include <string>
#include <memory>
#include "oatpp/core/Types.hpp"
#include "domain/dto/frequency/FrequencyDTO.h"
#include "domain/query/frequency/FrequencyQuery.h"
#include "domain/vo/frequency/FrequencyVO.h"

class FrequencyService {
public:
	// 1. 新增频次
	bool addFrequency(const oatpp::Object<FrequencyDTO>& dto, std::string& errorMsg);

	// 2. 修改频次
	bool modifyFrequency(const oatpp::Object<FrequencyDTO>& dto, std::string& errorMsg);

	// 3. 修改启停状态
	bool modifyStatus(const std::string& id, int32_t targetStatus, std::string& errorMsg);

	// 4. 删除频次（逻辑删除）
	bool removeFrequency(const std::string& id, std::string& errorMsg);

	// 5. 分页查询频次列表
	FrequencyPageDTO::Wrapper listAll(const FrequencyQuery::Wrapper& query);

	// 6. 根据 ID 获取详情
	FrequencyDTO::Wrapper getById(const std::string& id);

	// 7. 下拉框/名称列表查询 (返回 FrequencyVO 列表)
	oatpp::List<FrequencyVO::Wrapper> getName(const FrequencyQuery::Wrapper& query);
};

#endif // FREQUENCY_SERVICE_H