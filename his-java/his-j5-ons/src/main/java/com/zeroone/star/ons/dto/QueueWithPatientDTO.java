package com.zeroone.star.ons.dto;

import com.zeroone.star.ons.entity.WkfAppointmentQueue;
import lombok.Data;
import lombok.EqualsAndHashCode;

import java.time.LocalDate;

/**
 * 预约队列 + 患者基础信息 JOIN 查询结果。
 * 用于患者队列列表一次性返回姓名/性别/出生日期（年龄由Service层计算）。
 */
@Data
@EqualsAndHashCode(callSuper = true)
public class QueueWithPatientDTO extends WkfAppointmentQueue {

    /** adm_patient.name */
    private String patientName;

    /** adm_patient.gender_enum */
    private String patientGenderEnum;

    /** adm_patient.birth_date，用于计算年龄 */
    private LocalDate patientBirthDate;

    /** adm_location.name via destination_location_id（去向诊室名称） */
    private String destinationLocationName;
}
