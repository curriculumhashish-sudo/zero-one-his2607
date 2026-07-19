package com.zeroone.star.project.dto.j4.equipment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * 更新耗材目录请求：在创建字段基础上携带网关字符串主键。a
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "更新耗材目录请求：在创建字段基础上携带网关字符串主键")
public class EquipmentEditDTO extends EquipmentAddDTO {

    /** 设备定义主键 */
    @Schema(description = "设备定义主键")
    private String id;

    /** 编辑业务码（由上游业务判定并透传，示例：OK） */
    @Schema(description = "编辑业务码（由上游业务判定并透传，示例：OK）")
    private String editabilityCode;

}