package com.zeroone.star.project.vo.j7.dispense;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "门诊发药 - 患者队列返回结果")
public class PatientQueueVO {

    @Schema(description = "患者ID")
    private String patientId;

    @Schema(description = "就诊ID")
    private String encounterId;

    @Schema(description = "患者姓名")
    private String name;

    @Schema(description = "年龄")
    private Integer age;

    @Schema(description = "性别编码")
    private String genderEnum;

    @Schema(description = "联系电话")
    private String phone;

    @Schema(description = "发药状态(COMPLETED/PENDING)")
    private String statusEnum;

    @Schema(description = "发放类型(OUTPATIENT/REFUND)")
    private String dispenseEnum;

    @Schema(description = "药品项目数量")
    private Integer drugCount;

    @Schema(description = "最近创建时间")
    private String createdAt;
}
