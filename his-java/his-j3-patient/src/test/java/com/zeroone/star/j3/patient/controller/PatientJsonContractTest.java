package com.zeroone.star.j3.patient.controller;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.json.JsonMapper;
import com.fasterxml.jackson.datatype.jsr310.JavaTimeModule;
import com.zeroone.star.project.dto.j3.patient.PatientCreateDTO;
import org.junit.jupiter.api.Test;

import java.time.LocalDate;
import java.time.LocalDateTime;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class PatientJsonContractTest {

    private final ObjectMapper objectMapper = JsonMapper.builder()
            .addModule(new JavaTimeModule())
            .build();

    @Test
    void shouldReadPrototypeFieldNamesAndDateFormats()
            throws Exception {
        String json = """
                {
                  "name": "照夜清",
                  "genderEnum": "MALE",
                  "age": 36,
                  "birthDate": "1990-01-01 00:00:00",
                  "idCard": "110101199001011237",
                  "bloodAbo": 1,
                  "bloodRh": 2,
                  "deceasedDate": "2025-01-01 00:00:00",
                  "organizationId": "322663105141149696"
                }
                """;

        PatientCreateDTO dto = objectMapper.readValue(
                json,
                PatientCreateDTO.class
        );

        assertEquals(LocalDate.of(1990, 1, 1), dto.getBirthDate());
        assertEquals(1, dto.getBloodAbo());
        assertEquals(2, dto.getBloodRh());
        assertEquals(
                LocalDateTime.of(2025, 1, 1, 0, 0),
                dto.getDeceasedDate()
        );

        String serialized = objectMapper.writeValueAsString(dto);
        assertTrue(serialized.contains("\"bloodAbo\":1"));
        assertTrue(serialized.contains("\"bloodRh\":2"));
        assertFalse(serialized.contains("bloodTypeAbo"));
        assertFalse(serialized.contains("bloodTypeRh"));
    }

    @Test
    void shouldAlsoReadDateOnlyBirthDate() throws Exception {
        PatientCreateDTO dto = objectMapper.readValue(
                "{\"birthDate\":\"1990-01-01\"}",
                PatientCreateDTO.class
        );

        assertEquals(LocalDate.of(1990, 1, 1), dto.getBirthDate());
    }
}
