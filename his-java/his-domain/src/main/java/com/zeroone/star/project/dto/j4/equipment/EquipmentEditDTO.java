package com.zeroone.star.project.dto.j4.equipment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * 更新器材目录请求：在创建字段基础上携带网关字符串主键。
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "更新器材目录请求：在创建字段基础上携带网关字符串主键")
public class EquipmentEditDTO extends EquipmentAddDTO {

    /** 器定义主键 */
    @Schema(description = "器定义主键", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    /** 编辑业务码（由上游业务判定并透传） */
    @Schema(description = "编辑业务码（由上游业务判定并透传）", example = "OK")
    private String editabilityCode;

}