#include "stdafx.h"
#include "SampleService.h"
#include "SimpleDateTimeFormat.h"
#include <chrono>
#include <random>
#include <sstream>

void SampleService::addSample(const SampleAddDTO::Wrapper& dto) const
{
	SampleDAO dao;
	const auto sampleCode = toStdString(dto->sampleCode);
	if (dao.existsByCode(sampleCode))
	{
		throw std::runtime_error("sampleCode already exists.");
	}

	SampleDO sample;
	sample.setId(buildSampleId());
	sample.setSampleCode(sampleCode);
	sample.setSampleName(toStdString(dto->sampleName));
	if (dto->inputCode) sample.setInputCode(toStdString(dto->inputCode));
	if (dto->wbCode) sample.setWbCode(toStdString(dto->wbCode));
	sample.setStatus(dto->status ? dto->status.getValue(1) : 1);
	sample.setTypeEnum(DEFAULT_SAMPLE_TYPE);
	if (dto->remark) sample.setRemark(toStdString(dto->remark));

	const auto now = currentTime();
	sample.setCreateBy(DEFAULT_OPERATOR);
	sample.setCreateTime(now);
	sample.setUpdateBy(DEFAULT_OPERATOR);
	sample.setUpdateTime(now);

	if (dao.insertSample(sample) <= 0)
	{
		throw std::runtime_error("failed to add sample.");
	}
}

void SampleService::editSample(const SampleEditDTO::Wrapper& dto) const
{
	SampleDAO dao;
	const auto id = toStdString(dto->id);
	auto existing = dao.selectById(id);
	if (!existing)
	{
		throw std::runtime_error("sample not found.");
	}

	const auto sampleCode = toStdString(dto->sampleCode);
	if (dao.existsByCode(sampleCode, id))
	{
		throw std::runtime_error("sampleCode already exists.");
	}

	SampleDO sample;
	sample.setId(id);
	sample.setSampleCode(sampleCode);
	sample.setSampleName(toStdString(dto->sampleName));
	if (dto->inputCode) sample.setInputCode(toStdString(dto->inputCode));
	if (dto->wbCode) sample.setWbCode(toStdString(dto->wbCode));
	if (dto->status) sample.setStatus(dto->status.getValue(0));
	if (dto->remark) sample.setRemark(toStdString(dto->remark));
	sample.setUpdateBy(DEFAULT_OPERATOR);
	sample.setUpdateTime(currentTime());

	if (dao.updateSample(sample) <= 0)
	{
		throw std::runtime_error("failed to edit sample.");
	}
}

void SampleService::removeSample(const std::string& id) const
{
	SampleDAO dao;
	if (!dao.selectById(id))
	{
		throw std::runtime_error("sample not found.");
	}

	if (dao.removeSampleById(id) <= 0)
	{
		throw std::runtime_error("failed to remove sample.");
	}
}

void SampleService::switchSampleStatus(const SampleStatusDTO::Wrapper& dto) const
{
	SampleDAO dao;
	std::list<std::string> ids;
	for (const auto& id : *dto->ids)
	{
		if (id && !id->empty())
		{
			ids.push_back(toStdString(id));
		}
	}

	if (ids.empty())
	{
		throw std::runtime_error("ids is required.");
	}

	const auto affected = dao.updateStatusByIds(
		ids,
		dto->status.getValue(0),
		DEFAULT_OPERATOR,
		currentTime());

	if (affected <= 0)
	{
		throw std::runtime_error("failed to switch sample status.");
	}
}

std::string SampleService::buildSampleId() const
{
	const auto now = std::chrono::system_clock::now().time_since_epoch();
	const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
	static thread_local std::mt19937_64 engine(std::random_device{}());
	std::uniform_int_distribution<int> dist(1000, 9999);

	std::ostringstream oss;
	oss << "SAMPLE" << millis << dist(engine);
	return oss.str();
}

std::string SampleService::currentTime() const
{
	return SimpleDateTimeFormat::format();
}

std::string SampleService::toStdString(const oatpp::String& value)
{
	return value ? value.getValue("") : std::string();
}
