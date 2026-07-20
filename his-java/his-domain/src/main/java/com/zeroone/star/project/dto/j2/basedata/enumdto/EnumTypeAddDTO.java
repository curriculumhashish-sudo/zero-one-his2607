package com.zeroone.star.project.dto.j2.basedata.enumdto;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Pattern;
import lombok.Data;
import lombok.ToString;

/**
 * <p>
 * 描述：枚举类型添加DTO
 * TODO: 由于还没有设计实际的mongodb集合结构，这里的类型名字暂定，EnumTypeDTO也是如此
 * </p>
 * @author 逆光 ni_guang
 * @version 1.0
 */
@Data
@ToString
@Schema(description = "枚举类型添加DTO")
public class EnumTypeAddDTO {

    /**
     * 这里设计的枚举类型集合结构如下：
     * {
     *   "_id": ObjectId("..."),
     *   "tenantId": "tenant_001",           // 多租户隔离，必填
     *   "enumKey": "order_status",          // 业务唯一键，用于代码中引用
     *   "displayName": "订单状态",           // 显示名称
     *   "description": "订单的生命周期状态",   // 描述
     *   "isSystem": false,                 // 是否系统内置（保护不可删）
     *   "status": "active",                // active/archived
     *   "validationRules": {               // 扩展校验规则
     *     "min": 1,
     *     "max": 100,
     *     "regex": "^[A-Z]+$"
     *   },
     *   "createdBy": "1",                  // 创建人，填写的是租户id
     *   "createdAt": ISODate("2026-07-19T00:00:00Z"),
     *   "updatedAt": ISODate("2026-07-19T00:00:00Z"),
     *   “updatedBy": "1"                   // 同理，为租户id
     *   "version": 1                       // 乐观锁版本号
     * }
     *
     */

    @NotBlank(message = "枚举类型键不能为空")
    @Pattern(regexp = "^[a-zA-Z0-9_]+$", message = "枚举类型键必须由字母、数字或下划线组成")
    @Schema(description = "枚举类型键，必须唯一", example = "enumTypeKey", requiredMode = Schema.RequiredMode.REQUIRED)
    private String enumKey;

    @NotBlank(message = "枚举类型名称不能为空")
    @Schema(description = "枚举类型名称", example = "enumTypeName", requiredMode = Schema.RequiredMode.REQUIRED)
    private String displayName;

    @Schema(description = "枚举类型描述", example = "enumTypeDescription", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String description;

    @NotNull(message = "必须指定其是否为系统内置枚举类型，默认是false")
    @Schema(description = "是否系统枚举类型", example = "false", requiredMode = Schema.RequiredMode.REQUIRED)
    private Boolean isSystem;

    // TODO: 该类型计划也由枚举系统管理，本身为系统内置的枚举类型
    @NotNull(message = "枚举类型状态枚举值不能为空")
    @Schema(description = "枚举类型状态(0-激活, 1-未激活)", example = "0", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer status;
}
