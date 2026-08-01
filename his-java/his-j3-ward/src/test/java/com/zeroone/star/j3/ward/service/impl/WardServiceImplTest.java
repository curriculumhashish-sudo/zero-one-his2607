package com.zeroone.star.j3.ward.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.zeroone.star.j3.ward.entity.AdmLocation;
import com.zeroone.star.j3.ward.entity.AdmOrganization;
import com.zeroone.star.j3.ward.exception.WardBusinessException;
import com.zeroone.star.j3.ward.mapper.AdmLocationMapper;
import com.zeroone.star.j3.ward.mapper.AdmOrganizationMapper;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
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
import static org.assertj.core.api.Assertions.assertThatThrownBy;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyLong;
import static org.mockito.ArgumentMatchers.argThat;
import static org.mockito.Mockito.lenient;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

/**
 * WardServiceImpl 单元测试：覆盖 tenantId 的新增回填和查询过滤
 */
@ExtendWith(MockitoExtension.class)
@DisplayName("WardServiceImpl 单元测试")
class WardServiceImplTest {

    @Mock
    private AdmLocationMapper admLocationMapper;

    @Mock
    private AdmOrganizationMapper admOrganizationMapper;

    @Mock
    private UserHolder userHolder;

    @InjectMocks
    private WardServiceImpl wardService;

    private static final Long TEST_TENANT_ID = 100L;

    @BeforeEach
    void setUp() throws Exception {
        UserDTO mockUser = UserDTO.builder().id("1").username("admin").build();
        lenient().when(userHolder.getCurrentUser()).thenReturn(mockUser);
        lenient().when(admOrganizationMapper.getTenantIdByUserId(anyLong()))
                .thenReturn(TEST_TENANT_ID);
    }

    // ==================== addWard ====================

    @Nested
    @DisplayName("addWard — 新增病区")
    class AddWardTests {

        @Test
        @DisplayName("应通过 departmentCode 查询机构并回填 organizationId / tenantId / modeEnum / operationalEnum")
        void shouldBackfillOrganizationIdTenantIdAndBusinessFields() {
            WardAddDTO dto = new WardAddDTO();
            dto.setWardName("内科疗区");
            dto.setDepartmentCode("OR0007.OR0008");

            AdmOrganization mockOrg = new AdmOrganization();
            mockOrg.setId(322663105141149696L);
            mockOrg.setTenantId(TEST_TENANT_ID);
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
            assertThat(entity.getTenantId()).isEqualTo(TEST_TENANT_ID);
            assertThat(entity.getModeEnum()).isEqualTo("INPATIENT");
            assertThat(entity.getOperationalEnum()).isEqualTo("OPEN");
        }

        @Test
        @DisplayName("departmentCode 为空时应抛出异常")
        void shouldThrowWhenDepartmentCodeIsNull() {
            WardAddDTO dto = new WardAddDTO();
            dto.setWardName("独立病区");
            dto.setDepartmentCode(null);

            assertThatThrownBy(() -> wardService.addWard(dto))
                    .isInstanceOf(WardBusinessException.class)
                    .hasMessage("关联科室编号不能为空");
        }

        @Test
        @DisplayName("departmentCode 匹配不到机构时应抛出异常")
        void shouldThrowWhenOrgNotFound() {
            WardAddDTO dto = new WardAddDTO();
            dto.setWardName("未知科室病区");
            dto.setDepartmentCode("NON_EXISTENT");

            when(admOrganizationMapper.selectOne(any(LambdaQueryWrapper.class)))
                    .thenReturn(null);

            assertThatThrownBy(() -> wardService.addWard(dto))
                    .isInstanceOf(WardBusinessException.class)
                    .hasMessage("未找到关联科室：NON_EXISTENT");
        }
    }

    // ==================== addRoom ====================

    @Nested
    @DisplayName("addRoom — 新增病房")
    class AddRoomTests {

        @Test
        @DisplayName("应通过 wardCode 查询父病区并回填 parentId 和 tenantId")
        void shouldBackfillParentIdAndTenantIdFromWardCode() {
            RoomAddDTO dto = new RoomAddDTO();
            dto.setRoomName("302");
            dto.setWardCode("WARD_1234567890");

            AdmLocation parentWard = AdmLocation.builder()
                    .id(100L)
                    .name("内科疗区")
                    .formEnum("WARD")
                    .busNo("WARD_1234567890")
                    .tenantId(TEST_TENANT_ID)
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
            assertThat(entity.getTenantId()).isEqualTo(TEST_TENANT_ID);
        }

        @Test
        @DisplayName("wardCode 为空时应抛出异常")
        void shouldThrowWhenWardCodeIsNull() {
            RoomAddDTO dto = new RoomAddDTO();
            dto.setRoomName("101");
            dto.setWardCode(null);

            assertThatThrownBy(() -> wardService.addRoom(dto))
                    .isInstanceOf(WardBusinessException.class)
                    .hasMessage("所属病区编号不能为空");
        }
    }

    // ==================== addBed ====================

    @Nested
    @DisplayName("addBed — 新增床位")
    class AddBedTests {

        @Test
        @DisplayName("应通过 roomCode 查询父病房并回填 parentId 和 tenantId")
        void shouldBackfillParentIdAndTenantIdFromRoomCode() {
            BedAddDTO dto = new BedAddDTO();
            dto.setBedName("1号床");
            dto.setRoomCode("ROOM_9876543210");

            AdmLocation parentRoom = AdmLocation.builder()
                    .id(200L)
                    .name("302")
                    .formEnum("ROOM")
                    .busNo("ROOM_9876543210")
                    .tenantId(TEST_TENANT_ID)
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
            assertThat(entity.getTenantId()).isEqualTo(TEST_TENANT_ID);
        }

        @Test
        @DisplayName("roomCode 为空时应抛出异常")
        void shouldThrowWhenRoomCodeIsNull() {
            BedAddDTO dto = new BedAddDTO();
            dto.setBedName("2号床");
            dto.setRoomCode(null);

            assertThatThrownBy(() -> wardService.addBed(dto))
                    .isInstanceOf(WardBusinessException.class)
                    .hasMessage("所属病房编号不能为空");
        }
    }

    // ==================== 查询方法 tenantId 过滤 ====================

    @Nested
    @DisplayName("查询方法 — tenantId 过滤")
    class QueryTenantFilterTests {

        @Test
        @DisplayName("getWardTree 应过滤 tenantId")
        void getWardTreeShouldFilterByTenantId() {
            WardTreeQuery query = new WardTreeQuery();
            query.setOrgName(null);

            when(admLocationMapper.selectList(any(LambdaQueryWrapper.class)))
                    .thenReturn(Collections.emptyList());

            wardService.getWardTree(query);

            verify(admLocationMapper).selectList(argThat(wrapper ->
                    wrapper.getSqlSegment().contains("tenant_id")
            ));
        }

        @Test
        @DisplayName("getWardList 应过滤 tenantId")
        void getWardListShouldFilterByTenantId() {
            when(admLocationMapper.selectList(any(LambdaQueryWrapper.class)))
                    .thenReturn(Collections.emptyList());

            wardService.getWardList();

            verify(admLocationMapper).selectList(argThat(wrapper ->
                    wrapper.getSqlSegment().contains("tenant_id")
            ));
        }

        @Test
        @DisplayName("getRoomListByWard 应过滤 tenantId")
        void getRoomListByWardShouldFilterByTenantId() {
            when(admLocationMapper.selectList(any(LambdaQueryWrapper.class)))
                    .thenReturn(Collections.emptyList());

            wardService.getRoomListByWard(100L);

            verify(admLocationMapper).selectList(argThat(wrapper ->
                    wrapper.getSqlSegment().contains("tenant_id")
            ));
        }

        @Test
        @DisplayName("getBedListByRoom 应过滤 tenantId")
        void getBedListByRoomShouldFilterByTenantId() {
            when(admLocationMapper.selectList(any(LambdaQueryWrapper.class)))
                    .thenReturn(Collections.emptyList());

            wardService.getBedListByRoom(200L);

            verify(admLocationMapper).selectList(argThat(wrapper ->
                    wrapper.getSqlSegment().contains("tenant_id")
            ));
        }
    }
}