package com.zeroone.star.dispense.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.TableName;
import com.baomidou.mybatisplus.annotation.Version;
import org.junit.jupiter.api.Test;

import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.Arrays;
import java.util.Map;
import java.util.stream.Collectors;

import static org.junit.jupiter.api.Assertions.assertAll;
import static org.junit.jupiter.api.Assertions.assertDoesNotThrow;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;

class StockTakeOrderMappingTest {

    @Test
    void mapsPharmacyStockTakeOrderToSupplyRequestSchema() {
        Class<?> entityType = assertDoesNotThrow(
                () -> Class.forName("com.zeroone.star.dispense.entity.StockTakeOrder")
        );
        Map<String, Class<?>> actualFields = Arrays.stream(entityType.getDeclaredFields())
                .filter(field -> !Modifier.isStatic(field.getModifiers()))
                .collect(Collectors.toMap(Field::getName, Field::getType));

        Map<String, Class<?>> expectedFields = Map.ofEntries(
                Map.entry("id", String.class),
                Map.entry("tenantId", String.class),
                Map.entry("createdAt", LocalDateTime.class),
                Map.entry("updatedAt", LocalDateTime.class),
                Map.entry("createdBy", String.class),
                Map.entry("updatedBy", String.class),
                Map.entry("isDeleted", Boolean.class),
                Map.entry("deletedAt", LocalDateTime.class),
                Map.entry("deletedBy", String.class),
                Map.entry("version", Integer.class),
                Map.entry("busNo", String.class),
                Map.entry("originalBusNo", String.class),
                Map.entry("typeEnum", String.class),
                Map.entry("statusEnum", String.class),
                Map.entry("intentEnum", String.class),
                Map.entry("categoryEnum", String.class),
                Map.entry("priorityEnum", String.class),
                Map.entry("summaryId", String.class),
                Map.entry("deliverId", String.class),
                Map.entry("patientId", String.class),
                Map.entry("itemTable", String.class),
                Map.entry("itemQuantity", BigDecimal.class),
                Map.entry("totalQuantity", BigDecimal.class),
                Map.entry("itemId", String.class),
                Map.entry("unitCode", String.class),
                Map.entry("occurrenceTime", LocalDateTime.class),
                Map.entry("practitionerId", String.class),
                Map.entry("supplierId", String.class),
                Map.entry("reason", String.class),
                Map.entry("sourceTypeEnum", String.class),
                Map.entry("sourceLocationId", String.class),
                Map.entry("purposeTypeEnum", String.class),
                Map.entry("purposeLocationId", String.class),
                Map.entry("approverId", String.class),
                Map.entry("approvalTime", LocalDateTime.class),
                Map.entry("applicantId", String.class),
                Map.entry("applyTime", LocalDateTime.class),
                Map.entry("startTime", LocalDateTime.class),
                Map.entry("endTime", LocalDateTime.class),
                Map.entry("lotNumber", String.class),
                Map.entry("traceNo", String.class),
                Map.entry("price", BigDecimal.class),
                Map.entry("totalPrice", BigDecimal.class),
                Map.entry("remake", String.class),
                Map.entry("profitLossPrice", BigDecimal.class)
        );

        TableName tableName = entityType.getAnnotation(TableName.class);
        Field idField = assertDoesNotThrow(() -> entityType.getDeclaredField("id"));
        TableId tableId = idField.getAnnotation(TableId.class);

        assertAll(
                () -> assertNotNull(tableName),
                () -> assertEquals("wkf_supply_request", tableName.value()),
                () -> assertNotNull(tableId),
                () -> assertEquals(IdType.ASSIGN_ID, tableId.type()),
                () -> assertNotNull(entityType.getDeclaredField("isDeleted").getAnnotation(TableLogic.class)),
                () -> assertNotNull(entityType.getDeclaredField("version").getAnnotation(Version.class)),
                () -> assertEquals(expectedFields, actualFields)
        );
    }
}
