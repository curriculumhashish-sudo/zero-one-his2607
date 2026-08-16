package com.zeroone.star.dispense.service.impl;

import com.baomidou.mybatisplus.core.MybatisConfiguration;
import com.baomidou.mybatisplus.core.metadata.TableInfoHelper;
import com.zeroone.star.dispense.entity.InventoryItem;
import com.zeroone.star.dispense.entity.MedMedicationDispense;
import com.zeroone.star.dispense.mapper.InventoryItemMapper;
import com.zeroone.star.dispense.mapper.MedMedicationDispenseMapper;
import com.zeroone.star.project.dto.j7.pharmacy.ReturnConfirmDTO;
import com.zeroone.star.project.dto.j7.pharmacy.ReturnRejectDTO;
import com.zeroone.star.project.query.j7.dispense.DispenseRejectQuery;
import org.apache.ibatis.builder.MapperBuilderAssistant;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;

import java.math.BigDecimal;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.*;

/**
 * G13 退药驳回操作单元测试
 * <p>
 * 测试三个功能：
 * 1. 住院发药驳回（驳回汇总单）
 * 2. 住院退药确认（对所选药品执行退药并回补库存）
 * 3. 门诊拒绝退药
 * </p>
 */
@ExtendWith(MockitoExtension.class)
class G13RejectRefundServiceImplTest {

    // ==================== 初始化 MyBatis-Plus Lambda 缓存 ====================

    @BeforeAll
    static void initMybatisPlusLambdaCache() {
        MybatisConfiguration configuration = new MybatisConfiguration();
        MapperBuilderAssistant assistant = new MapperBuilderAssistant(configuration, "");
        TableInfoHelper.initTableInfo(assistant, MedMedicationDispense.class);
        TableInfoHelper.initTableInfo(assistant, InventoryItem.class);
    }

    // ==================== 1. 住院发药驳回 ====================

    @Mock
    private MedMedicationDispenseMapper medicationDispenseMapper;

    @InjectMocks
    private DispensingServiceImpl dispensingService;

    private static final String SUMMARY_NO = "TEST-SUMMARY-G13-001";

    @Test
    @DisplayName("住院发药驳回：汇总单号为空时抛异常")
    void testRejectDispensingWithEmptySummaryNo() {
        DispenseRejectQuery query = new DispenseRejectQuery();
        query.setSummaryNo("");
        query.setRejectReason("驳回原因");

        assertThrows(IllegalArgumentException.class,
                () -> dispensingService.rejectDispensing(query),
                "汇总单号为空应抛出IllegalArgumentException");
        verifyNoInteractions(medicationDispenseMapper);
    }

    @Test
    @DisplayName("住院发药驳回：根据汇总单号驳回所有PENDING记录")
    void testRejectDispensingBySummaryNo() {
        DispenseRejectQuery query = new DispenseRejectQuery();
        query.setSummaryNo(SUMMARY_NO);
        query.setRejectReason("库存不足，驳回发药");

        doReturn(2).when(medicationDispenseMapper).update(any(), any());

        assertDoesNotThrow(() -> dispensingService.rejectDispensing(query));
        verify(medicationDispenseMapper).update(any(), any());
    }

    // ==================== 2. 住院退药确认 ====================

    @Mock
    private InventoryItemMapper inventoryItemMapper;

    @InjectMocks
    private InpatientRefundMedicineServiceImpl inpatientRefundMedicineService;

    private static final String PATIENT_ID = "TEST-PATIENT-G13";
    private static final String REFUND_ID_1 = "REFUND-001";
    private static final String REFUND_ID_2 = "REFUND-002";

    @Test
    @DisplayName("住院退药确认：itemIds为空时直接返回，不操作数据库")
    void testConfirmReturnWithEmptyItemIds() {
        ReturnConfirmDTO dto = new ReturnConfirmDTO();
        dto.setItemIds(List.of());
        dto.setOperator("test-operator");

        assertDoesNotThrow(() -> inpatientRefundMedicineService.confirmReturn(dto));
        verifyNoInteractions(medicationDispenseMapper);
        verifyNoInteractions(inventoryItemMapper);
    }

    @Test
    @DisplayName("住院退药确认：确认退药并回补库存")
    void testConfirmReturnWithInventoryRestore() {
        // Given: mock 退药明细查询
        MedMedicationDispense item1 = new MedMedicationDispense();
        item1.setId(REFUND_ID_1);
        item1.setMedicationId("MED-001");
        item1.setLotNumber("LOT-001");
        item1.setQuantity(new BigDecimal("30"));
        item1.setStatusEnum("PENDING");

        MedMedicationDispense item2 = new MedMedicationDispense();
        item2.setId(REFUND_ID_2);
        item2.setMedicationId("MED-002");
        item2.setLotNumber("LOT-002");
        item2.setQuantity(new BigDecimal("5"));
        item2.setStatusEnum("PENDING");

        doReturn(List.of(item1, item2)).when(medicationDispenseMapper).selectList(any());

        // mock 库存查询（按批号匹配）
        InventoryItem stock1 = new InventoryItem();
        stock1.setId("STOCK-001");
        stock1.setItemTable("med_medication_definition");
        stock1.setItemId("MED-001");
        stock1.setLotNumber("LOT-001");
        stock1.setQuantity(new BigDecimal("100"));

        InventoryItem stock2 = new InventoryItem();
        stock2.setId("STOCK-002");
        stock2.setItemTable("med_medication_definition");
        stock2.setItemId("MED-002");
        stock2.setLotNumber("LOT-002");
        stock2.setQuantity(new BigDecimal("50"));

        doReturn(stock1).doReturn(stock2).when(inventoryItemMapper).selectOne(any());

        ReturnConfirmDTO dto = new ReturnConfirmDTO();
        dto.setReturnId(REFUND_ID_1);
        dto.setPatientId(PATIENT_ID);
        dto.setItemIds(List.of(REFUND_ID_1, REFUND_ID_2));
        dto.setOperator("test-operator");

        // When
        assertDoesNotThrow(() -> inpatientRefundMedicineService.confirmReturn(dto));

        // Then: 验证状态更新了（2条记录）
        verify(medicationDispenseMapper, atLeastOnce()).update(any(), any());

        // 验证库存回补了（2条记录）
        verify(inventoryItemMapper, times(2)).update(any(), any());
    }

    @Test
    @DisplayName("住院退药确认：库存记录不存在时跳过回补")
    void testConfirmReturnWithMissingStock() {
        MedMedicationDispense item = new MedMedicationDispense();
        item.setId(REFUND_ID_1);
        item.setMedicationId("MED-001");
        item.setLotNumber("LOT-001");
        item.setQuantity(new BigDecimal("30"));
        item.setStatusEnum("PENDING");

        doReturn(List.of(item)).when(medicationDispenseMapper).selectList(any());
        doReturn(null).when(inventoryItemMapper).selectOne(any());

        ReturnConfirmDTO dto = new ReturnConfirmDTO();
        dto.setReturnId(REFUND_ID_1);
        dto.setPatientId(PATIENT_ID);
        dto.setItemIds(List.of(REFUND_ID_1));
        dto.setOperator("test-operator");

        assertDoesNotThrow(() -> inpatientRefundMedicineService.confirmReturn(dto));

        verify(medicationDispenseMapper, atLeastOnce()).update(any(), any());
        verify(inventoryItemMapper, never()).update(any(), any());
    }

    // ==================== 3. 门诊拒绝退药 ====================

    @InjectMocks
    private OutpatientRefundServiceImpl outpatientRefundService;

    @Test
    @DisplayName("门诊拒绝退药：使用itemIds批量拒绝")
    void testRejectReturnWithItemIds() {
        ReturnRejectDTO dto = new ReturnRejectDTO();
        dto.setReturnId(REFUND_ID_1);
        dto.setPatientId(PATIENT_ID);
        dto.setRejectReason("门诊拒绝退药-药品不符合退药条件");
        dto.setItemIds(List.of(REFUND_ID_1, REFUND_ID_2));

        doReturn(2).when(medicationDispenseMapper).update(any(), any());

        assertDoesNotThrow(() -> outpatientRefundService.rejectReturn(dto));
        verify(medicationDispenseMapper).update(any(), any());
    }

    @Test
    @DisplayName("门诊拒绝退药：itemIds为空时使用returnId单条拒绝")
    void testRejectReturnWithReturnIdOnly() {
        ReturnRejectDTO dto = new ReturnRejectDTO();
        dto.setReturnId(REFUND_ID_1);
        dto.setPatientId(PATIENT_ID);
        dto.setRejectReason("门诊拒绝退药-药品已开封");

        doReturn(1).when(medicationDispenseMapper).update(any(), any());

        assertDoesNotThrow(() -> outpatientRefundService.rejectReturn(dto));
        verify(medicationDispenseMapper).update(any(), any());
    }

    @Test
    @DisplayName("门诊拒绝退药：影响行数为0时也不抛异常")
    void testRejectReturnNoMatch() {
        ReturnRejectDTO dto = new ReturnRejectDTO();
        dto.setReturnId("NON-EXIST-ID");
        dto.setRejectReason("门诊拒绝");

        doReturn(0).when(medicationDispenseMapper).update(any(), any());

        assertDoesNotThrow(() -> outpatientRefundService.rejectReturn(dto));
        verify(medicationDispenseMapper).update(any(), any());
    }
}