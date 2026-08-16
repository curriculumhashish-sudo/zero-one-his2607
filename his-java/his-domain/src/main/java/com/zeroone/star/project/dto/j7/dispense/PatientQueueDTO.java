package com.zeroone.star.project.dto.j7.dispense;

import com.zeroone.star.project.dto.PageDTO;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * 获取患者队列 前端请求入参DTO
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "患者队列查询请求参数")
public class PatientQueueDTO extends PageDTO {

    @Schema(description = "租户ID", example = "1")
    private String tenantId;

    @Schema(description = "患者业务编号", example = "PA0005")
    private String busNo;

    @Schema(description = "患者姓名", example = "周女士")
    private String name;

    @Schema(description = "性别编码(gender_enum)：MALE-男 FEMALE-女", example = "FEMALE")
    private String genderEnum;

    @Schema(description = "身份证号", example = "220322200002231548")
    private String idCard;

    @Schema(description = "联系电话", example = "13854489876")
    private String phone;

    @Schema(description = "患者状态编码(status_enum)：ACTIVE-有效", example = "ACTIVE")
    private String statusEnum;

    @Schema(description = "有效标识(0-无效 1-有效)", example = "1")
    private Integer activeFlag;

    @Schema(description = "所属机构ID", example = "322663308812357632")
    private String organizationId;
}