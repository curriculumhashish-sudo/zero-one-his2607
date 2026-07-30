package com.zeroone.star.j3.patient.controller;

import com.zeroone.star.j3.patient.service.PatientService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.patient.PatientCreateDTO;
import com.zeroone.star.project.dto.j3.patient.PatientUpdateDTO;
import com.zeroone.star.project.query.j3.patient.PatientQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j3.patient.PatientListVO;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;

import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertSame;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

@ExtendWith(MockitoExtension.class)
class PatientControllerTest {

    @Mock
    private PatientService patientService;

    private PatientController patientController;

    @BeforeEach
    void setUp() {
        patientController = new PatientController(patientService);
    }

    @Test
    void shouldReturnServicePage() {
        PatientQuery query = new PatientQuery();
        query.setPageIndex(1);
        query.setPageSize(10);
        PageDTO<PatientListVO> page =
                new PageDTO<>(1L, 10L, 0L, 0L, List.of());
        when(patientService.queryPatientPage(query)).thenReturn(page);

        JsonVO<PageDTO<PatientListVO>> result =
                patientController.queryPatientPage(query);

        assertEquals(10000, result.getCode());
        assertSame(page, result.getData());
    }

    @Test
    void shouldDelegateCreateAndUpdate() {
        PatientCreateDTO createDTO = new PatientCreateDTO();
        JsonVO<String> createResult =
                patientController.createPatient(createDTO);
        assertEquals(10000, createResult.getCode());
        assertEquals("新增成功", createResult.getData());
        verify(patientService).createPatient(createDTO);

        PatientUpdateDTO updateDTO = new PatientUpdateDTO();
        JsonVO<String> updateResult =
                patientController.updatePatient("patient-id", updateDTO);
        assertEquals(10000, updateResult.getCode());
        assertEquals("修改成功", updateResult.getData());
        verify(patientService).updatePatient("patient-id", updateDTO);
    }
}
