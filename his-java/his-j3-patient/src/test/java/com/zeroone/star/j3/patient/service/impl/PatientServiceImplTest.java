package com.zeroone.star.j3.patient.service.impl;

import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.j3.patient.entity.PatientDO;
import com.zeroone.star.j3.patient.exception.PatientBusinessException;
import com.zeroone.star.j3.patient.mapper.PatientMapper;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j3.patient.PatientQuery;
import com.zeroone.star.project.vo.j3.patient.PatientOptionVO;
import com.zeroone.star.project.vo.j3.patient.PatientVO;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Nested;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;

import java.time.LocalDate;
import java.util.List;

import static org.assertj.core.api.Assertions.assertThat;
import static org.assertj.core.api.Assertions.assertThatThrownBy;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

/**
 * 患者管理三个新功能的单元测试：
 * <ol>
 *   <li>queryPatients — 获取选择患者列表</li>
 *   <li>getPatientById — 获取患者详情</li>
 *   <li>deletePatientById — 删除患者（逻辑删除）</li>
 * </ol>
 */
@ExtendWith(MockitoExtension.class)
@DisplayName("PatientServiceImpl 单元测试")
class PatientServiceImplTest {

    @Mock
    private PatientMapper patientMapper;

    @InjectMocks
    private PatientServiceImpl patientService;

    private PatientDO patient1;
    private PatientDO patient2;

    @BeforeEach
    void setUp() {
        patient1 = new PatientDO();
        patient1.setId("P001");
        patient1.setName("张三");
        patient1.setGenderEnum("MALE");
        patient1.setBirthDate(LocalDate.of(1990, 5, 15));
        patient1.setIdCard("110101199005152345");
        patient1.setPhone("13800138001");
        patient1.setStatusEnum("ACTIVE");
        patient1.setActiveFlag(true);
        patient1.setTempFlag(false);

        patient2 = new PatientDO();
        patient2.setId("P002");
        patient2.setName("李四");
        patient2.setGenderEnum("FEMALE");
        patient2.setBirthDate(LocalDate.of(1985, 10, 20));
        patient2.setIdCard("220101198510204567");
        patient2.setPhone("13900139002");
        patient2.setStatusEnum("ACTIVE");
        patient2.setActiveFlag(true);
        patient2.setTempFlag(false);
    }

    // ──────────────────── queryPatients ────────────────────

    @Nested
    @DisplayName("queryPatients — 获取选择患者列表")
    class QueryPatientsTests {

        @Test
        @DisplayName("有数据时返回分页结果")
        void shouldReturnPageWhenDataExists() {
            PatientQuery query = new PatientQuery();
            query.setPageIndex(1);
            query.setPageSize(10);

            Page<PatientDO> mockPage = new Page<>(1, 10);
            mockPage.setRecords(List.of(patient1, patient2));
            mockPage.setTotal(2);
            mockPage.setPages(1);

            when(patientMapper.selectPatientPage(any(), eq(query)))
                    .thenReturn(mockPage);

            PageDTO<PatientOptionVO> result = patientService.queryPatients(query);

            assertThat(result.getPageIndex()).isEqualTo(1L);
            assertThat(result.getPageSize()).isEqualTo(10L);
            assertThat(result.getTotal()).isEqualTo(2L);
            assertThat(result.getRows()).hasSize(2);

            PatientOptionVO first = result.getRows().get(0);
            assertThat(first.getId()).isEqualTo("P001");
            assertThat(first.getName()).isEqualTo("张三");
            assertThat(first.getGenderEnum()).isEqualTo("MALE");
            assertThat(first.getIdCard()).isEqualTo("110101199005152345");
        }

        @Test
        @DisplayName("无数据时返回空列表")
        void shouldReturnEmptyWhenNoData() {
            PatientQuery query = new PatientQuery();
            query.setPageIndex(1);
            query.setPageSize(10);

            Page<PatientDO> mockPage = new Page<>(1, 10);
            mockPage.setRecords(List.of());
            mockPage.setTotal(0);
            mockPage.setPages(0);

            when(patientMapper.selectPatientPage(any(), eq(query)))
                    .thenReturn(mockPage);

            PageDTO<PatientOptionVO> result = patientService.queryPatients(query);

            assertThat(result.getTotal()).isZero();
            assertThat(result.getRows()).isEmpty();
        }

        @Test
        @DisplayName("关键词为空时不拼接 LIKE 条件")
        void shouldNormalizeEmptyKeyword() {
            PatientQuery query = new PatientQuery();
            query.setPageIndex(1);
            query.setPageSize(10);
            query.setKeyword("  "); // 会被 trimToNull 转为 null

            Page<PatientDO> mockPage = new Page<>(1, 10);
            mockPage.setRecords(List.of(patient1));
            mockPage.setTotal(1);

            when(patientMapper.selectPatientPage(any(), eq(query)))
                    .thenReturn(mockPage);

            PageDTO<PatientOptionVO> result = patientService.queryPatients(query);

            assertThat(result.getTotal()).isEqualTo(1L);
            // keyword 已被 normalizeQuery 转为 null
            assertThat(query.getKeyword()).isNull();
        }
    }

    // ──────────────────── getPatientById ────────────────────

    @Nested
    @DisplayName("getPatientById — 获取患者详情")
    class GetPatientByIdTests {

        @Test
        @DisplayName("患者存在时返回完整详情")
        void shouldReturnPatientWhenExists() {
            when(patientMapper.selectById("P001")).thenReturn(patient1);

            PatientVO result = patientService.getPatientById("P001");

            assertThat(result.getId()).isEqualTo("P001");
            assertThat(result.getName()).isEqualTo("张三");
            assertThat(result.getGenderEnum()).isEqualTo("MALE");
            assertThat(result.getBirthDate()).isEqualTo(LocalDate.of(1990, 5, 15));
            assertThat(result.getIdCard()).isEqualTo("110101199005152345");
            assertThat(result.getPhone()).isEqualTo("13800138001");
            assertThat(result.getStatusEnum()).isEqualTo("ACTIVE");
            assertThat(result.getActiveFlag()).isTrue();
        }

        @Test
        @DisplayName("患者不存在时抛异常")
        void shouldThrowWhenPatientNotFound() {
            when(patientMapper.selectById("NOT_EXIST")).thenReturn(null);

            assertThatThrownBy(() -> patientService.getPatientById("NOT_EXIST"))
                    .isInstanceOf(PatientBusinessException.class)
                    .hasMessage("患者不存在或已删除");
        }

        @Test
        @DisplayName("已逻辑删除的患者查不到（@TableLogic 自动过滤）")
        void shouldReturnNullForDeletedPatient() {
            when(patientMapper.selectById("DELETED")).thenReturn(null);

            assertThatThrownBy(() -> patientService.getPatientById("DELETED"))
                    .isInstanceOf(PatientBusinessException.class)
                    .hasMessage("患者不存在或已删除");
        }
    }

    // ──────────────────── deletePatientById ────────────────────

    @Nested
    @DisplayName("deletePatientById — 删除患者（逻辑删除）")
    class DeletePatientByIdTests {

        @Test
        @DisplayName("正常删除已存在的患者")
        void shouldDeleteExistingPatient() {
            when(patientMapper.selectById("P001")).thenReturn(patient1);
            when(patientMapper.deleteById("P001")).thenReturn(1);

            patientService.deletePatientById("P001");

            verify(patientMapper).selectById("P001");
            verify(patientMapper).deleteById("P001");
        }

        @Test
        @DisplayName("ID为空时抛异常")
        void shouldThrowWhenIdIsNull() {
            assertThatThrownBy(() -> patientService.deletePatientById(null))
                    .isInstanceOf(PatientBusinessException.class)
                    .hasMessage("患者ID不能为空");
        }

        @Test
        @DisplayName("ID为空白字符串时抛异常")
        void shouldThrowWhenIdIsBlank() {
            assertThatThrownBy(() -> patientService.deletePatientById("   "))
                    .isInstanceOf(PatientBusinessException.class)
                    .hasMessage("患者ID不能为空");
        }

        @Test
        @DisplayName("患者不存在时抛异常")
        void shouldThrowWhenPatientNotFound() {
            when(patientMapper.selectById("NOT_EXIST")).thenReturn(null);

            assertThatThrownBy(() -> patientService.deletePatientById("NOT_EXIST"))
                    .isInstanceOf(PatientBusinessException.class)
                    .hasMessage("患者不存在或已删除");
        }

        @Test
        @DisplayName("重复删除已删除患者抛异常")
        void shouldThrowWhenPatientAlreadyDeleted() {
            when(patientMapper.selectById("DELETED")).thenReturn(null);

            assertThatThrownBy(() -> patientService.deletePatientById("DELETED"))
                    .isInstanceOf(PatientBusinessException.class)
                    .hasMessage("患者不存在或已删除");
        }
    }
}
