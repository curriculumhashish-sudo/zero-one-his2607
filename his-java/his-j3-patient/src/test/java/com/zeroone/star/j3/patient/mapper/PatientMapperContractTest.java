package com.zeroone.star.j3.patient.mapper;

import org.apache.ibatis.annotations.Select;
import org.junit.jupiter.api.Test;

import java.util.Arrays;
import java.util.Locale;
import java.util.stream.Collectors;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class PatientMapperContractTest {

    @Test
    void shouldUseExplicitColumnsAndPrototypeOrdering() {
        String selectSql = Arrays.stream(
                        PatientMapper.class.getDeclaredMethods()
                )
                .map(method -> method.getAnnotation(Select.class))
                .filter(annotation -> annotation != null)
                .flatMap(annotation -> Arrays.stream(annotation.value()))
                .collect(Collectors.joining("\n"))
                .toUpperCase(Locale.ROOT);

        assertFalse(selectSql.contains("SELECT *"));
        assertTrue(selectSql.contains("WHERE IS_DELETED = 0"));
        assertTrue(selectSql.contains(
                "ORDER BY CREATED_AT DESC, ID DESC"
        ));
        assertTrue(selectSql.contains("PY_STR LIKE"));
        assertTrue(selectSql.contains("WB_STR LIKE"));
    }
}
