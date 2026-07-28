#include "stdafx.h"
#include "OutpatientrecordService.h"
#include "id/SnowFlake.h"
#include "SimpleDateTimeFormat.h"
#include <sstream>

StringJsonVO::Wrapper OutpatientrecordService::save(
	const outpatientrecordSaveDTO::Wrapper& dto,
	const PayloadDTO& payload)
{
	auto resultVo = StringJsonVO::createShared();

	// ---- 1. Parameter validation ----
	auto patientId = dto->patientId;
	if (!patientId || patientId.getValue("") == "") {
		resultVo->code = 400;
		resultVo->message = "Parameter cannot be empty, please pass patient ID";
		return resultVo;
	}
	auto visitId = dto->visitId;
	if (!visitId || visitId.getValue("") == "") {
		resultVo->code = 400;
		resultVo->message = "Parameter cannot be empty, please pass visit ID";
		return resultVo;
	}
	auto templateId = dto->templateId;
	if (!templateId || templateId.getValue("") == "") {
		resultVo->code = 400;
		resultVo->message = "Parameter cannot be empty, please pass template ID";
		return resultVo;
	}
	auto content = dto->content;
	if (!content || content.getValue("") == "") {
		resultVo->code = 400;
		resultVo->message = "Parameter cannot be empty, please pass medical record content";
		return resultVo;
	}

	// ---- 2. Build common field values ----
	auto now = SimpleDateTimeFormat::format();
	auto currentUser = payload.getUsername();
	auto orgId = payload.getId();

	// Auto-generate title if not provided
	std::string titleValue;
	auto titleField = dto->title;
	if (titleField && titleField.getValue("") != "") {
		titleValue = titleField.getValue("");
	}
	else {
		std::ostringstream titleStream;
		titleStream << ZH_WORDS_GETTER("outpatientrecord.tags.tag") << "-" << now;
		titleValue = titleStream.str();
	}

	// ---- 3. Determine insert vs update ----
	auto recordId = dto->outpatientrecordId;
	auto snowFlake = SnowFlake::getInstance();

	if (!recordId || recordId.getValue("") == "") {
		// ===== INSERT new record =====
		EmrDocumentDO doc;
		doc.setId(std::to_string(snowFlake->nextId()));
		doc.setPatientId(patientId.getValue(""));
		doc.setEncounterId(visitId.getValue(""));
		doc.setDocumentTypeCode("OUTPATIENT_RECORD");
		doc.setTitle(titleValue);
		doc.setContent(content.getValue(""));
		doc.setStatus("DRAFT");
		doc.setSourceTemplateId(templateId.getValue(""));
		doc.setCreatedAt(now);
		doc.setCreatedBy(currentUser);
		doc.setTenantId(orgId);

		emrDocumentDAO.insert(doc);

		resultVo->success(oatpp::String(doc.getId().c_str()));
		resultVo->message = "Medical record saved successfully";
	}
	else {
		// ===== UPDATE existing record =====
		auto docId = recordId.getValue("");
		auto existing = emrDocumentDAO.selectById(docId);

		if (!existing) {
			resultVo->code = 404;
			resultVo->message = "Medical record not found, please check the record ID";
			return resultVo;
		}

		existing->setPatientId(patientId.getValue(""));
		existing->setEncounterId(visitId.getValue(""));
		existing->setTitle(titleValue);
		existing->setContent(content.getValue(""));
		existing->setStatus("DRAFT");
		existing->setSourceTemplateId(templateId.getValue(""));
		existing->setUpdatedAt(now);
		existing->setUpdatedBy(currentUser);

		emrDocumentDAO.update(*existing);

		resultVo->success(oatpp::String(docId.c_str()));
		resultVo->message = "Medical record updated successfully";
	}

	return resultVo;
}
