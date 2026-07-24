package com.zeroone.star.project.dto.j2.basedata.enumdto;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Pattern;
import jakarta.validation.constraints.Size;
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
@Schema(description = "枚举类型添加DTO")
public class EnumTypeAddDTO {

    @Size(min = 1, max = 100, message = "枚举类型键长度必须在1到100个字符之间")
    @Pattern(regexp = "^[a-zA-Z0-9_]+$", message = "枚举类型键必须由字母、数字或下划线组成")
    @Schema(description = "枚举类型键，必须唯一", example = "enumTypeKey", requiredMode = Schema.RequiredMode.REQUIRED)
    private String enumKey;

    @Size(min = 1, max = 500, message = "枚举类型名称长度必须在1到500个字符之间")
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
     *   “isDeleted”: false                 // 软删除标志，逻辑删除用
     * }
     *
     * // 相较于字典系统，枚举字典更倾向于后端，个人认为其不用添加描述来解释
     * {
     *   "_id": ObjectId("..."),
     *   "enumTypeId": ObjectId("..."),    // 关联 enum_type._id
     *   "tenantId": "tenant_001",           // 冗余字段，便于按租户分片
     *   "enumKey": "SHIPPED",                 // 存储字符
     *   "enumCode": 1                             // 枚举对应的实数值，一个枚举类型的枚举值的code，string互不相同
     *   "display": "已发货",                  // 显示文本（支持多语言）
     *   "order": 10,                        // 排序权重，数值越小越靠前
     *   "status": "active",                // active/inactive/deleted
     *
     *   "createdBy": "user_001",
     *   "createdAt": ISODate("2026-07-19T00:00:00Z"),
     *   "updatedAt": ISODate("2026-07-19T00:00:00Z"),
     *   "updatedBy": "user_001"
     *   "version": 1                        // 乐观锁版本号
     *   “isDeleted”: false                 // 软删除标志，逻辑删除用
     * }
     */
}
