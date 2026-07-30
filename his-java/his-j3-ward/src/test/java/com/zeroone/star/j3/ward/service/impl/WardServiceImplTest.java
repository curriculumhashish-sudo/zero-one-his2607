package com.zeroone.star.j3.ward.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.zeroone.star.j3.ward.entity.AdmLocation;
import com.zeroone.star.j3.ward.entity.AdmOrganization;
import com.zeroone.star.j3.ward.mapper.AdmLocationMapper;
import com.zeroone.star.j3.ward.mapper.AdmOrganizationMapper;
import com.zeroone.star.project.dto.j3.ward.BedAddDTO;
import com.zeroone.star.project.dto.j3.ward.RoomAddDTO;
import com.zeroone.star.project.dto.j3.ward.WardAddDTO;
import com.zeroone.star.project.query.j3.ward.WardTreeQuery;
import com.zeroone.star.project.vo.j3.ward.WardTreeVO;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Nested;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.ArgumentCaptor;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;

import java.util.Collections;
import java.util.List;

import static org.assertj.core.api.Assertions.assertThat;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.argThat;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

/**
 * WardServiceImpl 单元测试：
 * 覆盖 addWard / addRoom / addBed / getWardTree 四个关键修复点
 */
@ExtendWith(MockitoExtension.class)
@DisplayName("WardServiceImpl 单元测试")
class WardServiceImplTest {

    @Mock
    private AdmLocationMapper admLocationMapper;

    @Mock
    private AdmOrganizationMapper admOrganizationMapper;

    @InjectMocks
    private WardServiceImpl wardService;

    // ==================== addWard ====================

    @Nested
    @DisplayName("addWard — 新增病区")
    class AddWardTests {

        @Test
        @DisplayName("应通过 departmentCode 查询机构并回填 organizationId / modeEnum / operationalEnum")
        void shouldBackfillOrganizationIdAndBusinessFields() {
            WardAddDTO dto = new WardAddDTO();
            dto.setWardName("内科疗区");
            dto.setDepartmentCode("OR0007.OR0008");

            AdmOrganization mockOrg = new AdmOrganization();
            mockOrg.setId(322663105141149696L);
            mockOrg.setBusNo("OR0007.OR0008");

            when(admOrganizationMapper.selectOne(any(LambdaQueryWrapper.class)))
                    .thenReturn(mockOrg);

            wardService.addWard(dto);

            ArgumentCaptor<AdmLocation> captor = ArgumentCaptor.forClass(AdmLocation.class);
            verify(admLocationMapper).insert(captor.capture());
            AdmLocation entity = captor.getValue();

            assertThat(entity.getName()).isEqualTo("内科疗区");
            assertThat(entity.getFormEnum()).isEqualTo("WARD");
            assertThat(entity.getStatusEnum()).isEqualTo("ENABLED");
            assertThat(entity.getOrganizationId()).isEqualTo(322663105141149696L);
            assertThat(entity.getModeEnum()).isEqualTo("INPATIENT");
            assertThat(entity.getOperationalEnum()).isEqualTo("OPEN");
        }

        @Test
        @DisplayName("departmentCode 为空时 organizationId 应为 null，其余字段正常填充")
        void shouldSetNullOrganizationIdWhenDepartmentCodeIsEmpty() {
            WardAddDTO dto = new WardAddDTO();
            dto.setWardName("独立病区");
            dto.setDepartmentCode(null);

            wardService.addWard(dto);

            ArgumentCaptor<AdmLocation> captor = ArgumentCaptor.forClass(AdmLocation.class);
            verify(admLocationMapper).insert(captor.capture());
            AdmLocation entity = captor.getValue();

            assertThat(entity.getOrganizationId()).isNull();
            assertThat(entity.getModeEnum()).isEqualTo("INPATIENT");
            assertThat(entity.getOperationalEnum()).isEqualTo("OPEN");
        }

        @Test
        @DisplayName("departmentCode 匹配不到机构时 organizationId 应为 null")
        void shouldSetNullOrganizationIdWhenOrgNotFound() {
            WardAddDTO dto = new WardAddDTO();
            dto.setWardName("未知科室病区");
            dto.setDepartmentCode("NON_EXISTENT");

            when(admOrganizationMapper.selectOne(any(LambdaQueryWrapper.class)))
                    .thenReturn(null);

            wardService.addWard(dto);

            ArgumentCaptor<AdmLocation> captor = ArgumentCaptor.forClass(AdmLocation.class);
            verify(admLocationMapper).insert(captor.capture());
            AdmLocation entity = captor.getValue();

            assertThat(entity.getOrganizationId()).isNull();
        }
    }

    // ==================== addRoom ====================

    @Nested
    @DisplayName("addRoom — 新增病房")
    class AddRoomTests {

        @Test
        @DisplayName("应通过 wardCode 查询父病区并回填 parentId")
        void shouldBackfillParentIdFromWardCode() {
            RoomAddDTO dto = new RoomAddDTO();
            dto.setRoomName("302");
            dto.setWardCode("WARD_1234567890");

            AdmLocation parentWard = AdmLocation.builder()
                    .id(100L)
                    .name("内科疗区")
                    .formEnum("WARD")
                    .busNo("WARD_1234567890")
                    .build();

            when(admLocationMapper.selectOne(any(LambdaQueryWrapper.class)))
                    .thenReturn(parentWard);

            wardService.addRoom(dto);

            ArgumentCaptor<AdmLocation> captor = ArgumentCaptor.forClass(AdmLocation.class);
            verify(admLocationMapper).insert(captor.capture());
            AdmLocation entity = captor.getValue();

            assertThat(entity.getName()).isEqualTo("302");
            assertThat(entity.getFormEnum()).isEqualTo("ROOM");
            assertThat(entity.getParentId()).isEqualTo(100L);
        }

        @Test
        @DisplayName("wardCode 为空时 parentId 应为 null")
        void shouldSetNullParentIdWhenWardCodeIsEmpty() {
            RoomAddDTO dto = new RoomAddDTO();
            dto.setRoomName("101");
            dto.setWardCode(null);

            wardService.addRoom(dto);

            ArgumentCaptor<AdmLocation> captor = ArgumentCaptor.forClass(AdmLocation.class);
            verify(admLocationMapper).insert(captor.capture());
            AdmLocation entity = captor.getValue();

            assertThat(entity.getParentId()).isNull();
        }
    }

    // ==================== addBed ====================

    @Nested
    @DisplayName("addBed — 新增床位")
    class AddBedTests {

        @Test
        @DisplayName("应通过 roomCode 查询父病房并回填 parentId")
        void shouldBackfillParentIdFromRoomCode() {
            BedAddDTO dto = new BedAddDTO();
            dto.setBedName("1号床");
            dto.setRoomCode("ROOM_9876543210");

            AdmLocation parentRoom = AdmLocation.builder()
                    .id(200L)
                    .name("302")
                    .formEnum("ROOM")
                    .busNo("ROOM_9876543210")
                    .build();

            when(admLocationMapper.selectOne(any(LambdaQueryWrapper.class)))
                    .thenReturn(parentRoom);

            wardService.addBed(dto);

            ArgumentCaptor<AdmLocation> captor = ArgumentCaptor.forClass(AdmLocation.class);
            verify(admLocationMapper).insert(captor.capture());
            AdmLocation entity = captor.getValue();

            assertThat(entity.getName()).isEqualTo("1号床");
            assertThat(entity.getFormEnum()).isEqualTo("BED");
            assertThat(entity.getParentId()).isEqualTo(200L);
        }

        @Test
        @DisplayName("roomCode 为空时 parentId 应为 null")
        void shouldSetNullParentIdWhenRoomCodeIsEmpty() {
            BedAddDTO dto = new BedAddDTO();
            dto.setBedName("2号床");
            dto.setRoomCode(null);

            wardService.addBed(dto);

            ArgumentCaptor<AdmLocation> captor = ArgumentCaptor.forClass(AdmLocation.class);
            verify(admLocationMapper).insert(captor.capture());
            AdmLocation entity = captor.getValue();

            assertThat(entity.getParentId()).isNull();
        }
    }

    // ==================== getWardTree ====================

    @Nested
    @DisplayName("getWardTree — 树形结构查询")
    class GetWardTreeTests {

        @Test
        @DisplayName("orgName 不为空时应通过 adm_organization 表过滤，而非直接匹配 location.name")
        void shouldFilterByOrganizationNameNotLocationName() {
            WardTreeQuery query = new WardTreeQuery();
            query.setOrgName("住院内科");

            AdmOrganization org = new AdmOrganization();
            org.setId(322663105141149696L);
            org.setName("住院内科");

            when(admOrganizationMapper.selectList(any(LambdaQueryWrapper.class)))
                    .thenReturn(List.of(org));
            when(admLocationMapper.selectList(any(LambdaQueryWrapper.class)))
                    .thenReturn(Collections.emptyList());

            List<WardTreeVO> result = wardService.getWardTree(query);

            assertThat(result).isEmpty();
            verify(admOrganizationMapper).selectList(argThat(wrapper -> {
                String sql = wrapper.getTargetSql();
                return sql.contains("name") && sql.contains("住院内科");
            }));
            verify(admLocationMapper).selectList(argThat(wrapper -> {
                String sql = wrapper.getTargetSql();
                return sql.contains("organization_id") && sql.contains("322663105141149696");
            }));
        }

        @Test
        @DisplayName("orgName 匹配不到机构时应直接返回空列表，不查询 location")
        void shouldReturnEmptyWhenNoOrgMatches() {
            WardTreeQuery query = new WardTreeQuery();
            query.setOrgName("不存在的机构");

            when(admOrganizationMapper.selectList(any(LambdaQueryWrapper.class)))
                    .thenReturn(Collections.emptyList());

            List<WardTreeVO> result = wardService.getWardTree(query);

            assertThat(result).isEmpty();
            verify(admOrganizationMapper).selectList(any(LambdaQueryWrapper.class));
            verify(admLocationMapper, never()).selectList(any(LambdaQueryWrapper.class));
        }

        @Test
        @DisplayName("orgName 为空时不应查询机构表，直接查询所有 location")
        void shouldNotQueryOrgWhenOrgNameIsNull() {
            WardTreeQuery query = new WardTreeQuery();
            query.setOrgName(null);

            when(admLocationMapper.selectList(any(LambdaQueryWrapper.class)))
                    .thenReturn(Collections.emptyList());

            List<WardTreeVO> result = wardService.getWardTree(query);

            assertThat(result).isEmpty();
            verify(admOrganizationMapper, never()).selectList(any(LambdaQueryWrapper.class));
        }
    }
}