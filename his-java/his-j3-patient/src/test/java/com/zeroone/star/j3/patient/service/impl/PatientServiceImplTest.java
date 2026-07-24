package com.zeroone.star.j3.patient.service.impl;

import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.j3.patient.entity.PatientDO;
import com.zeroone.star.j3.patient.exception.PatientBusinessException;
import com.zeroone.star.j3.patient.mapper.PatientMapper;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.patient.PatientCreateDTO;
import com.zeroone.star.project.dto.j3.patient.PatientUpdateDTO;
import com.zeroone.star.project.query.j3.patient.PatientQuery;
import com.zeroone.star.project.vo.j3.patient.PatientListVO;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

@ExtendWith(MockitoExtension.class)
class PatientServiceImplTest {

    private static final String ORGANIZATION_ID = "322663105141149696";

    @Mock
    private PatientMapper patientMapper;

    private PatientServiceImpl patientService;

    @BeforeEach
    void setUp() {
        patientService = new PatientServiceImpl(patientMapper);
    }

    @Test
    void shouldQueryPageAndMaskSensitiveFields() {
        PatientQuery query = new PatientQuery();
        query.setPageIndex(1);
        query.setPageSize(10);
        query.setName(" 小王 ");

        PatientDO patient = activePatient();
        patient.setName("小王");
        patient.setIdCard("220321200105064567");
        patient.setPhone("13944894568");

        Page<PatientDO> mapperPage = new Page<>(1, 10);
        mapperPage.setTotal(1);
        mapperPage.setRecords(List.of(patient));
        when(patientMapper.selectPatientPage(any(), eq(query)))
                .thenReturn(mapperPage);

        PageDTO<PatientListVO> result =
                patientService.queryPatientPage(query);

        assertEquals("小王", query.getName());
        assertEquals(1L, result.getTotal());
        assertEquals(1, result.getRows().size());
        PatientListVO row = result.getRows().get(0);
        assertEquals("220321********4567", row.getIdCard());
        assertEquals("139****4568", row.getPhone());
        assertEquals("PA00000001", row.getBusNo());
    }

    @Test
    void shouldCreateFormalPatientWithPrototypeDefaults() {
        PatientCreateDTO dto = validCreateDto();
        dto.setName(" 照夜清 ");
        dto.setIdCard("11010119900101123x");
        dto.setPhone("13800001234");

        when(patientMapper.selectTenantIdByActiveOrganizationId(
                ORGANIZATION_ID
        )).thenReturn("1");
        when(patientMapper.selectIdByIdCardForUpdate(
                "11010119900101123X",
                null
        )).thenReturn(null);
        when(patientMapper.selectSequenceValueForUpdate("1:PA"))
                .thenReturn(5L);
        when(patientMapper.updateSequence(
                eq("1:PA"),
                eq(6L),
                any(LocalDateTime.class)
        )).thenReturn(1);
        when(patientMapper.insert(any(PatientDO.class)))
                .thenAnswer(invocation -> {
                    PatientDO patient = invocation.getArgument(0);
                    patient.setId("new-patient-id");
                    return 1;
                });

        String id = patientService.createPatient(dto);

        assertEquals("new-patient-id", id);
        ArgumentCaptor<PatientDO> captor =
                ArgumentCaptor.forClass(PatientDO.class);
        verify(patientMapper).insert(captor.capture());
        PatientDO inserted = captor.getValue();
        assertEquals("照夜清", inserted.getName());
        assertEquals("11010119900101123X", inserted.getIdCard());
        assertEquals("PA00000006", inserted.getBusNo());
        assertEquals("ZYQ", inserted.getPyStr());
        assertEquals("ACTIVE", inserted.getStatusEnum());
        assertTrue(inserted.getActiveFlag());
        assertFalse(inserted.getTempFlag());
        assertFalse(inserted.getIsDeleted());
        assertEquals(0, inserted.getVersion());
        assertEquals("1", inserted.getTenantId());
        assertNotNull(inserted.getCreatedAt());
    }

    @Test
    void shouldRejectDuplicateIdCardIncludingSoftDeletedPatient() {
        PatientCreateDTO dto = validCreateDto();
        dto.setIdCard("110101199001011237");
        when(patientMapper.selectTenantIdByActiveOrganizationId(
                ORGANIZATION_ID
        )).thenReturn("1");
        when(patientMapper.selectIdByIdCardForUpdate(
                "110101199001011237",
                null
        )).thenReturn("occupied-patient-id");

        PatientBusinessException exception = assertThrows(
                PatientBusinessException.class,
                () -> patientService.createPatient(dto)
        );

        assertEquals(
                "该身份证号已存在患者档案",
                exception.getMessage()
        );
        verify(patientMapper, never()).insert(any(PatientDO.class));
    }

    @Test
    void shouldRejectInvalidOrganization() {
        PatientCreateDTO dto = validCreateDto();
        when(patientMapper.selectTenantIdByActiveOrganizationId(
                ORGANIZATION_ID
        )).thenReturn(null);

        PatientBusinessException exception = assertThrows(
                PatientBusinessException.class,
                () -> patientService.createPatient(dto)
        );

        assertEquals(
                "建档机构不存在、已停用或已删除",
                exception.getMessage()
        );
    }

    @Test
    void shouldUpdateActivePatientWithOptimisticLock() {
        PatientDO patient = activePatient();
        when(patientMapper.selectById(patient.getId()))
                .thenReturn(patient);
        when(patientMapper.selectTenantIdByActiveOrganizationId(
                ORGANIZATION_ID
        )).thenReturn("1");
        when(patientMapper.selectIdByIdCardForUpdate(
                "110101199001011237",
                patient.getId()
        )).thenReturn(null);
        when(patientMapper.updateById(any(PatientDO.class)))
                .thenReturn(1);

        PatientUpdateDTO dto = validUpdateDto();
        dto.setName("照夜清");
        dto.setIdCard("110101199001011237");
        dto.setPhone("13700001234");

        patientService.updatePatient(patient.getId(), dto);

        ArgumentCaptor<PatientDO> captor =
                ArgumentCaptor.forClass(PatientDO.class);
        verify(patientMapper).updateById(captor.capture());
        PatientDO updated = captor.getValue();
        assertEquals("照夜清", updated.getName());
        assertEquals("ZYQ", updated.getPyStr());
        assertEquals("13700001234", updated.getPhone());
        assertEquals("ACTIVE", updated.getStatusEnum());
        assertTrue(updated.getActiveFlag());
        assertFalse(updated.getTempFlag());
        assertNotNull(updated.getUpdatedAt());
    }

    @Test
    void shouldRejectMissingPatientAndStaleVersion() {
        PatientUpdateDTO dto = validUpdateDto();
        when(patientMapper.selectById("missing")).thenReturn(null);

        PatientBusinessException missing = assertThrows(
                PatientBusinessException.class,
                () -> patientService.updatePatient("missing", dto)
        );
        assertEquals("患者不存在或已删除", missing.getMessage());

        PatientDO patient = activePatient();
        patient.setVersion(2);
        when(patientMapper.selectById(patient.getId()))
                .thenReturn(patient);

        PatientBusinessException stale = assertThrows(
                PatientBusinessException.class,
                () -> patientService.updatePatient(patient.getId(), dto)
        );
        assertEquals(
                "患者资料已被其他人修改，请刷新后重试",
                stale.getMessage()
        );
        verify(patientMapper, never()).updateById(any(PatientDO.class));
    }

    @Test
    void shouldRejectTerminalStateAndStateFieldChanges() {
        PatientUpdateDTO dto = validUpdateDto();
        PatientDO inactivePatient = activePatient();
        inactivePatient.setStatusEnum("INACTIVE");
        when(patientMapper.selectById(inactivePatient.getId()))
                .thenReturn(inactivePatient);

        PatientBusinessException terminal = assertThrows(
                PatientBusinessException.class,
                () -> patientService.updatePatient(
                        inactivePatient.getId(),
                        dto
                )
        );
        assertEquals(
                "当前患者状态不允许修改基本资料",
                terminal.getMessage()
        );

        PatientDO activePatient = activePatient();
        when(patientMapper.selectById(activePatient.getId()))
                .thenReturn(activePatient);
        dto.setActiveFlag(false);

        PatientBusinessException stateChange = assertThrows(
                PatientBusinessException.class,
                () -> patientService.updatePatient(
                        activePatient.getId(),
                        dto
                )
        );
        assertEquals(
                "普通资料修改不允许变更启用标记",
                stateChange.getMessage()
        );
    }

    @Test
    void shouldReportVersionConflictWhenDatabaseUpdateLosesRace() {
        PatientDO patient = activePatient();
        when(patientMapper.selectById(patient.getId()))
                .thenReturn(patient);
        when(patientMapper.selectTenantIdByActiveOrganizationId(
                ORGANIZATION_ID
        )).thenReturn("1");
        when(patientMapper.updateById(any(PatientDO.class)))
                .thenReturn(0);

        PatientBusinessException conflict = assertThrows(
                PatientBusinessException.class,
                () -> patientService.updatePatient(
                        patient.getId(),
                        validUpdateDto()
                )
        );

        assertEquals(
                "患者资料版本冲突，请刷新后重试",
                conflict.getMessage()
        );
    }

    private PatientCreateDTO validCreateDto() {
        PatientCreateDTO dto = new PatientCreateDTO();
        dto.setName("原型验收甲");
        dto.setGenderEnum("MALE");
        dto.setBirthDate(LocalDate.of(1990, 1, 1));
        dto.setOrganizationId(ORGANIZATION_ID);
        return dto;
    }

    private PatientUpdateDTO validUpdateDto() {
        PatientUpdateDTO dto = new PatientUpdateDTO();
        dto.setName("原型验收甲-修改");
        dto.setGenderEnum("MALE");
        dto.setBirthDate(LocalDate.of(1990, 1, 1));
        dto.setOrganizationId(ORGANIZATION_ID);
        dto.setStatusEnum("ACTIVE");
        dto.setActiveFlag(true);
        dto.setTempFlag(false);
        dto.setVersion(0);
        return dto;
    }

    private PatientDO activePatient() {
        PatientDO patient = new PatientDO();
        patient.setId("patient-id");
        patient.setTenantId("1");
        patient.setBusNo("PA00000001");
        patient.setName("原型验收甲");
        patient.setGenderEnum("MALE");
        patient.setStatusEnum("ACTIVE");
        patient.setActiveFlag(true);
        patient.setTempFlag(false);
        patient.setIsDeleted(false);
        patient.setVersion(0);
        patient.setOrganizationId(ORGANIZATION_ID);
        patient.setCreatedAt(LocalDateTime.now().minusDays(1));
        return patient;
    }
}
