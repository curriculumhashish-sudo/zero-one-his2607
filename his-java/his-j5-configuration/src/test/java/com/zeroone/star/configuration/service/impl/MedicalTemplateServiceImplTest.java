package com.zeroone.star.configuration.service.impl;

import com.zeroone.star.configuration.entity.MedicalTemplate;
import com.zeroone.star.configuration.mapper.MedicalTemplateMapper;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.j5.emr.template.dto.MedicalTemplateCreateDTO;
import com.zeroone.star.project.j5.emr.template.dto.MedicalTemplateUpdateDTO;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;

import java.time.LocalDateTime;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.mockito.ArgumentMatchers.any;
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
    void addMedicalTemplateDefaultsToHospitalAndNormalizesFields() {
        MedicalTemplateCreateDTO dto = validCreateDto();
        dto.setScopeLevel(null);
        dto.setOrganizationId("dept-1");
        dto.setOwnerPractitionerId("doctor-1");
        dto.setEncounterClassCode(" amb ");
        when(medicalTemplateMapper.countByCode("OP_RECORD_NEW", null)).thenReturn(0L);
        when(medicalTemplateMapper.insert(any(MedicalTemplate.class))).thenAnswer(invocation -> {
            MedicalTemplate entity = invocation.getArgument(0);
            entity.setId("template-1");
            return 1;
        });

        String id = service.addMedicalTemplate(dto);

        assertEquals("template-1", id);
        ArgumentCaptor<MedicalTemplate> captor = ArgumentCaptor.forClass(MedicalTemplate.class);
        verify(medicalTemplateMapper).insert(captor.capture());
        MedicalTemplate saved = captor.getValue();
        assertEquals("门诊病历", saved.getName());
        assertEquals("HOSPITAL", saved.getScopeLevel());
        assertEquals("AMB", saved.getEncounterClassCode());
        assertNull(saved.getOrganizationId());
        assertNull(saved.getOwnerPractitionerId());
        assertEquals("user-1001", saved.getCreatedBy());
        assertFalse(saved.getDeleted());
        assertNotNull(saved.getCreatedAt());
    }

    @Test
    void addPersonalTemplateUsesCurrentUserAsOwner() {
        MedicalTemplateCreateDTO dto = validCreateDto();
        dto.setScopeLevel("personal");
        dto.setOwnerPractitionerId(null);
        when(medicalTemplateMapper.countByCode("OP_RECORD_NEW", null)).thenReturn(0L);
        when(medicalTemplateMapper.insert(any(MedicalTemplate.class))).thenAnswer(invocation -> {
            MedicalTemplate entity = invocation.getArgument(0);
            entity.setId("template-2");
            return 1;
        });

        service.addMedicalTemplate(dto);

        ArgumentCaptor<MedicalTemplate> captor = ArgumentCaptor.forClass(MedicalTemplate.class);
        verify(medicalTemplateMapper).insert(captor.capture());
        assertEquals("PERSONAL", captor.getValue().getScopeLevel());
        assertEquals("user-1001", captor.getValue().getOwnerPractitionerId());
        assertNull(captor.getValue().getOrganizationId());
    }

    @Test
    void addDepartmentTemplateRequiresOrganization() {
        MedicalTemplateCreateDTO dto = validCreateDto();
        dto.setScopeLevel("DEPARTMENT");
        dto.setOrganizationId(" ");
        when(medicalTemplateMapper.countByCode("OP_RECORD_NEW", null)).thenReturn(0L);

        IllegalArgumentException exception = assertThrows(
                IllegalArgumentException.class,
                () -> service.addMedicalTemplate(dto));

        assertEquals("科室模板必须选择适用科室", exception.getMessage());
        verify(medicalTemplateMapper, never()).insert(any(MedicalTemplate.class));
    }

    @Test
    void addMedicalTemplateRejectsCodeUsedBySoftDeletedRecord() {
        MedicalTemplateCreateDTO dto = validCreateDto();
        when(medicalTemplateMapper.countByCode("OP_RECORD_NEW", null)).thenReturn(1L);

        IllegalArgumentException exception = assertThrows(
                IllegalArgumentException.class,
                () -> service.addMedicalTemplate(dto));

        assertEquals("模板编码已存在: OP_RECORD_NEW", exception.getMessage());
        verify(medicalTemplateMapper, never()).insert(any(MedicalTemplate.class));
    }

    @Test
    void modifyToHospitalClearsDepartmentAndOwner() {
        MedicalTemplate existing = new MedicalTemplate();
        existing.setId("template-3");
        existing.setName("旧模板");
        existing.setScopeLevel("DEPARTMENT");
        existing.setOrganizationId("dept-1");
        existing.setOwnerPractitionerId("doctor-1");
        existing.setEncounterClassCode("AMB");
        when(medicalTemplateMapper.selectById("template-3")).thenReturn(existing);
        when(medicalTemplateMapper.updateById(existing)).thenReturn(1);

        MedicalTemplateUpdateDTO dto = new MedicalTemplateUpdateDTO();
        dto.setName("新模板");
        dto.setScopeLevel("hospital");
        dto.setEncounterClassCode(" imp ");

        String id = service.modifyMedicalTemplate("template-3", dto);

        assertEquals("template-3", id);
        assertEquals("新模板", existing.getName());
        assertEquals("HOSPITAL", existing.getScopeLevel());
        assertEquals("IMP", existing.getEncounterClassCode());
        assertNull(existing.getOrganizationId());
        assertNull(existing.getOwnerPractitionerId());
        assertEquals("user-1001", existing.getUpdatedBy());
        assertNotNull(existing.getUpdatedAt());
    }

    @Test
    void removeMedicalTemplateWritesSoftDeleteAudit() {
        when(medicalTemplateMapper.softDeleteById(
                eq("template-4"), any(LocalDateTime.class), eq("user-1001")))
                .thenReturn(1);

        service.removeMedicalTemplate("template-4");

        verify(medicalTemplateMapper).softDeleteById(
                eq("template-4"), any(LocalDateTime.class), eq("user-1001"));
    }

    @Test
    void removeMissingTemplateReturnsClearError() {
        when(medicalTemplateMapper.softDeleteById(
                eq("missing"), any(LocalDateTime.class), eq("user-1001")))
                .thenReturn(0);

        IllegalArgumentException exception = assertThrows(
                IllegalArgumentException.class,
                () -> service.removeMedicalTemplate("missing"));

        assertEquals("病历模板不存在或已删除: missing", exception.getMessage());
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
}
