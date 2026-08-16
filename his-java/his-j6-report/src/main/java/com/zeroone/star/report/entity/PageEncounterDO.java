package com.zeroone.star.report.entity;

import lombok.Data;

import java.time.LocalDateTime;

/**
 *  门诊基础信息
 */
@Data
public class PageEncounterDO {
    // === 就诊块 ===
    private String encounterId;
    private String busNo;
    private String patientName;
    private String registrationDepartmentName;
    private LocalDateTime encounterDate;
}
