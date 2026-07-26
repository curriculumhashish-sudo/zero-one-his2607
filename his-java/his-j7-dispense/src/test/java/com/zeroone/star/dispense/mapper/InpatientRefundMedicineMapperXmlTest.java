package com.zeroone.star.dispense.mapper;

import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.nio.charset.StandardCharsets;

import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * 住院退药 Mapper XML 关键字校验
 *
 * @author jacobzp
 * @since 2026-07-26
 */
class InpatientRefundMedicineMapperXmlTest {

    @Test
    void mapperFiltersRefundDispenseAndAggregatesByEncounter() throws IOException {
        String mapperXml = readMapperXml();

        assertTrue(mapperXml.contains("UPPER(d.dispense_enum) IN ('REFUND', 'RETURN')"));
        assertTrue(mapperXml.contains("GROUP BY"));
        assertTrue(mapperXml.contains("UPPER(d.status_enum) = 'PENDING'"));
        assertTrue(mapperXml.contains("UPPER(d.status_enum) = 'REFUNDED'"));
        assertTrue(mapperXml.contains("UPPER(e.class_enum) IN ('INPATIENT', 'IMP')"));
        assertTrue(mapperXml.contains("ELSE 'PENDING'"));
    }

    private String readMapperXml() throws IOException {
        try (var stream = getClass().getClassLoader()
                .getResourceAsStream("mapper/InpatientRefundMedicineMapper.xml")) {
            if (stream == null) {
                throw new IOException("找不到住院退药Mapper XML");
            }
            return new String(stream.readAllBytes(), StandardCharsets.UTF_8);
        }
    }
}
