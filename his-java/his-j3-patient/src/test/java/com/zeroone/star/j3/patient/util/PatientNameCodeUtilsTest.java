package com.zeroone.star.j3.patient.util;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;

class PatientNameCodeUtilsTest {

    @Test
    void shouldGenerateSamePinyinInitialsAsPrototype() {
        assertEquals(
                "ZYQ",
                PatientNameCodeUtils.toPinyinInitials("照夜清")
        );
        assertEquals(
                "AL1",
                PatientNameCodeUtils.toPinyinInitials("A李1")
        );
    }

    @Test
    void shouldReturnNullForBlankName() {
        assertNull(PatientNameCodeUtils.toPinyinInitials(null));
        assertNull(PatientNameCodeUtils.toPinyinInitials("   "));
    }
}
