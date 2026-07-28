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
class PatientCreateUpdateServiceTest {

    private static final String ORGANIZATION_ID =
            "322663105141149696";

    @Mock
    private PatientMapper patientMapper;

    private PatientServiceImpl patientService;

    @BeforeEach
    void setUp() {
        patientService = new PatientServiceImpl(patientMapper);
    }

    @Test
    void shouldReturnPrototypeListFieldsWithoutBackendMasking() {
        PatientQuery query = new PatientQuery();
        query.setPageIndex(1);
        query.setPageSize(10);

        PatientDO patient = activePatient();
        patient.setName("照夜清");
        patient.setGenderEnum("FEMALE");
        patient.setBirthDate(LocalDate.now().minusYears(26));
        patient.setIdCard("220321200105064567");
        patient.setPhone("13944894568");

        Page<PatientDO> mapperPage = new Page<>(1, 10);
        mapperPage.setTotal(1);
        mapperPage.setRecords(List.of(patient));
        when(patientMapper.selectPatientPage(any(), eq(query)))
                .thenReturn(mapperPage);

        PageDTO<PatientListVO> result =
                patientService.queryPatientPage(query);

        PatientListVO row = result.getRows().get(0);
        assertEquals(26, row.getAge());
        assertEquals("女性", row.getGenderEnumText());
        assertEquals("220321200105064567", row.getIdCard());
        assertEquals("13944894568", row.getPhone());
    }

    @Test
    void shouldCreatePatientWithPrototypeFields() {
        PatientCreateDTO dto = validCreateDto();
        dto.setName(" 照夜清 ");
        dto.setIdCard("11010119900101123x");
        dto.setPhone("13800001234");
        dto.setBloodAbo(1);
        dto.setBloodRh(2);
        dto.setDeceasedDate(LocalDateTime.now().minusDays(1));

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

        assertEquals(
                "new-patient-id",
                patientService.createPatient(dto)
        );

        ArgumentCaptor<PatientDO> captor =
                ArgumentCaptor.forClass(PatientDO.class);
        verify(patientMapper).insert(captor.capture());
        PatientDO inserted = captor.getValue();
        assertEquals("照夜清", inserted.getName());
        assertEquals("11010119900101123X", inserted.getIdCard());
        assertEquals("PA00000006", inserted.getBusNo());
        assertEquals("ZYQ", inserted.getPyStr());
        assertEquals(1, inserted.getBloodTypeAbo());
        assertEquals(2, inserted.getBloodTypeRh());
        assertEquals(dto.getDeceasedDate(), inserted.getDeceasedDate());
        assertEquals("ACTIVE", inserted.getStatusEnum());
        assertTrue(inserted.getActiveFlag());
        assertFalse(inserted.getTempFlag());
        assertNotNull(inserted.getCreatedAt());
    }

    @Test
    void shouldUpdateWithoutFrontendVersionButKeepOptimisticLock() {
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
        dto.setVersion(null);
        dto.setPhone("13700001234");

        patientService.updatePatient(patient.getId(), dto);

        ArgumentCaptor<PatientDO> captor =
                ArgumentCaptor.forClass(PatientDO.class);
        verify(patientMapper).updateById(captor.capture());
        PatientDO updated = captor.getValue();
        assertEquals("13700001234", updated.getPhone());
        assertEquals(0, updated.getVersion());
        assertNotNull(updated.getUpdatedAt());
    }

    @Test
    void shouldRejectBlankIdCardForCreateAndUpdate() {
        PatientCreateDTO createDTO = validCreateDto();
        createDTO.setIdCard(" ");

        assertThrows(
                PatientBusinessException.class,
                () -> patientService.createPatient(createDTO)
        );
        verify(patientMapper, never()).insert(any(PatientDO.class));

        PatientDO patient = activePatient();
        when(patientMapper.selectById(patient.getId()))
                .thenReturn(patient);
        PatientUpdateDTO updateDTO = validUpdateDto();
        updateDTO.setIdCard(null);

        assertThrows(
                PatientBusinessException.class,
                () -> patientService.updatePatient(
                        patient.getId(),
                        updateDTO
                )
        );
        verify(patientMapper, never()).updateById(any(PatientDO.class));
    }

    @Test
    void shouldRejectStaleVersionWhenFrontendProvidesIt() {
        PatientDO patient = activePatient();
        when(patientMapper.selectById(patient.getId()))
                .thenReturn(patient);
        PatientUpdateDTO dto = validUpdateDto();
        dto.setVersion(1);

        assertThrows(
                PatientBusinessException.class,
                () -> patientService.updatePatient(patient.getId(), dto)
        );
        verify(patientMapper, never()).updateById(any(PatientDO.class));
    }

    private PatientCreateDTO validCreateDto() {
        PatientCreateDTO dto = new PatientCreateDTO();
        dto.setName("原型验收患者");
        dto.setGenderEnum("MALE");
        dto.setIdCard("110101199001011237");
        dto.setAge(36);
        dto.setBirthDate(LocalDate.now().minusYears(36));
        dto.setOrganizationId(ORGANIZATION_ID);
        return dto;
    }

    private PatientUpdateDTO validUpdateDto() {
        PatientUpdateDTO dto = new PatientUpdateDTO();
        dto.setName("原型验收患者修改");
        dto.setGenderEnum("MALE");
        dto.setIdCard("110101199001011237");
        dto.setAge(36);
        dto.setBirthDate(LocalDate.now().minusYears(36));
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
        patient.setName("原型验收患者");
        patient.setGenderEnum("MALE");
        patient.setBirthDate(LocalDate.now().minusYears(36));
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
