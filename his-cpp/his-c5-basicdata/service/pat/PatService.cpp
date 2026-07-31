#include "stdafx.h"
#include "PatService.h"
#include "../../dao/pat/PatientDAO.h"
#include "SimpleDateTimeFormat.h"
#include <ctime>

std::string PatService::toGenderLabel(const std::string& genderEnum)
{
	if (genderEnum == "MALE" || genderEnum == "1")
	{
		return ZH_WORDS_GETTER("pat.select.gender.male");
	}
	if (genderEnum == "FEMALE" || genderEnum == "2")
	{
		return ZH_WORDS_GETTER("pat.select.gender.female");
	}
	return genderEnum;
}

int PatService::toGenderCode(const std::string& genderEnum)
{
	if (genderEnum == "MALE" || genderEnum == "1")
	{
		return 1;
	}
	if (genderEnum == "FEMALE" || genderEnum == "2")
	{
		return 2;
	}
	return 0;
}

int PatService::calcAge(const std::string& birthDate)
{
	if (birthDate.size() < 10)
	{
		return 0;
	}

	int year = 0;
	int month = 0;
	int day = 0;
	try
	{
		year = std::stoi(birthDate.substr(0, 4));
		month = std::stoi(birthDate.substr(5, 2));
		day = std::stoi(birthDate.substr(8, 2));
	}
	catch (...)
	{
		return 0;
	}

	if (year <= 0 || month < 1 || month > 12 || day < 1 || day > 31)
	{
		return 0;
	}

	std::time_t now = std::time(nullptr);
	std::tm localTm{};
#ifdef _WIN32
	localtime_s(&localTm, &now);
#else
	localtime_r(&now, &localTm);
#endif

	int age = (localTm.tm_year + 1900) - year;
	if ((localTm.tm_mon + 1) < month ||
		((localTm.tm_mon + 1) == month && localTm.tm_mday < day))
	{
		age--;
	}
	return age < 0 ? 0 : age;
}

PageDTO<PatSelectDTO::Wrapper>::Wrapper PatService::querySelectPatPage(const PatSelectPageQuery::Wrapper& query)
{
	auto pageData = PageDTO<PatSelectDTO::Wrapper>::createShared();
	pageData->pageIndex = query->pageIndex.getValue(1);
	pageData->pageSize = query->pageSize.getValue(20);
	pageData->rows = oatpp::List<PatSelectDTO::Wrapper>::createShared();

	PatientDAO patientDao;
	uint64_t totalCount = patientDao.countPatList(query);
	pageData->total = static_cast<int64_t>(totalCount);
	pageData->calcPages();

	if (totalCount == 0)
	{
		return pageData;
	}

	auto patientList = patientDao.selectPatList(query);
	for (const auto& patientDO : patientList)
	{
		auto dto = PatSelectDTO::createShared();
		dto->busNo = patientDO->getBusNo();
		dto->name = patientDO->getName();
		dto->gender = toGenderLabel(patientDO->getGender());
		dto->birthDate = patientDO->getBirthDate();
		dto->age = calcAge(patientDO->getBirthDate());
		dto->phone = patientDO->getPhone();
		dto->idCard = patientDO->getIdCard();
		pageData->addData(dto);
	}

	return pageData;
}

PageDTO<PatDTO::Wrapper>::Wrapper PatService::queryPatPage(const PatPageQuery::Wrapper& query)
{
	auto pageData = PageDTO<PatDTO::Wrapper>::createShared();
	pageData->pageIndex = query->pageIndex.getValue(1);
	pageData->pageSize = query->pageSize.getValue(10);
	pageData->rows = oatpp::List<PatDTO::Wrapper>::createShared();

	PatientDAO patientDao;
	uint64_t totalCount = patientDao.countPatPage(query);
	pageData->total = static_cast<int64_t>(totalCount);
	pageData->calcPages();

	if (totalCount == 0)
	{
		return pageData;
	}

	auto patientList = patientDao.queryPatPage(query);
	for (const auto& patientDO : patientList)
	{
		auto dto = PatDTO::createShared();
		// Map plain string fields DO -> DTO
		ZO_STAR_DOMAIN_DO_TO_DTO_1(dto, patientDO, name, Name, idCard, IdCard, phone, Phone);
		// Gender enum (MALE/FEMALE) -> numeric code (1 male / 2 female)
		dto->gender = toGenderCode(patientDO->getGender());
		// Age derived from birth date
		dto->age = calcAge(patientDO->getBirthDate());
		pageData->addData(dto);
	}

	return pageData;
}

bool PatService::removePatient(const std::string& id, const PayloadDTO& payload)
{
	PatientDAO dao;
	std::string now = SimpleDateTimeFormat::getFormatNow();
	std::string actor = payload.getUsername();
	if (actor.empty() || actor == "none")
	{
		actor = payload.getId();
	}
	if (actor.empty())
	{
		actor = "unknown";
	}
	int affected = dao.softDeletePatient(id, now, actor);
	return affected > 0;
}
