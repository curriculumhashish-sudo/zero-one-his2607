#include "stdafx.h"
#include "PrescriptionService.h"
#include "../../domain/dto/prescription/PrescriptionHistoryDTO.h"
#include "../../dao/prescription/PrescriptionDAO.h"
#include "domain/dto/PayloadDTO.h"

#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>

namespace
{
	// Extract every medicine name from JSON fields {"medicine_name": "<name>"}.
	oatpp::List<oatpp::String> parseMedicineNames(const std::string& json)
	{
		auto result = oatpp::List<oatpp::String>::createShared();
		if (json.empty())
		{
			return result;
		}
		const std::string key = "\"medicine_name\"";
		size_t pos = 0;
		while ((pos = json.find(key, pos)) != std::string::npos)
		{
			pos = json.find(':', pos);
			if (pos == std::string::npos)
			{
				break;
			}
			++pos;
			pos = json.find('"', pos);
			if (pos == std::string::npos)
			{
				break;
			}
			++pos;
			size_t endPos = json.find('"', pos);
			if (endPos == std::string::npos)
			{
				break;
			}
			result->push_back(oatpp::String(json.substr(pos, endPos - pos)));
			pos = endPos + 1;
		}
		return result;
	}

	// Append the UTF-8 encoding of a BMP code point to out.
	void appendUtf8(std::string& out, unsigned int cp)
	{
		if (cp <= 0x7F)
		{
			out.push_back(static_cast<char>(cp));
		}
		else if (cp <= 0x7FF)
		{
			out.push_back(static_cast<char>(0xC0 | (cp >> 6)));
			out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
		}
		else
		{
			out.push_back(static_cast<char>(0xE0 | (cp >> 12)));
			out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
			out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
		}
	}

	// Unescape a JSON string body (the content between the surrounding quotes).
	std::string jsonUnescape(const std::string& s)
	{
		std::string out;
		out.reserve(s.size());
		for (size_t i = 0; i < s.size(); ++i)
		{
			char ch = s[i];
			if (ch != '\\' || i + 1 >= s.size())
			{
				out.push_back(ch);
				continue;
			}
			char nx = s[++i];
			switch (nx)
			{
			case '"': out.push_back('"'); break;
			case '\\': out.push_back('\\'); break;
			case '/': out.push_back('/'); break;
			case 'n': out.push_back('\n'); break;
			case 't': out.push_back('\t'); break;
			case 'r': out.push_back('\r'); break;
			case 'b': out.push_back('\b'); break;
			case 'f': out.push_back('\f'); break;
			case 'u':
				if (i + 4 < s.size())
				{
					unsigned int cp = static_cast<unsigned int>(
						std::strtoul(s.substr(i + 1, 4).c_str(), nullptr, 16));
					appendUtf8(out, cp);
					i += 4;
				}
				break;
			default: out.push_back(nx); break;
			}
		}
		return out;
	}

	struct JsonScalar
	{
		std::string text; // unescaped text (empty when the element is null)
		bool isNull = false;
	};

	// Parse the JSON array-of-arrays produced by
	//   JSON_ARRAYAGG(JSON_ARRAY(m1.id, m2.id, m2.name, m1.dose,
	//                            a2.price_value, t2.display))
	// into PrescriptionCurrentMedicineDTO objects, following the fixed positional order.
	oatpp::List<PrescriptionCurrentMedicineDTO::Wrapper>
	parseMedicineLines(const std::string& json)
	{
		auto lines =
			oatpp::List<PrescriptionCurrentMedicineDTO::Wrapper>::createShared();
		size_t i = 0;
		const size_t n = json.size();

		while (i < n && json[i] != '[') ++i;
		if (i >= n) return lines;
		++i;

		while (i < n)
		{
			while (i < n &&
				(json[i] == ',' || std::isspace(static_cast<unsigned char>(json[i]))))
			{
				++i;
			}
			if (i >= n || json[i] == ']') break;
			if (json[i] != '[') { ++i; continue; }
			++i;

			std::vector<JsonScalar> fields;
			while (i < n)
			{
				while (i < n &&
					(json[i] == ',' ||
						std::isspace(static_cast<unsigned char>(json[i]))))
				{
					++i;
				}
				if (i >= n) break;
				if (json[i] == ']') { ++i; break; }

				JsonScalar sc;
				if (json[i] == '"')
				{
					++i;
					std::string raw;
					while (i < n)
					{
						char ch = json[i];
						if (ch == '\\' && i + 1 < n)
						{
							raw.push_back(ch);
							raw.push_back(json[i + 1]);
							i += 2;
							continue;
						}
						if (ch == '"') { ++i; break; }
						raw.push_back(ch);
						++i;
					}
					sc.text = jsonUnescape(raw);
				}
				else
				{
					std::string raw;
					while (i < n && json[i] != ',' && json[i] != ']')
					{
						raw.push_back(json[i]);
						++i;
					}
					size_t a = raw.find_first_not_of(" \t\r\n");
					size_t b = raw.find_last_not_of(" \t\r\n");
					if (a == std::string::npos) raw.clear();
					else raw = raw.substr(a, b - a + 1);
					if (raw == "null") sc.isNull = true;
					else sc.text = raw;
				}
				fields.push_back(sc);
			}

			auto med = PrescriptionCurrentMedicineDTO::createShared();
			const auto getText = [&fields](size_t index) -> oatpp::String
			{
				if (index >= fields.size() || fields[index].isNull)
				{
					return oatpp::String("");
				}
				return oatpp::String(fields[index].text);
			};
			const auto getNumber = [&fields](size_t index) -> v_float64
			{
				if (index >= fields.size() || fields[index].isNull || fields[index].text.empty())
				{
					return 0.0;
				}
				return static_cast<v_float64>(std::strtod(fields[index].text.c_str(), nullptr));
			};

			med->medicationRequestId = getText(0);
			med->medicationDefinitionId = getText(1);
			// Skip rows whose medication request or definition id is missing.
			if (med->medicationRequestId.getValue("").empty() ||
				med->medicationDefinitionId.getValue("").empty()) {
				continue;
			}
			med->medicationName = getText(2);
			med->dose = getNumber(3);
			med->retailUnitPrice = getNumber(4);
			med->unitName = getText(5);

			lines->push_back(med);
		}

		return lines;
	}
}

PrescriptionHistoryListDTO::Wrapper PrescriptionService::listAllHistoryPrescription(
	const PrescriptionHistoryQuery::Wrapper& query)
{
	auto historyList = PrescriptionHistoryListDTO::createShared();
	historyList->historyRows = oatpp::List<PrescriptionHistoryDTO::Wrapper>::createShared();

	if (!query->id || !query->patientId) {
		return historyList;
	}

	PrescriptionDAO dao;
	uint64_t count = dao.countHistory(query);
	if (count <= 0)
	{
		return historyList;
	}

	std::list<PrescriptionHistorySummaryDO> result =
		dao.selectHistoryPrescription(query);

	for (const PrescriptionHistorySummaryDO& summary : result)
	{
		auto dto = PrescriptionHistoryDTO::createShared();
		dto->id = oatpp::String(summary.getPrescriptionNo());
		dto->prescriptionGroupKey = oatpp::String(summary.getPrescriptionGroupKey());
		dto->encounterId = oatpp::String(summary.getEncounterId());
		if (dto->encounterId.getValue("").empty()) { continue; }
		dto->authoredAt = oatpp::String(summary.getAuthoredTime());
		dto->summaryText = static_cast<v_int32>(summary.getMedicationCount());
		dto->medicationNames = parseMedicineNames(summary.getMedicationNamesJson());
		historyList->historyRows->push_back(dto);
	}
	return historyList;
}

PrescriptionCurrentListDTO::Wrapper PrescriptionService::listAllCurrentPrescription(
	const PrescriptionCurrentQuery::Wrapper& query)
{
	auto currentList = PrescriptionCurrentListDTO::createShared();
	currentList->prescriptions = oatpp::List<PrescriptionCurrentDTO::Wrapper>::createShared();

	if (!query->id || !query->patientId) {
		return currentList;
	}

	PrescriptionDAO dao;
	uint64_t count = dao.countCurrent(query);
	if (count <= 0)
	{
		return currentList;
	}

	std::list<PrescriptionCurrentSummaryDO> result =
		dao.selectCurrentPrescription(query);

	for (const PrescriptionCurrentSummaryDO& summary : result)
	{
		auto dto = PrescriptionCurrentDTO::createShared();
		dto->id = oatpp::String(summary.getPrescriptionNo());
		// Skip rows whose prescription number is missing.
		if (dto->id.getValue("").empty()) { continue; }
		dto->statusEnum = oatpp::String(summary.getStatusEnum());
		dto->prescriptionGroupKey = oatpp::String(summary.getPrescriptionGroupKey());
		dto->therapyEnum = oatpp::String(summary.getTherapyEnum());
		dto->methodName = oatpp::String(summary.getMethodDisplay());
		dto->rateName = oatpp::String(summary.getFrequencyName());
		dto->days = summary.getDispensePerDuration();
		dto->chineseHerbsDoseQuantity =
			static_cast<v_int32>(summary.getChineseHerbsDoseQuantity());
		dto->sufferingFlag = summary.getSufferingFlag();
		dto->description = oatpp::String(summary.getDiagnosisDescription());
		dto->remark = oatpp::String(summary.getRemark());
		dto->lines = parseMedicineLines(summary.getMedicationDetailsJson());
		currentList->prescriptions->push_back(dto);
	}
	return currentList;
}

PrescriptionNewResponseDTO::Wrapper PrescriptionService::savePrescription(const PrescriptionNewDTO::Wrapper& dto, const PayloadDTO& payload)
{
	std::string errmsg = dto->validate();
	if (errmsg != "")
	{
		auto res = PrescriptionNewResponseDTO::createShared();
		res->success = false;
		res->message = errmsg;
		return res;
	}

	// Map top-level scalar fields from DTO to DO.
	PrescriptionNewSummaryDO data;
	ZO_STAR_DOMAIN_DTO_TO_DO(data, dto,
		EncounterId, encounterId,
		PatientId,   patientId,
		TenantId,    tenantId);
	data.setCreateAt(std::to_string(payload.Iat()));

	// oatpp::List cannot be handled by the macro; copy prescriptions manually.
	std::list<PrescriptionNewPrDTO::Wrapper> prList;
	if (dto->prescriptions)
	{
		for (const auto& pr : *dto->prescriptions)
		{
			prList.push_back(pr);
		}
	}
	data.setPrescriptions(prList);

	PrescriptionDAO dao;
	oatpp::List<oatpp::String> idList = dao.insertNewPrescription(data);
	
	auto res = PrescriptionNewResponseDTO::createShared();
	if (!idList->empty())
	{
		res->newMedicationRequestIds = idList;
		res->success = true;
		res->message = "add success";
	}
	else
	{
		res->success = false;
		res->message = "add fail";
	}
	return res;
}

PrescriptionDeleteResponseDTO::Wrapper PrescriptionService::removePrescription(const PrescriptionDeleteDTO::Wrapper& dto) {
	std::string errmsg = dto->validate();
	if (errmsg != "")
	{
		auto res = PrescriptionDeleteResponseDTO::createShared();
		res->success = false;
		res->message = errmsg;
		return res;
	}

	PrescriptionDeleteSummaryDO data;
	ZO_STAR_DOMAIN_DTO_TO_DO(data, dto,
		EncounterId, encounterId,
		PatientId,   patientId,
		TenantId,    tenantId);

	std::list<std::string> medicationRequestIds;
	if (dto->id)
	{
		for (const auto& one : *dto->id)
		{
			medicationRequestIds.push_back(one.getValue(""));
		}
	}
	data.setMedicationRequestIds(medicationRequestIds);

	PrescriptionDAO dao;
	int out = dao.deleteById(data);

	auto res = PrescriptionDeleteResponseDTO::createShared();
	if (out > 0) {
		res->success = true;
		res->message = "delete success";
	}
	else {
		res->success = false;
		res->message = "delete fail";
	}
	return res;
}