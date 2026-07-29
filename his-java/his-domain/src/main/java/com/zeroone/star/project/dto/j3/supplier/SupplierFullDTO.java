package com.zeroone.star.project.dto.j3.supplier;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：供应商数据传输对象
 * </p>
 * @author 业之
 * @version 1.0.0
 */
@Data
@AllArgsConstructor
@NoArgsConstructor
@Schema(description = "供应商数据库完整映射DTO")
public class SupplierFullDTO implements Serializable {

    @Schema(description = "主键", example = "4100000000000000002")
    private String id;

    @Schema(description = "租户标识", example = "1")
    private String tenantId;

    @Schema(description = "创建时间", example = "2025-01-01 10:00:00")
    private LocalDateTime createdAt;

    @Schema(description = "最后更新时间", example = "2025-01-10 15:30:00")
    private LocalDateTime updatedAt;

    @Schema(description = "创建人", example = "admin")
    private String createdBy;

    @Schema(description = "最后更新人", example = "admin")
    private String updatedBy;

    @Schema(description = "软删标记（0未删除 1已删除）", example = "0")
    private Integer isDeleted;

    @Schema(description = "删除时间", example = "2025-06-01 08:00:00")
    private LocalDateTime deletedAt;

    @Schema(description = "删除人", example = "admin")
    private String deletedBy;

    @Schema(description = "乐观锁版本号", example = "0")
    private Integer version;

    // ==================== 业务字段 ====================

    @Schema(description = "业务编号", example = "SUPP002")
    private String busNo;

    @Schema(description = "名称", example = "华润医药集团有限公司", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "类型枚举值", example = "ENTERPRISE", requiredMode = Schema.RequiredMode.REQUIRED)
    private String typeEnum;

    @Schema(description = "统一社会信用代码", example = "91110000100000XXXX", requiredMode = Schema.RequiredMode.REQUIRED)
    private String creditCode;

    @Schema(description = "联系人", example = "张三")
    private String contactPerson;

    @Schema(description = "联系电话", example = "13800138000")
    private String contactPhone;

    @Schema(description = "地址", example = "上海市浦东新区")
    private String address;

    @Schema(description = "资质信息", example = "医疗器械经营许可证")
    private String qualificationInfo;
}
