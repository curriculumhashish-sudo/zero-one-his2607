#include "stdafx.h"
#include "OutpatientrecordService.h"
#include "id/SnowFlake.h"               // 雪花分布式ID生成工具头文件
#include "SimpleDateTimeFormat.h"       // 统一时间格式化工具
#include <sstream>                      // C++字符串流，用于拼接自动病历标题

/**
 * @brief 门诊病历保存业务实现：自动区分新增/更新电子病历
 * @param dto 前端传入的保存请求DTO，包含病历、患者、就诊、模板、富文本内容等入参
 * @param payload JWT令牌载荷，存储当前登录医生账号、机构租户ID等鉴权信息
 * @return StringJsonVO::Wrapper 全局统一JSON返回体，成功时data为病历唯一ID
 * 业务流程：参数校验 → 通用基础数据组装 → 判断新增/更新分支 → 数据库操作 → 统一结果返回
 */
StringJsonVO::Wrapper OutpatientrecordService::save(
	const outpatientrecordSaveDTO::Wrapper& dto,
	const PayloadDTO& payload)
{
	// 初始化全局标准返回VO，用于存放接口状态码、提示信息、返回数据
	auto resultVo = StringJsonVO::createShared();

	// ===================== 1. 入参合法性校验 =====================
	// 获取前端传入患者ID
	auto patientId = dto->patientId;
	// 校验：字段为空 或 字符串无内容，返回400参数错误
	if (!patientId || patientId.getValue("") == "") {
		resultVo->code = 400;
		resultVo->message = "Parameter cannot be empty, please pass patient ID";
		return resultVo;
	}

	// 获取前端传入就诊流水ID（单次门诊挂号唯一标识）
	auto visitId = dto->visitId;
	if (!visitId || visitId.getValue("") == "") {
		resultVo->code = 400;
		resultVo->message = "Parameter cannot be empty, please pass visit ID";
		return resultVo;
	}

	// 获取前端使用的病历模板ID
	auto templateId = dto->templateId;
	if (!templateId || templateId.getValue("") == "") {
		resultVo->code = 400;
		resultVo->message = "Parameter cannot be empty, please pass template ID";
		return resultVo;
	}

	// 获取富文本病历正文（Tiptap结构化JSON）
	auto content = dto->content;
	if (!content || content.getValue("") == "") {
		resultVo->code = 400;
		resultVo->message = "Parameter cannot be empty, please pass medical record content";
		return resultVo;
	}

	// ===================== 2. 组装全局通用基础字段 =====================
	// 获取当前系统格式化时间（统一格式，用于创建/更新时间）
	auto now = SimpleDateTimeFormat::format();
	// 从JWT载荷读取当前操作医生账号
	auto currentUser = payload.getUsername();
	// 从JWT载荷读取租户机构ID（SaaS多医院数据隔离）
	auto orgId = payload.getId();

	// 病历标题自动生成逻辑
	std::string titleValue;
	auto titleField = dto->title;
	// 如果前端传了标题，直接使用传入值
	if (titleField && titleField.getValue("") != "") {
		titleValue = titleField.getValue("");
	}
	else {
		// 前端未传标题，自动拼接：【门诊病历】+ 当前时间作为默认标题
		std::ostringstream titleStream;
		titleStream << ZH_WORDS_GETTER("outpatientrecord.tags.tag") << "-" << now;
		titleValue = titleStream.str();
	}

	// ===================== 3. 根据病历ID区分「新增」/「更新」逻辑 =====================
	// 获取前端传入病历主键ID
	auto recordId = dto->outpatientrecordId;
	// 获取雪花ID单例实例，用于新增病历生成全局唯一主键
	auto snowFlake = SnowFlake::getInstance();

	// 分支1：病历ID为空 → 新增门诊病历
	if (!recordId || recordId.getValue("") == "") {
		// 初始化数据库实体DO，对应emr_document电子病历表
		EmrDocumentDO doc;
		// 雪花算法生成分布式唯一病历ID，转为字符串存入主键
		doc.setId(std::to_string(snowFlake->nextId()));
		// 设置关联患者ID
		doc.setPatientId(patientId.getValue(""));
		// 设置关联门诊就诊流水ID
		doc.setEncounterId(visitId.getValue(""));
		// 文档类型编码：固定标识为门诊病历
		doc.setDocumentTypeCode("OUTPATIENT_RECORD");
		// 设置病历标题（前端传入/自动生成）
		doc.setTitle(titleValue);
		// 设置富文本病历正文JSON
		doc.setContent(content.getValue(""));
		// 病历状态：草稿（DRAFT，未提交归档）
		doc.setStatus("DRAFT");
		// 记录当前病历基于哪一套模板创建
		doc.setSourceTemplateId(templateId.getValue(""));
		// 审计字段：创建时间
		doc.setCreatedAt(now);
		// 审计字段：创建操作人（当前登录医生）
		doc.setCreatedBy(currentUser);
		// 审计字段：租户机构ID，区分不同医院数据
		doc.setTenantId(orgId);

		// 调用DAO层执行数据库insert插入操作
		emrDocumentDAO.insert(doc);

		// 设置接口成功返回，data为新生成的病历ID
		resultVo->success(oatpp::String(doc.getId().c_str()));
		resultVo->message = "Medical record saved successfully";
	}
	// 分支2：病历ID有值 → 更新已有病历
	else {
		// 取出前端传入待更新的病历主键
		auto docId = recordId.getValue("");
		// 根据病历ID查询数据库原始病历记录
		auto existing = emrDocumentDAO.selectById(docId);

		// 校验：查询不到对应病历，返回404资源不存在
		if (!existing) {
			resultVo->code = 404;
			resultVo->message = "Medical record not found, please check the record ID";
			return resultVo;
		}

		// 覆盖更新业务字段
		existing->setPatientId(patientId.getValue(""));
		existing->setEncounterId(visitId.getValue(""));
		existing->setTitle(titleValue);
		existing->setContent(content.getValue(""));
		existing->setStatus("DRAFT"); // 更新后依旧保留草稿状态
		existing->setSourceTemplateId(templateId.getValue(""));
		// 更新审计字段：更新时间、更新人
		existing->setUpdatedAt(now);
		existing->setUpdatedBy(currentUser);

		// 调用DAO执行数据库update更新操作
		emrDocumentDAO.update(*existing);

		// 接口成功返回，data为本次更新的病历ID
		resultVo->success(oatpp::String(docId.c_str()));
		resultVo->message = "Medical record updated successfully";
	}

	// 返回组装完成的标准JSON响应
	return resultVo;
}