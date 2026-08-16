package com.zeroone.star.configuration.service.impl;

import com.zeroone.star.configuration.entity.MedicalTemplateDO;
import com.zeroone.star.configuration.exception.MedicalTemplateBusinessException;
import com.zeroone.star.configuration.mapper.MedicalTemplateMapper;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.j5.emr.MedicalTemplateCreateDTO;
import com.zeroone.star.project.dto.j5.emr.MedicalTemplateUpdateDTO;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;
import org.springframework.dao.DuplicateKeyException;

import java.time.LocalDateTime;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyBoolean;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.lenient;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

@ExtendWith(MockitoExtension.class)
class MedicalTemplateServiceImplTest {

    @Mock
    private MedicalTemplateMapper medicalTemplateMapper;

    @Mock
    private UserHolder userHolder;

    private MedicalTemplateServiceImpl service;

    @BeforeEach
    void setUp() throws Exception {
        service = new MedicalTemplateServiceImpl(medicalTemplateMapper, userHolder);
        lenient().when(userHolder.getCurrentUser())
                .thenReturn(UserDTO.builder().id("user-1001").build());
    }

    @Test
    void addDefaultsToHospitalAndNormalizesFields() {
        MedicalTemplateCreateDTO dto = validCreateDto();
        dto.setScopeLevel(null);
        dto.setOrganizationId("dept-1");
        dto.setOwnerPractitionerId("doctor-1");
        dto.setEncounterClassCode(" amb ");
        stubSuccessfulInsert("template-1");

        String id = service.addMedicalTemplate(dto);

        assertEquals("template-1", id);
        ArgumentCaptor<MedicalTemplateDO> captor = ArgumentCaptor.forClass(MedicalTemplateDO.class);
        verify(medicalTemplateMapper).insert(captor.capture());
        MedicalTemplateDO saved = captor.getValue();
        assertEquals("门诊病历", saved.getName());
        assertEquals("OP_RECORD_NEW", saved.getCode());
        assertEquals("HOSPITAL", saved.getScopeLevel());
        assertEquals("AMB", saved.getEncounterClassCode());
        assertNull(saved.getOrganizationId());
        assertNull(saved.getOwnerPractitionerId());
        assertEquals("user-1001", saved.getCreatedBy());
        assertFalse(saved.getDeleted());
        assertTrue(saved.getEnabled());
        assertNotNull(saved.getCreatedAt());
        assertEquals(saved.getCreatedAt(), saved.getUpdatedAt());
    }

    @Test
    void addPersonalTemplateUsesCurrentUserAsOwner() {
        MedicalTemplateCreateDTO dto = validCreateDto();
        dto.setScopeLevel(" personal ");
        dto.setOwnerPractitionerId(null);
        stubSuccessfulInsert("template-2");

        service.addMedicalTemplate(dto);

        ArgumentCaptor<MedicalTemplateDO> captor = ArgumentCaptor.forClass(MedicalTemplateDO.class);
        verify(medicalTemplateMapper).insert(captor.capture());
        assertEquals("PERSONAL", captor.getValue().getScopeLevel());
        assertEquals("user-1001", captor.getValue().getOwnerPractitionerId());
        assertNull(captor.getValue().getOrganizationId());
    }

    @Test
    void addPersonalTemplateRejectsMissingOwnerAndCurrentUser() throws Exception {
        MedicalTemplateCreateDTO dto = validCreateDto();
        dto.setScopeLevel("PERSONAL");
        when(userHolder.getCurrentUser()).thenReturn(null);

        MedicalTemplateBusinessException exception = assertThrows(
                MedicalTemplateBusinessException.class,
                () -> service.addMedicalTemplate(dto));

        assertEquals("个人模板必须指定所有者", exception.getMessage());
        verify(medicalTemplateMapper, never()).insert(any(MedicalTemplateDO.class));
    }

    @Test
    void addDepartmentTemplateRequiresOrganization() {
        MedicalTemplateCreateDTO dto = validCreateDto();
        dto.setScopeLevel("DEPARTMENT");
        dto.setOrganizationId(" ");

        MedicalTemplateBusinessException exception = assertThrows(
                MedicalTemplateBusinessException.class,
                () -> service.addMedicalTemplate(dto));

        assertEquals("科室模板必须选择适用科室", exception.getMessage());
        verify(medicalTemplateMapper, never()).insert(any(MedicalTemplateDO.class));
    }

    @Test
    void addRejectsInvalidScope() {
        MedicalTemplateCreateDTO dto = validCreateDto();
        dto.setScopeLevel("WARD");

        MedicalTemplateBusinessException exception = assertThrows(
                MedicalTemplateBusinessException.class,
                () -> service.addMedicalTemplate(dto));

        assertEquals("使用范围必须是 PERSONAL、DEPARTMENT 或 HOSPITAL", exception.getMessage());
        verify(medicalTemplateMapper, never()).insert(any(MedicalTemplateDO.class));
    }

    @Test
    void addRejectsCodeUsedBySoftDeletedRecord() {
        MedicalTemplateCreateDTO dto = validCreateDto();
        when(medicalTemplateMapper.existsByCode("OP_RECORD_NEW", null)).thenReturn(true);

        MedicalTemplateBusinessException exception = assertThrows(
                MedicalTemplateBusinessException.class,
                () -> service.addMedicalTemplate(dto));

        assertEquals("模板编码已存在: OP_RECORD_NEW", exception.getMessage());
        verify(medicalTemplateMapper, never()).insert(any(MedicalTemplateDO.class));
    }

    @Test
    void addConvertsConcurrentUniqueKeyConflictToBusinessError() {
        MedicalTemplateCreateDTO dto = validCreateDto();
        when(medicalTemplateMapper.insert(any(MedicalTemplateDO.class)))
                .thenThrow(new DuplicateKeyException("duplicate"));

        MedicalTemplateBusinessException exception = assertThrows(
                MedicalTemplateBusinessException.class,
                () -> service.addMedicalTemplate(dto));

        assertEquals("模板编码已存在: OP_RECORD_NEW", exception.getMessage());
        assertTrue(exception.getCause() instanceof DuplicateKeyException);
    }

    @Test
    void addRejectsMissingGeneratedId() {
        MedicalTemplateCreateDTO dto = validCreateDto();
        when(medicalTemplateMapper.insert(any(MedicalTemplateDO.class))).thenReturn(1);

        MedicalTemplateBusinessException exception = assertThrows(
                MedicalTemplateBusinessException.class,
                () -> service.addMedicalTemplate(dto));

        assertEquals("新增病历模板失败，未生成模板ID", exception.getMessage());
    }

    @Test
    void modifyToHospitalClearsDepartmentAndOwner() {
        MedicalTemplateDO current = existingTemplate("DEPARTMENT", "dept-1", "doctor-1", "AMB");
        when(medicalTemplateMapper.selectScopeById("template-3")).thenReturn(current);
        when(medicalTemplateMapper.updateSelectiveById(any(MedicalTemplateDO.class), eq(false), eq(true)))
                .thenReturn(1);

        MedicalTemplateUpdateDTO dto = new MedicalTemplateUpdateDTO();
        dto.setName(" 新模板 ");
        dto.setScopeLevel("hospital");
        dto.setEncounterClassCode(" imp ");

        String id = service.modifyMedicalTemplate("template-3", dto);

        assertEquals("template-3", id);
        ArgumentCaptor<MedicalTemplateDO> captor = ArgumentCaptor.forClass(MedicalTemplateDO.class);
        verify(medicalTemplateMapper).updateSelectiveById(captor.capture(), eq(false), eq(true));
        MedicalTemplateDO update = captor.getValue();
        assertEquals("新模板", update.getName());
        assertEquals("HOSPITAL", update.getScopeLevel());
        assertEquals("IMP", update.getEncounterClassCode());
        assertNull(update.getOrganizationId());
        assertNull(update.getOwnerPractitionerId());
        assertEquals("user-1001", update.getUpdatedBy());
        assertNotNull(update.getUpdatedAt());
    }

    @Test
    void modifyBasicFieldDoesNotRewriteScopeColumns() {
        when(medicalTemplateMapper.selectScopeById("template-4"))
                .thenReturn(existingTemplate("HOSPITAL", null, null, "AMB"));
        when(medicalTemplateMapper.updateSelectiveById(any(MedicalTemplateDO.class), eq(false), eq(false)))
                .thenReturn(1);
        MedicalTemplateUpdateDTO dto = new MedicalTemplateUpdateDTO();
        dto.setEnabled(false);

        service.modifyMedicalTemplate("template-4", dto);

        ArgumentCaptor<MedicalTemplateDO> captor = ArgumentCaptor.forClass(MedicalTemplateDO.class);
        verify(medicalTemplateMapper).updateSelectiveById(captor.capture(), eq(false), eq(false));
        assertFalse(captor.getValue().getEnabled());
        assertNull(captor.getValue().getScopeLevel());
        assertNull(captor.getValue().getOrganizationId());
    }

    @Test
    void modifyAllowsClearingOptionalCode() {
        when(medicalTemplateMapper.selectScopeById("template-5"))
                .thenReturn(existingTemplate("HOSPITAL", null, null, "AMB"));
        when(medicalTemplateMapper.updateSelectiveById(any(MedicalTemplateDO.class), eq(true), eq(false)))
                .thenReturn(1);
        MedicalTemplateUpdateDTO dto = new MedicalTemplateUpdateDTO();
        dto.setCode(" ");

        service.modifyMedicalTemplate("template-5", dto);

        ArgumentCaptor<MedicalTemplateDO> captor = ArgumentCaptor.forClass(MedicalTemplateDO.class);
        verify(medicalTemplateMapper).updateSelectiveById(captor.capture(), eq(true), eq(false));
        assertNull(captor.getValue().getCode());
        verify(medicalTemplateMapper, never()).existsByCode(any(), any());
    }

    @Test
    void modifyRejectsDuplicateCode() {
        when(medicalTemplateMapper.selectScopeById("template-6"))
                .thenReturn(existingTemplate("HOSPITAL", null, null, "AMB"));
        when(medicalTemplateMapper.existsByCode("USED_CODE", "template-6")).thenReturn(true);
        MedicalTemplateUpdateDTO dto = new MedicalTemplateUpdateDTO();
        dto.setCode(" USED_CODE ");

        MedicalTemplateBusinessException exception = assertThrows(
                MedicalTemplateBusinessException.class,
                () -> service.modifyMedicalTemplate("template-6", dto));

        assertEquals("模板编码已存在: USED_CODE", exception.getMessage());
        verify(medicalTemplateMapper, never())
                .updateSelectiveById(any(MedicalTemplateDO.class), anyBoolean(), anyBoolean());
    }

    @Test
    void modifyScopeFieldRetainsExistingDepartment() {
        when(medicalTemplateMapper.selectScopeById("template-7"))
                .thenReturn(existingTemplate("DEPARTMENT", "dept-7", null, "IMP"));
        when(medicalTemplateMapper.updateSelectiveById(any(MedicalTemplateDO.class), eq(false), eq(true)))
                .thenReturn(1);
        MedicalTemplateUpdateDTO dto = new MedicalTemplateUpdateDTO();
        dto.setEncounterClassCode(" amb ");

        service.modifyMedicalTemplate("template-7", dto);

        ArgumentCaptor<MedicalTemplateDO> captor = ArgumentCaptor.forClass(MedicalTemplateDO.class);
        verify(medicalTemplateMapper).updateSelectiveById(captor.capture(), eq(false), eq(true));
        assertEquals("DEPARTMENT", captor.getValue().getScopeLevel());
        assertEquals("dept-7", captor.getValue().getOrganizationId());
        assertEquals("AMB", captor.getValue().getEncounterClassCode());
    }

    @Test
    void modifyToDepartmentRequiresOrganization() {
        when(medicalTemplateMapper.selectScopeById("template-8"))
                .thenReturn(existingTemplate("HOSPITAL", null, null, "AMB"));
        MedicalTemplateUpdateDTO dto = new MedicalTemplateUpdateDTO();
        dto.setScopeLevel("DEPARTMENT");

        MedicalTemplateBusinessException exception = assertThrows(
                MedicalTemplateBusinessException.class,
                () -> service.modifyMedicalTemplate("template-8", dto));

        assertEquals("科室模板必须选择适用科室", exception.getMessage());
        verify(medicalTemplateMapper, never())
                .updateSelectiveById(any(MedicalTemplateDO.class), anyBoolean(), anyBoolean());
    }

    @Test
    void modifyRejectsMissingTemplate() {
        when(medicalTemplateMapper.selectScopeById("missing")).thenReturn(null);
        MedicalTemplateUpdateDTO dto = new MedicalTemplateUpdateDTO();
        dto.setName("新模板");

        MedicalTemplateBusinessException exception = assertThrows(
                MedicalTemplateBusinessException.class,
                () -> service.modifyMedicalTemplate("missing", dto));

        assertEquals("病历模板不存在: missing", exception.getMessage());
    }

    @Test
    void modifyRejectsEmptyRequest() {
        MedicalTemplateBusinessException exception = assertThrows(
                MedicalTemplateBusinessException.class,
                () -> service.modifyMedicalTemplate("template-9", new MedicalTemplateUpdateDTO()));

        assertEquals("至少提供一个需要修改的字段", exception.getMessage());
        verify(medicalTemplateMapper, never()).selectScopeById(any());
    }

    @Test
    void removeWritesSoftDeleteAudit() {
        when(medicalTemplateMapper.softDeleteById(
                eq("template-10"), any(LocalDateTime.class), eq("user-1001")))
                .thenReturn(1);

        service.removeMedicalTemplate("template-10");

        verify(medicalTemplateMapper).softDeleteById(
                eq("template-10"), any(LocalDateTime.class), eq("user-1001"));
    }

    @Test
    void removeMissingTemplateReturnsClearError() {
        when(medicalTemplateMapper.softDeleteById(
                eq("missing"), any(LocalDateTime.class), eq("user-1001")))
                .thenReturn(0);

        MedicalTemplateBusinessException exception = assertThrows(
                MedicalTemplateBusinessException.class,
                () -> service.removeMedicalTemplate("missing"));

        assertEquals("病历模板不存在或已删除: missing", exception.getMessage());
    }

    @Test
    void removeRejectsBlankIdBeforeDatabaseAccess() {
        MedicalTemplateBusinessException exception = assertThrows(
                MedicalTemplateBusinessException.class,
                () -> service.removeMedicalTemplate(" "));

        assertEquals("模板ID不能为空", exception.getMessage());
        verify(medicalTemplateMapper, never())
                .softDeleteById(any(), any(LocalDateTime.class), any());
    }

    private void stubSuccessfulInsert(String id) {
        when(medicalTemplateMapper.insert(any(MedicalTemplateDO.class))).thenAnswer(invocation -> {
            MedicalTemplateDO template = invocation.getArgument(0);
            template.setId(id);
            return 1;
        });
    }

    private static MedicalTemplateCreateDTO validCreateDto() {
        MedicalTemplateCreateDTO dto = new MedicalTemplateCreateDTO();
        dto.setName(" 门诊病历 ");
        dto.setCode(" OP_RECORD_NEW ");
        dto.setDocumentTypeCode("OUTPATIENT_RECORD");
        dto.setContent("{\"type\":\"doc\"}");
        dto.setEncounterClassCode("AMB");
        return dto;
    }

    private static MedicalTemplateDO existingTemplate(String scope,
                                                       String organizationId,
                                                       String ownerId,
                                                       String encounterClassCode) {
        MedicalTemplateDO template = new MedicalTemplateDO();
        template.setScopeLevel(scope);
        template.setOrganizationId(organizationId);
        template.setOwnerPractitionerId(ownerId);
        template.setEncounterClassCode(encounterClassCode);
        return template;
    }
}
