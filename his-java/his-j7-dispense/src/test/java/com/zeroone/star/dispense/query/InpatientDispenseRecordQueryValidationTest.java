package com.zeroone.star.dispense.query;

import com.zeroone.star.project.query.j7.dispense.InpatientDispenseRecordQuery;
import jakarta.validation.Validation;
import jakarta.validation.Validator;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

class InpatientDispenseRecordQueryValidationTest {

    private final Validator validator = Validation.buildDefaultValidatorFactory().getValidator();

    @Test
    void rejectsReversedTimeRangeAtApiValidationLayer() {
        InpatientDispenseRecordQuery query = new InpatientDispenseRecordQuery();
        query.setStartTime("2026-08-01");
        query.setEndTime("2026-07-31 23:59:59");

        var violations = validator.validate(query);

        assertEquals(1, violations.size());
        assertEquals("开始时间不能晚于结束时间", violations.iterator().next().getMessage());
    }

    @Test
    void rejectsInvalidCalendarDateAtApiValidationLayer() {
        InpatientDispenseRecordQuery query = new InpatientDispenseRecordQuery();
        query.setStartTime("2026-02-30");

        var violations = validator.validate(query);

        assertEquals(1, violations.size());
        assertEquals("开始或结束时间不是有效日期", violations.iterator().next().getMessage());
    }

    @Test
    void rejectsInvalidCalendarDateWithTimeAtApiValidationLayer() {
        InpatientDispenseRecordQuery query = new InpatientDispenseRecordQuery();
        query.setStartTime("2026-02-30 12:00:00");

        var violations = validator.validate(query);

        assertEquals(1, violations.size());
        assertEquals("开始或结束时间不是有效日期", violations.iterator().next().getMessage());
    }
}
