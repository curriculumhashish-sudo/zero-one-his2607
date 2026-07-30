package com.zeroone.star.j3.patient.controller;

import com.zeroone.star.project.dto.j3.patient.PatientCreateDTO;
import com.zeroone.star.project.dto.j3.patient.PatientUpdateDTO;
import jakarta.validation.ConstraintViolation;
import jakarta.validation.Validation;
import jakarta.validation.Validator;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import java.time.LocalDate;
import java.util.Set;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class PatientDtoValidationTest {

    private static Validator validator;

    @BeforeAll
    static void setUpValidator() {
        validator = Validation.buildDefaultValidatorFactory()
                .getValidator();
    }

    @Test
    void shouldAcceptMinimalCreateContract() {
        PatientCreateDTO dto = validCreateDto();

        assertTrue(validator.validate(dto).isEmpty());
    }

    @Test
    void shouldRejectBlankNameInvalidGenderAndFutureBirthDate() {
        PatientCreateDTO dto = validCreateDto();
        dto.setName("   ");
        dto.setGenderEnum("INVALID");
        dto.setBirthDate(LocalDate.now().plusDays(1));

        Set<ConstraintViolation<PatientCreateDTO>> violations =
                validator.validate(dto);

        assertFalse(violations.isEmpty());
        assertTrue(violations.stream().anyMatch(
                violation -> violation.getPropertyPath()
                        .toString()
                        .equals("name")
        ));
        assertTrue(violations.stream().anyMatch(
                violation -> violation.getPropertyPath()
                        .toString()
                        .equals("genderEnum")
        ));
        assertTrue(violations.stream().anyMatch(
                violation -> violation.getPropertyPath()
                        .toString()
                        .equals("birthDate")
        ));
    }

    @Test
    void shouldRejectBlankOrganization() {
        PatientCreateDTO dto = validCreateDto();
        dto.setOrganizationId(" ");

        assertFalse(validator.validate(dto).isEmpty());
    }

    @Test
    void shouldRejectMissingPrototypeRequiredFieldsAndInvalidAge() {
        PatientCreateDTO dto = validCreateDto();
        dto.setIdCard(" ");
        dto.setAge(151);
        dto.setBirthDate(null);

        Set<ConstraintViolation<PatientCreateDTO>> violations =
                validator.validate(dto);

        assertTrue(violations.stream().anyMatch(
                violation -> violation.getPropertyPath()
                        .toString()
                        .equals("idCard")
        ));
        assertTrue(violations.stream().anyMatch(
                violation -> violation.getPropertyPath()
                        .toString()
                        .equals("age")
        ));
        assertTrue(violations.stream().anyMatch(
                violation -> violation.getPropertyPath()
                        .toString()
                        .equals("birthDate")
        ));
    }

    @Test
    void shouldAllowMissingVersionAndRejectNegativeVersion() {
        PatientUpdateDTO dto = validUpdateDto();
        dto.setVersion(null);
        assertTrue(validator.validate(dto).isEmpty());

        dto.setVersion(-1);
        assertFalse(validator.validate(dto).isEmpty());

        dto.setVersion(0);
        assertTrue(validator.validate(dto).isEmpty());
    }

    private PatientCreateDTO validCreateDto() {
        PatientCreateDTO dto = new PatientCreateDTO();
        dto.setName("原型验收甲");
        dto.setGenderEnum("MALE");
        dto.setIdCard("110101199001011237");
        dto.setAge(36);
        dto.setBirthDate(LocalDate.of(1990, 1, 1));
        dto.setOrganizationId("322663105141149696");
        return dto;
    }

    private PatientUpdateDTO validUpdateDto() {
        PatientUpdateDTO dto = new PatientUpdateDTO();
        dto.setName("原型验收甲");
        dto.setGenderEnum("MALE");
        dto.setIdCard("110101199001011237");
        dto.setAge(36);
        dto.setBirthDate(LocalDate.of(1990, 1, 1));
        dto.setOrganizationId("322663105141149696");
        return dto;
    }
}
