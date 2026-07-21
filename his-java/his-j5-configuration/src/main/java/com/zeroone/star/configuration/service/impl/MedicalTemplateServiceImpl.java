package com.zeroone.star.configuration.service.impl;

import com.zeroone.star.configuration.entity.MedicalTemplate;
import com.zeroone.star.configuration.mapper.MedicalTemplateMapper;
import com.zeroone.star.configuration.service.IMedicalTemplateService;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.j5.emr.template.dto.MedicalTemplateCreateDTO;
import com.zeroone.star.project.j5.emr.template.dto.MedicalTemplateUpdateDTO;
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
            throw new IllegalArgumentException("新增病历模板参数不能为空");
        }

        String actorId = getCurrentUserId();
        String code = normalizeOptional(dto.getCode());
        ensureCodeAvailable(code, null);

        MedicalTemplate template = new MedicalTemplate();
        template.setName(requireText(dto.getName(), "模板名称不能为空"));
        template.setCode(code);
        template.setDocumentTypeCode(requireText(dto.getDocumentTypeCode(), "文书类型编码不能为空"));
        template.setEncounterClassEnum(dto.getEncounterClassEnum());
        template.setEncounterTypeEnum(dto.getEncounterTypeEnum());
        template.setContent(requireText(dto.getContent(), "模板内容不能为空"));
        template.setEnabled(dto.getEnabled() == null || dto.getEnabled());
        applyScope(template,
                normalizeScope(dto.getScopeLevel(), true),
                dto.getOrganizationId(),
                dto.getOwnerPractitionerId(),
                actorId,
                dto.getEncounterClassCode());

        LocalDateTime now = LocalDateTime.now();
        template.setCreatedAt(now);
        template.setUpdatedAt(now);
        template.setCreatedBy(actorId);
        template.setUpdatedBy(actorId);
        template.setDeleted(false);

        try {
            if (medicalTemplateMapper.insert(template) != 1) {
                throw new IllegalStateException("新增病历模板失败");
            }
        } catch (DuplicateKeyException ex) {
            throw new IllegalArgumentException("模板编码已存在: " + code, ex);
        }
        return template.getId();
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public String modifyMedicalTemplate(String id, MedicalTemplateUpdateDTO dto) {
        String templateId = requireText(id, "模板ID不能为空");
        if (dto == null) {
            throw new IllegalArgumentException("修改病历模板参数不能为空");
        }

        MedicalTemplate template = medicalTemplateMapper.selectById(templateId);
        if (template == null) {
            throw new IllegalArgumentException("病历模板不存在: " + templateId);
        }

        if (dto.getName() != null) {
            template.setName(requireText(dto.getName(), "模板名称不能为空"));
        }
        if (dto.getCode() != null) {
            String code = normalizeOptional(dto.getCode());
            ensureCodeAvailable(code, templateId);
            template.setCode(code);
        }
        if (dto.getDocumentTypeCode() != null) {
            template.setDocumentTypeCode(requireText(dto.getDocumentTypeCode(), "文书类型编码不能为空"));
        }
        if (dto.getEncounterClassEnum() != null) {
            template.setEncounterClassEnum(dto.getEncounterClassEnum());
        }
        if (dto.getEncounterTypeEnum() != null) {
            template.setEncounterTypeEnum(dto.getEncounterTypeEnum());
        }
        if (dto.getContent() != null) {
            template.setContent(requireText(dto.getContent(), "模板内容不能为空"));
        }
        if (dto.getEnabled() != null) {
            template.setEnabled(dto.getEnabled());
        }

        String actorId = getCurrentUserId();
        if (hasScopeUpdate(dto)) {
            String scope = dto.getScopeLevel() == null
                    ? normalizeScope(template.getScopeLevel(), true)
                    : normalizeScope(dto.getScopeLevel(), false);
            String organizationId = dto.getOrganizationId() == null
                    ? template.getOrganizationId()
                    : dto.getOrganizationId();
            String ownerPractitionerId = dto.getOwnerPractitionerId() == null
                    ? template.getOwnerPractitionerId()
                    : dto.getOwnerPractitionerId();
            String encounterClassCode = dto.getEncounterClassCode() == null
                    ? template.getEncounterClassCode()
                    : dto.getEncounterClassCode();
            applyScope(template, scope, organizationId, ownerPractitionerId, actorId, encounterClassCode);
        }

        template.setUpdatedAt(LocalDateTime.now());
        template.setUpdatedBy(actorId);
        try {
            if (medicalTemplateMapper.updateById(template) != 1) {
                throw new IllegalStateException("修改病历模板失败，模板可能已被删除");
            }
        } catch (DuplicateKeyException ex) {
            throw new IllegalArgumentException("模板编码已存在: " + template.getCode(), ex);
        }
        return templateId;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public void removeMedicalTemplate(String id) {
        String templateId = requireText(id, "模板ID不能为空");
        if (medicalTemplateMapper.softDeleteById(templateId, LocalDateTime.now(), getCurrentUserId()) != 1) {
            throw new IllegalArgumentException("病历模板不存在或已删除: " + templateId);
        }
    }

    private void ensureCodeAvailable(String code, String excludeId) {
        if (StringUtils.hasText(code) && medicalTemplateMapper.countByCode(code, excludeId) > 0) {
            throw new IllegalArgumentException("模板编码已存在: " + code);
        }
    }

    private void applyScope(MedicalTemplate template,
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
                    throw new IllegalArgumentException("科室模板必须选择适用科室");
                }
                normalizedOwnerId = null;
            }
            case SCOPE_PERSONAL -> {
                normalizedOrganizationId = null;
                if (!StringUtils.hasText(normalizedOwnerId)) {
                    normalizedOwnerId = currentUserId;
                }
                if (!StringUtils.hasText(normalizedOwnerId)) {
                    throw new IllegalArgumentException("个人模板必须指定所有者");
                }
            }
            default -> throw new IllegalArgumentException("无效的使用范围: " + scope);
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

    private static String normalizeScope(String raw, boolean useDefault) {
        if (!StringUtils.hasText(raw)) {
            if (useDefault) {
                return SCOPE_HOSPITAL;
            }
            throw new IllegalArgumentException("使用范围不能为空");
        }
        String scope = raw.trim().toUpperCase(Locale.ROOT);
        if (!VALID_SCOPES.contains(scope)) {
            throw new IllegalArgumentException("使用范围必须是 PERSONAL、DEPARTMENT 或 HOSPITAL");
        }
        return scope;
    }

    private static String normalizeEncounterClassCode(String raw) {
        if (!StringUtils.hasText(raw)) {
            throw new IllegalArgumentException("适用就诊类别不能为空");
        }
        return raw.trim().toUpperCase(Locale.ROOT);
    }

    private static String requireText(String raw, String message) {
        if (!StringUtils.hasText(raw)) {
            throw new IllegalArgumentException(message);
        }
        return raw.trim();
    }

    private static String normalizeOptional(String raw) {
        return StringUtils.hasText(raw) ? raw.trim() : null;
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
