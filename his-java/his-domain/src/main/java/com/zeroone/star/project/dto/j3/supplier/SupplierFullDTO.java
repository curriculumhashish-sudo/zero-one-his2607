package com.zeroone.star.project.dto.j3.supplier;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：供应商数据库完整实体映射DTO（字段与 cus_supplier 表完全对齐）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zzcsz
 * @version 1.0.0
 */
@Data
@AllArgsConstructor
@NoArgsConstructor
@Schema(description = "供应商数据库完整映射DTO")
public class SupplierFullDTO implements Serializable {

    // ==================== 基础字段 ====================

    @Schema(description = "主键", example = "1234567890123456789", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "租户标识", example = "tenant001")
    private String tenantId;

    @Schema(description = "创建时间", example = "2025-01-01 10:00:00", requiredMode = Schema.RequiredMode.REQUIRED)
    private LocalDateTime createdAt;

    @Schema(description = "最后更新时间", example = "2025-01-10 15:30:00")
    private LocalDateTime updatedAt;

    @Schema(description = "创建人", example = "admin")
    private String createdBy;

    @Schema(description = "最后更新人", example = "admin")
    private String updatedBy;

    @Schema(description = "软删标记（0未删除 1已删除）", example = "0", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer isDeleted;

    @Schema(description = "删除时间", example = "2025-06-01 08:00:00")
    private LocalDateTime deletedAt;

    @Schema(description = "删除人", example = "admin")
    private String deletedBy;

    @Schema(description = "乐观锁版本号", example = "0", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer version;

    // ==================== 业务字段 ====================

    @Schema(description = "业务编号", example = "SUP20250001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;

    @Schema(description = "名称", example = "华润医药集团有限公司", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "类型枚举值", example = "ENTERPRISE", requiredMode = Schema.RequiredMode.REQUIRED)
    private String typeEnum;

    @Schema(description = "统一社会信用代码", example = "91110000100000XXXX", requiredMode = Schema.RequiredMode.REQUIRED)
    private String creditCode;

    @Schema(description = "法定代表人", example = "张三")
    private String legalRepresentative;

    @Schema(description = "地址", example = "北京市朝阳区XX路XX号")
    private String address;

    @Schema(description = "拼音码", example = "HRYY", requiredMode = Schema.RequiredMode.REQUIRED)
    private String pyStr;

    @Schema(description = "五笔码", example = "WXDG", requiredMode = Schema.RequiredMode.REQUIRED)
    private String wbStr;

    @Schema(description = "联系电话", example = "010-88888888")
    private String phone;

    @Schema(description = "邮箱", example = "contact@huarun.com")
    private String email;

    @Schema(description = "开户行", example = "中国工商银行北京分行")
    private String bankName;

    @Schema(description = "银行账户", example = "6222000000000000000")
    private String bankAccount;

    @Schema(description = "机构标识", example = "ORG001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String orgId;

    @Schema(description = "显示顺序", example = "1")
    private Integer displayOrder;

    @Schema(description = "状态枚举值（ENABLED-启用/DISABLED-停用）", example = "ENABLED", requiredMode = Schema.RequiredMode.REQUIRED)
    private String statusEnum;

    @Schema(description = "备注说明", example = "优质供应商，年合作额超百万")
    private String note;
}
