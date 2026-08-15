package com.zeroone.star.j3.ward.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;
import lombok.experimental.Accessors;

import java.time.LocalDateTime;

/**
 * <p>
 * 描述：管理域机构实体（科室/机构）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zhujian
 * @version 1.0.0
 */
@Data
@AllArgsConstructor
@NoArgsConstructor
@Accessors
@Builder
@TableName("adm_organization")
@Schema(description = "管理域机构实体", name = "AdmOrganization")
public class AdmOrganization {

    @TableId(type = IdType.ASSIGN_ID)
    @Schema(description = "主键ID", example = "322663218269917184", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "租户标识", example = "1")
    private String tenantId;

    @Schema(description = "创建时间", example = "2026-07-20T12:33:49+08:00", requiredMode = Schema.RequiredMode.REQUIRED)
    private LocalDateTime createdAt;

    @Schema(description = "最后更新时间", example = "2026-07-20T12:33:49+08:00")
    private LocalDateTime updatedAt;

    @Schema(description = "创建人", example = "admin")
    private String createdBy;

    @Schema(description = "最后更新人", example = "admin")
    private String updatedBy;

    @Schema(description = "软删标记", example = "false", requiredMode = Schema.RequiredMode.REQUIRED)
    private Boolean isDeleted;

    @Schema(description = "删除时间")
    private LocalDateTime deletedAt;

    @Schema(description = "删除人")
    private String deletedBy;

    @Schema(description = "乐观锁版本号", example = "0", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer version;

    @Schema(description = "机构/科室名称", example = "住院内科", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "业务编号", example = "OR0007.OR0008", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;

    @Schema(description = "父节点标识", example = "322663105157926912")
    private String parentId;

    @Schema(description = "类型枚举值", example = "DEPARTMENT", requiredMode = Schema.RequiredMode.REQUIRED)
    private String typeEnum;

    @Schema(description = "分类枚举值")
    private String classEnum;

    @Schema(description = "状态枚举值", example = "ENABLED", requiredMode = Schema.RequiredMode.REQUIRED)
    private String statusEnum;

    @Schema(description = "活跃标志位", example = "true", requiredMode = Schema.RequiredMode.REQUIRED)
    private Boolean activeFlag;

    @Schema(description = "显示顺序", example = "1")
    private Integer displayOrder;

    @Schema(description = "默认医生标识")
    private String defDoctorId;

    @Schema(description = "拼音码")
    private String pyStr;

    @Schema(description = "五笔码")
    private String wbStr;
}
