package com.zeroone.star.configuration.service.impl;

import com.zeroone.star.configuration.converter.MedicalTemplateConverter;
import com.zeroone.star.configuration.entity.MedicalTemplateDO;
import com.zeroone.star.configuration.exception.MedicalTemplateBusinessException;
import com.zeroone.star.configuration.mapper.MedicalTemplateMapper;
import com.zeroone.star.configuration.service.IMedicalTemplateService;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.j5.emr.MedicalTemplateCreateDTO;
import com.zeroone.star.project.dto.j5.emr.MedicalTemplateUpdateDTO;
import org.springframework.dao.DuplicateKeyException;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import java.time.LocalDateTime;
import java.util.Locale;
import java.util.Set;

/**
 * 病历模板写操作服务实现。
 */
@Service
public class MedicalTemplateServiceImpl implements IMedicalTemplateService {

    private static final String SCOPE_PERSONAL = "PERSONAL";
    private static final String SCOPE_DEPARTMENT = "DEPARTMENT";
    private static final String SCOPE_HOSPITAL = "HOSPITAL";
    private static final Set<String> VALID_SCOPES = Set.of(
            SCOPE_PERSONAL, SCOPE_DEPARTMENT, SCOPE_HOSPITAL);

    private final MedicalTemplateMapper medicalTemplateMapper;
    private final UserHolder userHolder;

    public MedicalTemplateServiceImpl(MedicalTemplateMapper medicalTemplateMapper, UserHolder userHolder) {
        this.medicalTemplateMapper = medicalTemplateMapper;
        this.userHolder = userHolder;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public String addMedicalTemplate(MedicalTemplateCreateDTO dto) {
        if (dto == null) {
            throw new MedicalTemplateBusinessException("新增病历模板参数不能为空");
        }

        MedicalTemplateDO template = MedicalTemplateConverter.toDO(dto);
        template.setName(requireText(dto.getName(), "模板名称不能为空"));
        template.setCode(normalizeOptional(dto.getCode()));
        template.setDocumentTypeCode(requireText(dto.getDocumentTypeCode(), "文书类型编码不能为空"));
        template.setContent(requireText(dto.getContent(), "模板内容不能为空"));
        template.setEnabled(dto.getEnabled() == null || dto.getEnabled());

        String actorId = getCurrentUserId();
        applyScope(template,
                normalizeScope(dto.getScopeLevel(), true),
                dto.getOrganizationId(),
                dto.getOwnerPractitionerId(),
                actorId,
                dto.getEncounterClassCode());
        ensureCodeAvailable(template.getCode(), null);

        LocalDateTime now = LocalDateTime.now();
        template.setCreatedAt(now);
        template.setUpdatedAt(now);
        template.setCreatedBy(actorId);
        template.setUpdatedBy(actorId);
        template.setDeleted(false);

        try {
            if (medicalTemplateMapper.insert(template) != 1) {
                throw new MedicalTemplateBusinessException("新增病历模板失败");
            }
        } catch (DuplicateKeyException ex) {
            throw duplicateCodeException(template.getCode(), ex);
        }
        if (!StringUtils.hasText(template.getId())) {
            throw new MedicalTemplateBusinessException("新增病历模板失败，未生成模板ID");
        }
        return template.getId();
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public String modifyMedicalTemplate(String id, MedicalTemplateUpdateDTO dto) {
        String templateId = requireText(id, "模板ID不能为空");
        if (dto == null) {
            throw new MedicalTemplateBusinessException("修改病历模板参数不能为空");
        }
        if (!hasUpdateField(dto)) {
            throw new MedicalTemplateBusinessException("至少提供一个需要修改的字段");
        }

        MedicalTemplateDO current = medicalTemplateMapper.selectScopeById(templateId);
        if (current == null) {
            throw new MedicalTemplateBusinessException("病历模板不存在: " + templateId);
        }

        MedicalTemplateDO update = MedicalTemplateConverter.toDO(dto);
        update.setId(templateId);
        normalizeBasicUpdateFields(update, dto);

        boolean codeChanged = dto.getCode() != null;
        if (codeChanged) {
            update.setCode(normalizeOptional(dto.getCode()));
            ensureCodeAvailable(update.getCode(), templateId);
        }

        String actorId = getCurrentUserId();
        boolean scopeChanged = hasScopeUpdate(dto);
        if (scopeChanged) {
            String scope = dto.getScopeLevel() == null
                    ? normalizeScope(current.getScopeLevel(), true)
                    : normalizeScope(dto.getScopeLevel(), false);
            String organizationId = dto.getOrganizationId() == null
                    ? current.getOrganizationId()
                    : dto.getOrganizationId();
            String ownerPractitionerId = dto.getOwnerPractitionerId() == null
                    ? current.getOwnerPractitionerId()
                    : dto.getOwnerPractitionerId();
            String encounterClassCode = dto.getEncounterClassCode() == null
                    ? current.getEncounterClassCode()
                    : dto.getEncounterClassCode();
            applyScope(update, scope, organizationId, ownerPractitionerId, actorId, encounterClassCode);
        }

        update.setUpdatedAt(LocalDateTime.now());
        update.setUpdatedBy(actorId);
        try {
            if (medicalTemplateMapper.updateSelectiveById(update, codeChanged, scopeChanged) != 1) {
                throw new MedicalTemplateBusinessException("修改病历模板失败，模板可能已被删除");
            }
        } catch (DuplicateKeyException ex) {
            throw duplicateCodeException(update.getCode(), ex);
        }
        return templateId;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public void removeMedicalTemplate(String id) {
        String templateId = requireText(id, "模板ID不能为空");
        if (medicalTemplateMapper.softDeleteById(templateId, LocalDateTime.now(), getCurrentUserId()) != 1) {
            throw new MedicalTemplateBusinessException("病历模板不存在或已删除: " + templateId);
        }
    }

    private void normalizeBasicUpdateFields(MedicalTemplateDO update, MedicalTemplateUpdateDTO dto) {
        if (dto.getName() != null) {
            update.setName(requireText(dto.getName(), "模板名称不能为空"));
        }
        if (dto.getDocumentTypeCode() != null) {
            update.setDocumentTypeCode(requireText(dto.getDocumentTypeCode(), "文书类型编码不能为空"));
        }
        if (dto.getContent() != null) {
            update.setContent(requireText(dto.getContent(), "模板内容不能为空"));
        }
    }

    private void ensureCodeAvailable(String code, String excludeId) {
        if (StringUtils.hasText(code) && medicalTemplateMapper.existsByCode(code, excludeId)) {
            throw new MedicalTemplateBusinessException("模板编码已存在: " + code);
        }
    }

    private void applyScope(MedicalTemplateDO template,
                            String scope,
                            String organizationId,
                            String ownerPractitionerId,
                            String currentUserId,
                            String encounterClassCode) {
        String normalizedOrganizationId = normalizeOptional(organizationId);
        String normalizedOwnerId = normalizeOptional(ownerPractitionerId);

        switch (scope) {
            case SCOPE_HOSPITAL -> {
                normalizedOrganizationId = null;
                normalizedOwnerId = null;
            }
            case SCOPE_DEPARTMENT -> {
                if (!StringUtils.hasText(normalizedOrganizationId)) {
                    throw new MedicalTemplateBusinessException("科室模板必须选择适用科室");
                }
                normalizedOwnerId = null;
            }
            case SCOPE_PERSONAL -> {
                normalizedOrganizationId = null;
                if (!StringUtils.hasText(normalizedOwnerId)) {
                    normalizedOwnerId = currentUserId;
                }
                if (!StringUtils.hasText(normalizedOwnerId)) {
                    throw new MedicalTemplateBusinessException("个人模板必须指定所有者");
                }
            }
            default -> throw new MedicalTemplateBusinessException("无效的使用范围: " + scope);
        }

        template.setScopeLevel(scope);
        template.setOrganizationId(normalizedOrganizationId);
        template.setOwnerPractitionerId(normalizedOwnerId);
        template.setEncounterClassCode(normalizeEncounterClassCode(encounterClassCode));
    }

    private static boolean hasScopeUpdate(MedicalTemplateUpdateDTO dto) {
        return dto.getScopeLevel() != null
                || dto.getOrganizationId() != null
                || dto.getOwnerPractitionerId() != null
                || dto.getEncounterClassCode() != null;
    }

    private static boolean hasUpdateField(MedicalTemplateUpdateDTO dto) {
        return dto.getName() != null
                || dto.getCode() != null
                || dto.getDocumentTypeCode() != null
                || dto.getOrganizationId() != null
                || dto.getEncounterClassEnum() != null
                || dto.getEncounterTypeEnum() != null
                || dto.getContent() != null
                || dto.getEnabled() != null
                || dto.getScopeLevel() != null
                || dto.getOwnerPractitionerId() != null
                || dto.getEncounterClassCode() != null;
    }

    private static String normalizeScope(String raw, boolean useDefault) {
        if (!StringUtils.hasText(raw)) {
            if (useDefault) {
                return SCOPE_HOSPITAL;
            }
            throw new MedicalTemplateBusinessException("使用范围不能为空");
        }
        String scope = raw.trim().toUpperCase(Locale.ROOT);
        if (!VALID_SCOPES.contains(scope)) {
            throw new MedicalTemplateBusinessException("使用范围必须是 PERSONAL、DEPARTMENT 或 HOSPITAL");
        }
        return scope;
    }

    private static String normalizeEncounterClassCode(String raw) {
        if (!StringUtils.hasText(raw)) {
            throw new MedicalTemplateBusinessException("适用就诊类别不能为空");
        }
        return raw.trim().toUpperCase(Locale.ROOT);
    }

    private static String requireText(String raw, String message) {
        if (!StringUtils.hasText(raw)) {
            throw new MedicalTemplateBusinessException(message);
        }
        return raw.trim();
    }

    private static String normalizeOptional(String raw) {
        return StringUtils.hasText(raw) ? raw.trim() : null;
    }

    private static MedicalTemplateBusinessException duplicateCodeException(String code, DuplicateKeyException cause) {
        String message = StringUtils.hasText(code)
                ? "模板编码已存在: " + code
                : "病历模板数据违反唯一约束";
        return new MedicalTemplateBusinessException(message, cause);
    }

    private String getCurrentUserId() {
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            return currentUser == null ? null : normalizeOptional(currentUser.getId());
        } catch (Exception ignored) {
            return null;
        }
    }
}
