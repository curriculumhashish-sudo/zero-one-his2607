#pragma once


#ifndef COSTINFO_QUERY
#define COSTINFO_QUERY

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)


// ��ȡ��ϸ�б���query
class CostInfoQuery :public PageQuery
{
	DTO_INIT(CostInfoQuery, PageQuery);

	// ��ʼʱ��
	DTO_FIELD(String, start_time);
	DTO_FIELD_INFO(start_time) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.query.start_time");
		info->required = true;
	}

	// ����ʱ��
	DTO_FIELD(String, end_time);
	DTO_FIELD_INFO(end_time) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.query.end_time");
		info->required = true;
	}

	// ��������
	DTO_FIELD(String, name);
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.query.name");
	}

	// �Һſ���
	DTO_FIELD(String, room);
	DTO_FIELD_INFO(room) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.query.room");
	}

	// �շѴ���
	DTO_FIELD(String, feeClass);
	DTO_FIELD_INFO(feeClass) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.query.feeClass");
	}

	// ϸ����
	DTO_FIELD(String, detailClass);
	DTO_FIELD_INFO(detailClass) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.query.detailClass");
	}

	// ��Ŀ����
	DTO_FIELD(String, projectName);
	DTO_FIELD_INFO(projectName) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.query.projectName");
	}

	// Ժ�ڱ��
	DTO_FIELD(String, hisID);
	DTO_FIELD_INFO(hisID) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.query.hisID");
	}

	// ����Ա
	DTO_FIELD(String, checker);
	DTO_FIELD_INFO(checker) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.query.checker");
	}

	// ���˷���
	DTO_FIELD(String, feeDir);
	DTO_FIELD_INFO(feeDir) {
		info->description = ZH_WORDS_GETTER("rhqm.form.cost_info.query.feeDir");
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif