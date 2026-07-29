package com.zeroone.star.dispense.mapper;

import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.nio.charset.StandardCharsets;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class InpatientDispenseRecordMapperXmlTest {

    @Test
    void mapperTranslatesPersistedStatusesAndFallsBackToBusinessNumber() throws IOException {
        String mapperXml = readMapperXml();

        assertTrue(mapperXml.contains("WHEN 'REFUNDED' THEN '已退药'"));
        assertTrue(mapperXml.contains("WHEN 'DECLINED' THEN '已拒绝'"));
        assertTrue(mapperXml.contains(
                "COALESCE(NULLIF(r.prescription_no, ''), r.bus_no, d.bus_no) AS prescription_no"
        ));
    }

    @Test
    void mapperUsesServiceNormalizedEndTimeDirectly() throws IOException {
        String mapperXml = readMapperXml();

        assertTrue(mapperXml.contains("AND d.dispense_time &lt;= #{query.endTime}"));
        assertFalse(mapperXml.contains("LENGTH(#{query.endTime})"));
    }

    private String readMapperXml() throws IOException {
        try (var stream = getClass().getClassLoader()
                .getResourceAsStream("mapper/InpatientDispenseRecordMapper.xml")) {
            if (stream == null) {
                throw new IOException("找不到住院发退药Mapper XML");
            }
            return new String(stream.readAllBytes(), StandardCharsets.UTF_8);
        }
    }
}
