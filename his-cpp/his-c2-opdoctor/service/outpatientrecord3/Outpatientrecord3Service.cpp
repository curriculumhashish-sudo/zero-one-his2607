#include "stdafx.h"
#include "Outpatientrecord3Service.h"
#include "id/SnowFlake.h"               // 雪花算法ID生成头文件
#include "SimpleDateTimeFormat.h"       // 统一时间格式化工具
#include <sstream>                      // C++字符串流，用于动态拼接文本

/**
 * @brief 门诊病历业务实现，新增病历/修改病历逻辑
 * @param dto 门诊病历保存DTO，包含患者、就诊、模板、病历内容等信息
 * @param payload JWT载体信息，存储当前登录用户名称、租户ID及权限信息
 * @return StringJsonVO::Wrapper 全局统一JSON返回对象，成功时data为病历唯一ID
 * 业务执行流程：参数校验 → 组装通用字段 → 区分新增/更新逻辑 → 数据库操作 → 统一返回结果
 */
StringJsonVO::Wrapper outpatientrecord3Service::save(
	const outpatientrecord3SaveDTO::Wrapper& dto,
	const PayloadDTO& payload)
{
	// 初始化标准返回VO，预置接口状态码、提示信息容器
	auto resultVo = StringJsonVO::createShared();

	// ===================== 1. 参数校验 =====================
	// 获取就诊患者ID
	auto patientId = dto->patientId;
	// 校验字段非空，为空返回400错误
	if (!patientId || patientId.getValue("") == "") {
		resultVo->code = 400;
		resultVo->message = "Parameter cannot be empty, please pass patient ID";
		return resultVo;
	}

	// 获取就诊会话ID，标识一次就诊唯一标识
	auto encounterId = dto->encounterId;
	if (!encounterId || encounterId.getValue("") == "") {
		resultVo->code = 400;
		resultVo->message = "Parameter cannot be empty, please pass encounter ID";
		return resultVo;
	}

	// 获取当前使用的病历模板ID
	auto templateId = dto->templateId;
	if (!templateId || templateId.getValue("") == "") {
		resultVo->code = 400;
		resultVo->message = "Parameter cannot be empty, please pass template ID";
		return resultVo;
	}

	// 获取病历正文，Tiptap结构JSON字符串
	auto content = dto->content;
	if (!content || content.getValue("") == "") {
		resultVo->code = 400;
		resultVo->message = "Parameter cannot be empty, please pass medical record content";
		return resultVo;
	}

	// ===================== 2. 组装全局通用字段 =====================
	// 获取系统当前格式化时间（用于创建/更新时间）
	auto now = SimpleDateTimeFormat::format();
	// 从JWT载体读取当前登录用户名
	auto currentUser = payload.getUsername();
	// 从JWT载体读取租户ID（SaaS多租户数据隔离）
	auto orgId = payload.getId();

	// 病历标题自动生成逻辑
	std::string titleValue;
	auto titleField = dto->title;
	// 如果前端传入标题，直接使用传入值
	if (titleField && titleField.getValue("") != "") {
		titleValue = titleField.getValue("");
	}
	else {
		// 前端未传标题，自动拼接：门诊病历 + 当前时间作为默认标题
		std::ostringstream titleStream;
		titleStream << ZH_WORDS_GETTER("outpatientrecord3.tags.tag") << "-" << now;
		titleValue = titleStream.str();
	}

	// ===================== 3. 根据记录ID区分新增/更新逻辑 =====================
	// 获取门诊病历主键ID
	auto recordId = dto->outpatientrecord3Id;
	// 获取雪花算法实例，生成全局唯一主键
	auto snowFlake = SnowFlake::getInstance();

	// 分支1：主键ID为空 → 新增门诊病历
	if (!recordId || recordId.getValue("") == "") {
		// 初始化数据库实体DO，对应emr_document数据表
		EmrDocument3DO doc;
		// 雪花算法生成分布式唯一ID，转为字符串存储
		doc.setId(std::to_string(snowFlake->nextId()));
		// 设置患者ID
		doc.setPatientId(patientId.getValue(""));
		// 设置就诊会话ID
		doc.setEncounterId(encounterId.getValue(""));
		// 文档类型标识：门诊病历
		doc.setDocumentTypeCode("OUTPATIENT_RECORD");
		// 设置病历标题（前端传入/自动生成）
		doc.setTitle(titleValue);
		// 设置病历富文本JSON内容
		doc.setContent(content.getValue(""));
		// 病历状态：草稿（未签发）
		doc.setStatus("DRAFT");
		// 记录当前选用的模板ID
		doc.setSourceTemplateId(templateId.getValue(""));
		// 设置创建时间
		doc.setCreatedAt(now);
		// 设置创建人：当前登录用户
		doc.setCreatedBy(currentUser);
		// 设置租户ID，隔离不同租户数据
		doc.setTenantId(orgId);

		// 调用DAO执行数据库insert插入操作
		EmrDocument3DAO.insert(doc);

		// 接口成功返回，data为新增生成的病历ID
		resultVo->success(oatpp::String(doc.getId().c_str()));
		resultVo->message = "Medical record saved successfully";
	}
	// 分支2：主键ID存在 → 更新已有病历
	else {
		// 获取待修改的病历主键
		auto docId = recordId.getValue("");
		// 根据主键查询数据库原始病历记录
		auto existing = EmrDocument3DAO.selectById(docId);

		// 校验记录是否存在，不存在返回404
		if (!existing) {
			resultVo->code = 404;
			resultVo->message = "Medical record not found, please check the record ID";
			return resultVo;
		}

		// 更新业务字段
		existing->setPatientId(patientId.getValue(""));
		existing->setEncounterId(encounterId.getValue(""));
		existing->setTitle(titleValue);
		existing->setContent(content.getValue(""));
		existing->setStatus("DRAFT"); // 更新后仍保留草稿状态
		existing->setSourceTemplateId(templateId.getValue(""));
		// 更新更新时间、更新人
		existing->setUpdatedAt(now);
		existing->setUpdatedBy(currentUser);

		// 调用DAO执行数据库update更新操作
		EmrDocument3DAO.update(*existing);

		// 接口成功返回，data为被修改的病历ID
		resultVo->success(oatpp::String(docId.c_str()));
		resultVo->message = "Medical record updated successfully";
	}

	// 返回组装完成的标准JSON响应
	return resultVo;
}