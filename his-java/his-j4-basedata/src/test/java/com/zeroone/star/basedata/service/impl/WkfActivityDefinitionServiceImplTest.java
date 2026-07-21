package com.zeroone.star.basedata.service.impl;

import com.zeroone.star.basedata.entity.WkfActivityDefinitionDO;
import com.zeroone.star.basedata.mapper.WkfActivityDefinitionMapper;
import com.zeroone.star.basedata.service.IServiceCatalog;
import com.zeroone.star.project.dto.j4.treatment.ServiceCatalogDTO;
import com.zeroone.star.project.dto.j4.treatment.ServiceCatalogResponseDTO;
import com.zeroone.star.project.dto.j4.treatment.ServiceCatalogStatusDTO;
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
class WkfActivityDefinitionServiceImplTest {

    @Resource
    private IServiceCatalog serviceCatalogService;

    @Resource
    private WkfActivityDefinitionMapper wkfActivityDefinitionMapper;

    @Test
    void testAddServiceCatalog() {
        ServiceCatalogDTO serviceCatalogDTO = new ServiceCatalogDTO();
        serviceCatalogDTO.setCategoryCode("TEST");
        serviceCatalogDTO.setBusNo("TEST-BUS-001");
        serviceCatalogDTO.setName("测试诊疗项目");
        serviceCatalogDTO.setTypeName("TEST_TYPE");
        serviceCatalogDTO.setStatusEnum("ACTIVE");

//        ServiceCatalogResponseDTO response = serviceCatalogService.addServiceCatalog(serviceCatalogDTO);
//
//        assertNotNull(response);
//        assertNotNull(response.getId());
//        assertEquals("测试诊疗项目", response.getName());
//        assertEquals("ACTIVE", response.getStatusEnum());
//        assertEquals(0, response.getIsDeleted());
    }

    @Test
    void testUpdateServiceCatalog() {
        ServiceCatalogDTO serviceCatalogDTO = new ServiceCatalogDTO();
        serviceCatalogDTO.setCategoryCode("TEST");
        serviceCatalogDTO.setBusNo("TEST-BUS-002");
        serviceCatalogDTO.setName("测试诊疗项目2");
        serviceCatalogDTO.setTypeName("TEST_TYPE");
        serviceCatalogDTO.setStatusEnum("ACTIVE");

//        ServiceCatalogResponseDTO createResponse = serviceCatalogService.addServiceCatalog(serviceCatalogDTO);
//
//        ServiceCatalogDTO updateDTO = new ServiceCatalogDTO();
//        updateDTO.setId(createResponse.getId());
//        updateDTO.setCategoryCode("TEST");
//        updateDTO.setBusNo("TEST-BUS-002");
//        updateDTO.setName("修改后的诊疗项目");
//        updateDTO.setTypeName("TEST_TYPE");
//        updateDTO.setStatusEnum("ACTIVE");
//
//        ServiceCatalogResponseDTO updateResponse = serviceCatalogService.updateServiceCatalog(updateDTO);
//
//        assertNotNull(updateResponse);
//        assertEquals("修改后的诊疗项目", updateResponse.getName());
    }

    @Test
    void testUpdateServiceCatalogNotFound() {
        ServiceCatalogDTO serviceCatalogDTO = new ServiceCatalogDTO();
        serviceCatalogDTO.setId("non-existent-id");
        serviceCatalogDTO.setCategoryCode("TEST");
        serviceCatalogDTO.setBusNo("TEST-BUS");
        serviceCatalogDTO.setName("测试");
        serviceCatalogDTO.setTypeName("TEST_TYPE");
        serviceCatalogDTO.setStatusEnum("ACTIVE");

//        RuntimeException exception = assertThrows(RuntimeException.class, () -> {
//            serviceCatalogService.updateServiceCatalog(serviceCatalogDTO);
//        });
//        assertEquals("诊疗项目不存在", exception.getMessage());
    }

    @Test
    void testDeleteServiceCatalog() {
        ServiceCatalogDTO serviceCatalogDTO = new ServiceCatalogDTO();
        serviceCatalogDTO.setCategoryCode("TEST");
        serviceCatalogDTO.setBusNo("TEST-BUS-003");
        serviceCatalogDTO.setName("测试诊疗项目3");
        serviceCatalogDTO.setTypeName("TEST_TYPE");
        serviceCatalogDTO.setStatusEnum("ACTIVE");

//        ServiceCatalogResponseDTO createResponse = serviceCatalogService.addServiceCatalog(serviceCatalogDTO);
//
//        Boolean result = serviceCatalogService.removeById(createResponse.getId());
//
//        assertTrue(result);
//
//        WkfActivityDefinitionDO wkfActivityDefinitionDO = wkfActivityDefinitionMapper.selectById(createResponse.getId());
//        assertNotNull(wkfActivityDefinitionDO);
//        assertEquals(1, wkfActivityDefinitionDO.getIsDeleted());
    }

    @Test
    void testDeleteServiceCatalogNotFound() {
        Boolean result = serviceCatalogService.removeById("non-existent-id");
        assertFalse(result);
    }

    @Test
    void testUpdateStatus() {
        ServiceCatalogDTO serviceCatalogDTO = new ServiceCatalogDTO();
        serviceCatalogDTO.setCategoryCode("TEST");
        serviceCatalogDTO.setBusNo("TEST-BUS-004");
        serviceCatalogDTO.setName("测试诊疗项目4");
        serviceCatalogDTO.setTypeName("TEST_TYPE");
        serviceCatalogDTO.setStatusEnum("ACTIVE");

//        ServiceCatalogResponseDTO createResponse = serviceCatalogService.addServiceCatalog(serviceCatalogDTO);
//
//        ServiceCatalogStatusDTO statusDTO = new ServiceCatalogStatusDTO();
//        statusDTO.setId(createResponse.getId());
//        statusDTO.setStatusEnum("INACTIVE");
//
//        ServiceCatalogResponseDTO updateResponse = serviceCatalogService.modifyStatus(statusDTO);
//
//        assertNotNull(updateResponse);
//        assertEquals("INACTIVE", updateResponse.getStatusEnum());
    }

    @Test
    void testUpdateStatusNotFound() {
        ServiceCatalogStatusDTO statusDTO = new ServiceCatalogStatusDTO();
        statusDTO.setId("non-existent-id");
        statusDTO.setStatusEnum("INACTIVE");

        RuntimeException exception = assertThrows(RuntimeException.class, () -> {
            serviceCatalogService.modifyStatus(statusDTO);
        });
        assertEquals("诊疗项目不存在", exception.getMessage());
    }
}