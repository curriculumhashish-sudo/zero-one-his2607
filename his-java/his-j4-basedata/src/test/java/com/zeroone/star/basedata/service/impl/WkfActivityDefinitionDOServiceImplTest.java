package com.zeroone.star.basedata.service.impl;

import com.zeroone.star.basedata.entity.WkfActivityDefinitionDO;
import com.zeroone.star.basedata.mapper.TreatmentMapper;
import com.zeroone.star.basedata.service.ITreatmentService;
import com.zeroone.star.project.dto.j4.treatment.TreatmentDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentResponseDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentStatusDTO;
import jakarta.annotation.Resource;
import org.junit.jupiter.api.Test;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.transaction.annotation.Transactional;

import static org.junit.jupiter.api.Assertions.*;

/**
 * <p>
 * 描述：诊疗管理服务实现单元测试
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@SpringBootTest
@Transactional
class WkfActivityDefinitionDOServiceImplTest {

    @Resource
    private ITreatmentService treatmentService;

    @Resource
    private TreatmentMapper treatmentMapper;

    @Test
    void testAddTreatment() {
        TreatmentDTO treatmentDTO = new TreatmentDTO();
        treatmentDTO.setCategoryCode("TEST");
        treatmentDTO.setBusNo("TEST-BUS-001");
        treatmentDTO.setName("测试诊疗项目");
        treatmentDTO.setTypeEnum("TEST_TYPE");
        treatmentDTO.setStatusEnum("ACTIVE");

        TreatmentResponseDTO response = treatmentService.addTreatment(treatmentDTO);

        assertNotNull(response);
        assertNotNull(response.getId());
        assertEquals("测试诊疗项目", response.getName());
        assertEquals("ACTIVE", response.getStatusEnum());
        assertEquals(0, response.getIsDeleted());
    }

    @Test
    void testUpdateTreatment() {
        TreatmentDTO treatmentDTO = new TreatmentDTO();
        treatmentDTO.setCategoryCode("TEST");
        treatmentDTO.setBusNo("TEST-BUS-002");
        treatmentDTO.setName("测试诊疗项目2");
        treatmentDTO.setTypeEnum("TEST_TYPE");
        treatmentDTO.setStatusEnum("ACTIVE");

        TreatmentResponseDTO createResponse = treatmentService.addTreatment(treatmentDTO);

        TreatmentDTO updateDTO = new TreatmentDTO();
        updateDTO.setId(createResponse.getId());
        updateDTO.setCategoryCode("TEST");
        updateDTO.setBusNo("TEST-BUS-002");
        updateDTO.setName("修改后的诊疗项目");
        updateDTO.setTypeEnum("TEST_TYPE");
        updateDTO.setStatusEnum("ACTIVE");

        TreatmentResponseDTO updateResponse = treatmentService.updateTreatment(updateDTO);

        assertNotNull(updateResponse);
        assertEquals("修改后的诊疗项目", updateResponse.getName());
    }

    @Test
    void testUpdateTreatmentNotFound() {
        TreatmentDTO treatmentDTO = new TreatmentDTO();
        treatmentDTO.setId("non-existent-id");
        treatmentDTO.setCategoryCode("TEST");
        treatmentDTO.setBusNo("TEST-BUS");
        treatmentDTO.setName("测试");
        treatmentDTO.setTypeEnum("TEST_TYPE");
        treatmentDTO.setStatusEnum("ACTIVE");

        RuntimeException exception = assertThrows(RuntimeException.class, () -> {
            treatmentService.updateTreatment(treatmentDTO);
        });
        assertEquals("诊疗项目不存在", exception.getMessage());
    }

    @Test
    void testDeleteTreatment() {
        TreatmentDTO treatmentDTO = new TreatmentDTO();
        treatmentDTO.setCategoryCode("TEST");
        treatmentDTO.setBusNo("TEST-BUS-003");
        treatmentDTO.setName("测试诊疗项目3");
        treatmentDTO.setTypeEnum("TEST_TYPE");
        treatmentDTO.setStatusEnum("ACTIVE");

        TreatmentResponseDTO createResponse = treatmentService.addTreatment(treatmentDTO);

        Boolean result = treatmentService.deleteTreatment(createResponse.getId());

        assertTrue(result);

        WkfActivityDefinitionDO wkfActivityDefinitionDO = treatmentMapper.selectById(createResponse.getId());
        assertNotNull(wkfActivityDefinitionDO);
        assertEquals(1, wkfActivityDefinitionDO.getIsDeleted());
    }

    @Test
    void testDeleteTreatmentNotFound() {
        Boolean result = treatmentService.deleteTreatment("non-existent-id");
        assertFalse(result);
    }

    @Test
    void testUpdateStatus() {
        TreatmentDTO treatmentDTO = new TreatmentDTO();
        treatmentDTO.setCategoryCode("TEST");
        treatmentDTO.setBusNo("TEST-BUS-004");
        treatmentDTO.setName("测试诊疗项目4");
        treatmentDTO.setTypeEnum("TEST_TYPE");
        treatmentDTO.setStatusEnum("ACTIVE");

        TreatmentResponseDTO createResponse = treatmentService.addTreatment(treatmentDTO);

        TreatmentStatusDTO statusDTO = new TreatmentStatusDTO();
        statusDTO.setId(createResponse.getId());
        statusDTO.setStatusEnum("INACTIVE");

        TreatmentResponseDTO updateResponse = treatmentService.updateStatus(statusDTO);

        assertNotNull(updateResponse);
        assertEquals("INACTIVE", updateResponse.getStatusEnum());
    }

    @Test
    void testUpdateStatusNotFound() {
        TreatmentStatusDTO statusDTO = new TreatmentStatusDTO();
        statusDTO.setId("non-existent-id");
        statusDTO.setStatusEnum("INACTIVE");

        RuntimeException exception = assertThrows(RuntimeException.class, () -> {
            treatmentService.updateStatus(statusDTO);
        });
        assertEquals("诊疗项目不存在", exception.getMessage());
    }
}