package com.zeroone.star.project.dto.j4.treatment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * 更新诊疗项目请求：在创建字段基础上携带网关字符串主键和编辑校验码。
 *
 * 【前端对接说明】
 * 1. id必填，为要修改的诊疗项目主键ID
 * 2. 其他字段同TreatmentAddDTO，只需传需要修改的字段
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "更新诊疗项目请求：在创建字段基础上携带网关字符串主键和编辑校验码")
public class TreatmentEditDTO extends TreatmentAddDTO {

    @Schema(description = "诊疗定义主键", example = "323001764504670208", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "编辑业务码（由上游业务判定并透传）", example = "OK")
    private String editabilityCode;
}