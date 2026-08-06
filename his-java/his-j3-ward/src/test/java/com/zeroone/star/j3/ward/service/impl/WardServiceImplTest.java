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

import static org.assertj.core.api.Assertions.assertThat;
import static org.assertj.core.api.Assertions.assertThatThrownBy;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.lenient;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

@ExtendWith(MockitoExtension.class)
@DisplayName("WardServiceImpl")
class WardServiceImplTest {

    private static final String TEST_TENANT_ID = "100";

    @Mock
    private AdmLocationMapper admLocationMapper;

    @Mock
    private AdmOrganizationMapper admOrganizationMapper;

    @Mock
    private UserHolder userHolder;

    @InjectMocks
    private WardServiceImpl wardService;

    @BeforeEach
    void setUp() throws Exception {
        UserDTO mockUser = UserDTO.builder().id("1").username("admin").build();
        lenient().when(userHolder.getCurrentUser()).thenReturn(mockUser);
        lenient().when(userHolder.getCurrentTenantId()).thenReturn(TEST_TENANT_ID);
    }

    @Nested
    class AddWardTests {
        @Test
        void shouldBackfillTenantOrganizationAndAuditFields() {
            WardAddDTO dto = new WardAddDTO();
            dto.setWardName("internal ward");
            dto.setDepartmentCode("OR0007.OR0008");

            AdmOrganization organization = new AdmOrganization();
            organization.setId("322663105141149696");
            organization.setTenantId(TEST_TENANT_ID);
            organization.setBusNo("OR0007.OR0008");

            when(admOrganizationMapper.selectOne(any(LambdaQueryWrapper.class))).thenReturn(organization);

            wardService.addWard(dto);

            ArgumentCaptor<AdmLocation> captor = ArgumentCaptor.forClass(AdmLocation.class);
            verify(admLocationMapper).insert(captor.capture());
            AdmLocation entity = captor.getValue();

            assertThat(entity.getName()).isEqualTo("internal ward");
            assertThat(entity.getFormEnum()).isEqualTo("WARD");
            assertThat(entity.getStatusEnum()).isEqualTo("ENABLED");
            assertThat(entity.getOrganizationId()).isEqualTo("322663105141149696");
            assertThat(entity.getTenantId()).isEqualTo(TEST_TENANT_ID);
            assertThat(entity.getModeEnum()).isEqualTo("INPATIENT");
            assertThat(entity.getOperationalEnum()).isEqualTo("OPEN");
            assertThat(entity.getCreatedAt()).isNotNull();
            assertThat(entity.getCreatedBy()).isEqualTo("admin");
        }

        @Test
        void shouldThrowWhenDepartmentCodeIsBlank() {
            WardAddDTO dto = new WardAddDTO();
            dto.setWardName("ward");

            assertThatThrownBy(() -> wardService.addWard(dto))
                    .isInstanceOf(WardBusinessException.class)
                    .hasMessage("关联科室编号不能为空");
        }

        @Test
        void shouldThrowWhenOrganizationIsMissing() {
            WardAddDTO dto = new WardAddDTO();
            dto.setWardName("ward");
            dto.setDepartmentCode("NON_EXISTENT");

            when(admOrganizationMapper.selectOne(any(LambdaQueryWrapper.class))).thenReturn(null);

            assertThatThrownBy(() -> wardService.addWard(dto))
                    .isInstanceOf(WardBusinessException.class)
                    .hasMessage("未找到关联科室：NON_EXISTENT");
        }
    }

    @Nested
    class AddRoomTests {
        @Test
        void shouldBackfillParentTenantAndAuditFields() {
            RoomAddDTO dto = new RoomAddDTO();
            dto.setRoomName("302");
            dto.setWardCode("WARD_1234567890");

            AdmLocation parentWard = AdmLocation.builder()
                    .id("100")
                    .name("ward")
                    .formEnum("WARD")
                    .busNo("WARD_1234567890")
                    .tenantId(TEST_TENANT_ID)
                    .organizationId("ORG_1")
                    .build();

            when(admLocationMapper.selectOne(any(LambdaQueryWrapper.class))).thenReturn(parentWard);

            wardService.addRoom(dto);

            ArgumentCaptor<AdmLocation> captor = ArgumentCaptor.forClass(AdmLocation.class);
            verify(admLocationMapper).insert(captor.capture());
            AdmLocation entity = captor.getValue();

            assertThat(entity.getName()).isEqualTo("302");
            assertThat(entity.getFormEnum()).isEqualTo("ROOM");
            assertThat(entity.getParentId()).isEqualTo("100");
            assertThat(entity.getTenantId()).isEqualTo(TEST_TENANT_ID);
            assertThat(entity.getOrganizationId()).isEqualTo("ORG_1");
            assertThat(entity.getCreatedAt()).isNotNull();
        }

        @Test
        void shouldThrowWhenWardCodeIsBlank() {
            RoomAddDTO dto = new RoomAddDTO();
            dto.setRoomName("101");

            assertThatThrownBy(() -> wardService.addRoom(dto))
                    .isInstanceOf(WardBusinessException.class)
                    .hasMessage("所属病区编号不能为空");
        }
    }

    @Nested
    class AddBedTests {
        @Test
        void shouldBackfillParentTenantAndAuditFields() {
            BedAddDTO dto = new BedAddDTO();
            dto.setBedName("1号床");
            dto.setRoomCode("ROOM_9876543210");

            AdmLocation parentRoom = AdmLocation.builder()
                    .id("200")
                    .name("302")
                    .formEnum("ROOM")
                    .busNo("ROOM_9876543210")
                    .tenantId(TEST_TENANT_ID)
                    .organizationId("ORG_1")
                    .build();

            when(admLocationMapper.selectOne(any(LambdaQueryWrapper.class))).thenReturn(parentRoom);

            wardService.addBed(dto);

            ArgumentCaptor<AdmLocation> captor = ArgumentCaptor.forClass(AdmLocation.class);
            verify(admLocationMapper).insert(captor.capture());
            AdmLocation entity = captor.getValue();

            assertThat(entity.getName()).isEqualTo("1号床");
            assertThat(entity.getFormEnum()).isEqualTo("BED");
            assertThat(entity.getParentId()).isEqualTo("200");
            assertThat(entity.getTenantId()).isEqualTo(TEST_TENANT_ID);
            assertThat(entity.getOrganizationId()).isEqualTo("ORG_1");
            assertThat(entity.getCreatedAt()).isNotNull();
        }

        @Test
        void shouldThrowWhenRoomCodeIsBlank() {
            BedAddDTO dto = new BedAddDTO();
            dto.setBedName("2号床");

            assertThatThrownBy(() -> wardService.addBed(dto))
                    .isInstanceOf(WardBusinessException.class)
                    .hasMessage("所属病房编号不能为空");
        }
    }

    @Nested
    class QueryTenantFilterTests {
        @Test
        void getWardTreeShouldFilterByTenantId() {
            WardTreeQuery query = new WardTreeQuery();
            query.setPageIndex(1);
            query.setPageSize(10);

            when(admLocationMapper.selectList(any(LambdaQueryWrapper.class))).thenReturn(Collections.emptyList());

            wardService.getWardTree(query);

            verify(userHolder).getCurrentTenantId();
            verify(admLocationMapper).selectList(any(LambdaQueryWrapper.class));
        }

        @Test
        void getWardTreeShouldRejectInvalidPageParams() {
            WardTreeQuery query = new WardTreeQuery();
            query.setPageIndex(0);
            query.setPageSize(10);

            assertThatThrownBy(() -> wardService.getWardTree(query))
                    .isInstanceOf(WardBusinessException.class)
                    .hasMessage("pageIndex和pageSize必须大于0");
        }

        @Test
        void getWardListShouldFilterByTenantId() {
            when(admLocationMapper.selectList(any(LambdaQueryWrapper.class))).thenReturn(Collections.emptyList());

            wardService.getWardList();

            verify(userHolder).getCurrentTenantId();
            verify(admLocationMapper).selectList(any(LambdaQueryWrapper.class));
        }

        @Test
        void getRoomListByWardShouldFilterByTenantId() {
            AdmLocation parentWard = AdmLocation.builder()
                    .id("100")
                    .formEnum("WARD")
                    .tenantId(TEST_TENANT_ID)
                    .build();
            when(admLocationMapper.selectOne(any(LambdaQueryWrapper.class))).thenReturn(parentWard);
            when(admLocationMapper.selectList(any(LambdaQueryWrapper.class))).thenReturn(Collections.emptyList());

            wardService.getRoomListByWard("100");

            verify(userHolder).getCurrentTenantId();
            verify(admLocationMapper).selectList(any(LambdaQueryWrapper.class));
        }

        @Test
        void getBedListByRoomShouldFilterByTenantId() {
            AdmLocation parentRoom = AdmLocation.builder()
                    .id("200")
                    .formEnum("ROOM")
                    .tenantId(TEST_TENANT_ID)
                    .build();
            when(admLocationMapper.selectOne(any(LambdaQueryWrapper.class))).thenReturn(parentRoom);
            when(admLocationMapper.selectList(any(LambdaQueryWrapper.class))).thenReturn(Collections.emptyList());

            wardService.getBedListByRoom("200");

            verify(userHolder).getCurrentTenantId();
            verify(admLocationMapper).selectList(any(LambdaQueryWrapper.class));
        }

        @Test
        void getBedListByRoomShouldRejectNegativeRoomId() {
            assertThatThrownBy(() -> wardService.getBedListByRoom("-1"))
                    .isInstanceOf(WardBusinessException.class)
                    .hasMessage("id不能为负数");
        }
    }

}
