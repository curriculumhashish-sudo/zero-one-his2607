#include "stdafx.h"
#include "SampleService.h"
#include "SimpleDateTimeFormat.h"
#include <chrono>
#include <random>
#include <sstream>

std::list<SampleNameDTO::Wrapper> SampleService::listSampleNames(const SampleNameQuery::Wrapper& query) const
{
	SampleDAO dao;
	const auto keyword = query ? toStdString(query->keyword) : std::string();
	const auto rows = dao.selectByKeyword(keyword, 0, 100);

	std::list<SampleNameDTO::Wrapper> result;
	for (const auto& row : rows)
	{
		auto dto = SampleNameDTO::createShared();
		dto->id = row->getId().c_str();
		dto->name = row->getName().c_str();
		result.push_back(dto);
	}
	return result;
}

SamplePageDTO::Wrapper SampleService::listSamplePage(const SampleQuery::Wrapper& query) const
{
	const auto pageIndex = query ? query->page_index.getValue(1) : 1;
	const auto pageSize = query ? query->page_size.getValue(10) : 10;
	const auto safePageIndex = pageIndex == 0 ? 1ULL : pageIndex;
	const auto safePageSize = pageSize == 0 ? 10ULL : pageSize;
	const auto offset = (safePageIndex - 1) * safePageSize;
	const auto keyword = query ? toStdString(query->keyword) : std::string();

	SampleDAO dao;
	auto page = SamplePageDTO::createShared();
	page->page_index = safePageIndex;
	page->page_size = safePageSize;
	page->rows = {};

	const auto rows = dao.selectByKeyword(keyword, offset, safePageSize);
	for (const auto& row : rows)
	{
		page->addData(toSampleDTO(row));
	}

	page->total = static_cast<v_int64>(dao.countByKeyword(keyword));
	page->calcPages();
	return page;
}

void SampleService::addSample(const SampleAddDTO::Wrapper& dto) const
{
	SampleDAO dao;
	const auto sampleCode = toStdString(firstPresent(dto->busNo, dto->sampleCode));
	const auto sampleName = toStdString(firstPresent(dto->name, dto->sampleName));
	if (dao.existsByCode(sampleCode))
	{
		throw std::runtime_error("sampleCode already exists.");
	}
	if (sampleCode.empty() || sampleName.empty())
	{
		throw std::runtime_error("busNo/name is required.");
	}

	SampleDO sample;
	sample.setId(buildSampleId());
	sample.setSampleCode(sampleCode);
	sample.setSampleName(sampleName);
	if (firstPresent(dto->pyStr, dto->inputCode)) sample.setInputCode(toStdString(firstPresent(dto->pyStr, dto->inputCode)));
	if (firstPresent(dto->wbStr, dto->wbCode)) sample.setWbCode(toStdString(firstPresent(dto->wbStr, dto->wbCode)));
	sample.setStatusEnum(resolveStatusEnum(dto->statusEnum, dto->status, 1));
	sample.setTypeEnum(dto->typeEnum ? toStdString(dto->typeEnum) : std::string(DEFAULT_SAMPLE_TYPE));
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

	const auto sampleCode = toStdString(firstPresent(dto->busNo, dto->sampleCode));
	const auto sampleName = toStdString(firstPresent(dto->name, dto->sampleName));
	if (dao.existsByCode(sampleCode, id))
	{
		throw std::runtime_error("sampleCode already exists.");
	}
	if (sampleCode.empty() || sampleName.empty())
	{
		throw std::runtime_error("busNo/name is required.");
	}

	SampleDO sample;
	sample.setId(id);
	sample.setSampleCode(sampleCode);
	sample.setSampleName(sampleName);
	if (firstPresent(dto->pyStr, dto->inputCode)) sample.setInputCode(toStdString(firstPresent(dto->pyStr, dto->inputCode)));
	if (firstPresent(dto->wbStr, dto->wbCode)) sample.setWbCode(toStdString(firstPresent(dto->wbStr, dto->wbCode)));
	if (dto->statusEnum || dto->status) sample.setStatusEnum(resolveStatusEnum(dto->statusEnum, dto->status));
	if (dto->typeEnum) sample.setTypeEnum(toStdString(dto->typeEnum));
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
		dto->statusEnum ? (toStdString(dto->statusEnum) == "ACTIVE" ? 1 : 0) : dto->status.getValue(0),
		DEFAULT_OPERATOR,
		currentTime());

	if (affected <= 0)
	{
		throw std::runtime_error("failed to switch sample status.");
	}
}

SampleDTO::Wrapper SampleService::toSampleDTO(const PtrSampleDO& sample)
{
	auto dto = SampleDTO::createShared();
	if (!sample)
	{
		return dto;
	}

	dto->id = sample->getId().c_str();
	dto->tenant_id = sample->getTenantId().c_str();
	dto->created_at = sample->getCreateTime().c_str();
	dto->updated_at = sample->getUpdateTime().c_str();
	dto->created_by = sample->getCreateBy().c_str();
	dto->updated_by = sample->getUpdateBy().c_str();
	dto->is_deleted = sample->getIsDeleted();
	dto->deleted_at = sample->getDeletedAt().c_str();
	dto->deleted_by = sample->getDeletedBy().c_str();
	dto->version = sample->getVersion();
	dto->bus_no = sample->getBusNo().c_str();
	dto->name = sample->getName().c_str();
	dto->py_str = sample->getPyStr().c_str();
	dto->wb_str = sample->getWbStr().c_str();
	dto->type_enum = sample->getTypeEnum().c_str();
	dto->status_enum = sample->getStatusEnum().c_str();
	return dto;
}

oatpp::String SampleService::firstPresent(const oatpp::String& primary, const oatpp::String& fallback)
{
	return primary && !primary->empty() ? primary : fallback;
}

std::string SampleService::resolveStatusEnum(const oatpp::String& statusEnum, const oatpp::Int32& status, int defaultStatus)
{
	if (statusEnum && !statusEnum->empty())
	{
		return toStdString(statusEnum);
	}
	return status ? (status.getValue(defaultStatus) == 1 ? "ACTIVE" : "INACTIVE")
		: (defaultStatus == 1 ? "ACTIVE" : "INACTIVE");
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
