package com.zeroone.star.configuration.service.impl;

import com.zeroone.star.project.j5.emr.template.dto.MedicalTemplateCreateDTO;
import com.zeroone.star.project.j5.emr.template.dto.MedicalTemplateUpdateDTO;
import jakarta.validation.ConstraintViolation;
import jakarta.validation.Validation;
import jakarta.validation.Validator;
import jakarta.validation.ValidatorFactory;
import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import java.util.Set;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

class MedicalTemplateDtoValidationTest {

    private static ValidatorFactory validatorFactory;
    private static Validator validator;

    @BeforeAll
    static void setUpValidator() {
        validatorFactory = Validation.buildDefaultValidatorFactory();
        validator = validatorFactory.getValidator();
    }

    @AfterAll
    static void closeValidator() {
        validatorFactory.close();
    }

    @Test
    void validCreateRequestPassesValidation() {
        MedicalTemplateCreateDTO dto = validCreateDto();
        dto.setScopeLevel(" department ");
        dto.setOrganizationId("dept-1");

        assertTrue(validator.validate(dto).isEmpty());
    }

    @Test
    void departmentCreateRequiresOrganizationAtApiBoundary() {
        MedicalTemplateCreateDTO dto = validCreateDto();
        dto.setScopeLevel("DEPARTMENT");
        dto.setOrganizationId(" ");

        Set<ConstraintViolation<MedicalTemplateCreateDTO>> violations = validator.validate(dto);

        assertTrue(hasMessage(violations, "科室模板必须选择适用科室"));
    }

    @Test
    void createRejectsInvalidScopeAtApiBoundary() {
        MedicalTemplateCreateDTO dto = validCreateDto();
        dto.setScopeLevel("WARD");

        Set<ConstraintViolation<MedicalTemplateCreateDTO>> violations = validator.validate(dto);

        assertTrue(hasMessage(violations, "使用范围必须是 PERSONAL、DEPARTMENT 或 HOSPITAL"));
    }

    @Test
    void createRejectsBlankRequiredFields() {
        MedicalTemplateCreateDTO dto = new MedicalTemplateCreateDTO();
        dto.setName(" ");
        dto.setDocumentTypeCode(null);
        dto.setContent("");
        dto.setEncounterClassCode(" ");

        Set<ConstraintViolation<MedicalTemplateCreateDTO>> violations = validator.validate(dto);

        assertEquals(4, violations.size());
    }

    @Test
    void updateRejectsEmptyRequestAtApiBoundary() {
        Set<ConstraintViolation<MedicalTemplateUpdateDTO>> violations =
                validator.validate(new MedicalTemplateUpdateDTO());

        assertTrue(hasMessage(violations, "至少提供一个需要修改的字段"));
    }

    @Test
    void updateWithFalseBooleanCountsAsProvidedField() {
        MedicalTemplateUpdateDTO dto = new MedicalTemplateUpdateDTO();
        dto.setEnabled(false);

        assertTrue(validator.validate(dto).isEmpty());
    }

    private static boolean hasMessage(Set<? extends ConstraintViolation<?>> violations, String message) {
        return violations.stream().anyMatch(violation -> message.equals(violation.getMessage()));
    }

    private static MedicalTemplateCreateDTO validCreateDto() {
        MedicalTemplateCreateDTO dto = new MedicalTemplateCreateDTO();
        dto.setName("门诊病历");
        dto.setCode("OP_RECORD_NEW");
        dto.setDocumentTypeCode("OUTPATIENT_RECORD");
        dto.setContent("{\"type\":\"doc\"}");
        dto.setEncounterClassCode("AMB");
        return dto;
    }
}
