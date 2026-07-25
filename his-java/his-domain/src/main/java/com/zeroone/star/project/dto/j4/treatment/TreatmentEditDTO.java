package com.zeroone.star.project.dto.j4.treatment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * 更新诊疗项目请求：在创建字段基础上携带网关字符串主键和编辑校验码。
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "更新诊疗项目请求：在创建字段基础上携带网关字符串主键和编辑校验码")
public class TreatmentEditDTO extends TreatmentAddDTO {

    @Schema(description = "诊疗定义主键", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "编辑业务校验码（由上游业务判定并透传，用于并发编辑保护）", example = "OK", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String editabilityCode;
}