package com.zeroone.star.basedata.service.impl;

import com.baomidou.mybatisplus.annotation.FieldStrategy;
import com.baomidou.mybatisplus.annotation.TableField;
import com.zeroone.star.basedata.entity.frequency.CusFrequencyDO;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

import java.lang.reflect.Field;
import java.util.stream.Stream;

import static org.junit.jupiter.api.Assertions.assertDoesNotThrow;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;

class FrequencyServiceImplTest {

    @ParameterizedTest(name = "{0} 仅保留当前周期适用字段")
    @MethodSource("cycleFieldMatrix")
    void normalizeCycleFieldsClearsFieldsFromPreviousCycle(
            String cycleType,
            boolean keepDayTimes,
            boolean keepDayInterval,
            boolean keepDayCount,
            boolean keepWeekCycleFlag,
            boolean keepExecutionPeriod,
            boolean keepExecutionPeriodUnit,
            boolean keepTotalExecutionCount,
            boolean keepAllowCrossDay) {
        CusFrequencyDO entity = entityWithAllCycleFields(cycleType);

        FrequencyServiceImpl.normalizeCycleFields(entity);

        assertField("08:00", entity.getDayTimes(), keepDayTimes);
        assertField(2, entity.getDayInterval(), keepDayInterval);
        assertField(3, entity.getDayCount(), keepDayCount);
        assertField((byte) 1, entity.getWeekCycleFlag(), keepWeekCycleFlag);
        assertField(8, entity.getExecutionPeriod(), keepExecutionPeriod);
        assertField("h", entity.getExecutionPeriodUnit(), keepExecutionPeriodUnit);
        assertField(5, entity.getTotalExecutionCount(), keepTotalExecutionCount);
        assertField("cross-day", entity.getAllowCrossDay(), keepAllowCrossDay);
    }

    @Test
    void cycleFieldsAlwaysParticipateInUpdateSoNullValuesReachDatabase() throws NoSuchFieldException {
        for (String fieldName : new String[]{
                "dayTimes", "dayInterval", "dayCount", "weekCycleFlag",
                "executionPeriod", "executionPeriodUnit", "totalExecutionCount", "allowCrossDay"
        }) {
            Field field = CusFrequencyDO.class.getDeclaredField(fieldName);
            TableField tableField = field.getAnnotation(TableField.class);

            assertEquals(FieldStrategy.ALWAYS, tableField.updateStrategy(), fieldName);
        }
    }

    @Test
    void normalizeCycleFieldsIgnoresMissingEntityOrCycleType() {
        CusFrequencyDO entity = entityWithAllCycleFields(null);

        assertDoesNotThrow(() -> FrequencyServiceImpl.normalizeCycleFields(null));
        FrequencyServiceImpl.normalizeCycleFields(entity);

        assertEquals("08:00", entity.getDayTimes());
        assertEquals(8, entity.getExecutionPeriod());
    }

    @Test
    void normalizeCycleFieldsDoesNotChangeCommonFields() {
        CusFrequencyDO entity = entityWithAllCycleFields("fixed-count");
        entity.setRateCode("ST");
        entity.setName("立即执行一次");
        entity.setApplicableScenes("all");
        entity.setActiveFlag((byte) 1);
        entity.setSortNo(10);

        FrequencyServiceImpl.normalizeCycleFields(entity);

        assertEquals("ST", entity.getRateCode());
        assertEquals("立即执行一次", entity.getName());
        assertEquals("all", entity.getApplicableScenes());
        assertEquals((byte) 1, entity.getActiveFlag());
        assertEquals(10, entity.getSortNo());
    }

    private static Stream<Arguments> cycleFieldMatrix() {
        return Stream.of(
                Arguments.of("daily", true, true, true, false, false, false, false, true),
                Arguments.of("weekly", true, false, false, true, false, false, false, true),
                Arguments.of("interval", false, true, false, false, true, true, false, true),
                Arguments.of("as-needed", true, false, false, false, false, false, false, false),
                Arguments.of("fixed-count", true, false, false, false, false, false, true, false),
                Arguments.of("unsupported", false, false, false, false, false, false, false, false)
        );
    }

    private static CusFrequencyDO entityWithAllCycleFields(String cycleType) {
        CusFrequencyDO entity = new CusFrequencyDO();
        entity.setCycleType(cycleType);
        entity.setDayTimes("08:00");
        entity.setDayInterval(2);
        entity.setDayCount(3);
        entity.setWeekCycleFlag((byte) 1);
        entity.setExecutionPeriod(8);
        entity.setExecutionPeriodUnit("h");
        entity.setTotalExecutionCount(5);
        entity.setAllowCrossDay("cross-day");
        return entity;
    }

    private static void assertField(Object expected, Object actual, boolean retained) {
        if (retained) {
            assertEquals(expected, actual);
        } else {
            assertNull(actual);
        }
    }
}
