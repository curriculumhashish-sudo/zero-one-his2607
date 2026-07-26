package com.zeroone.star.j3.department.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
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
 * 描述：科室单元DO对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 胡傲宇
 * @version 1.0.0
 */
@Data
@AllArgsConstructor
@NoArgsConstructor
@Accessors
@Builder
@Schema(description = "科室单元对象实体类", name = "DepartmentUnit")
@TableName(value = "whale_organization_units")
public class DepartmentUnitDO {

    @Schema(
            description = "主键ID",
            example = "322663218269917184",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    @TableId(type = IdType.ASSIGN_ID)
    private Long id;

    @Schema(
            description = "编码",
            example = "OR0007.OR0011",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private String code;

    @Schema(
            description = "显示名称",
            example = "护理站",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private String displayName;

    @Schema(
            description = "父节点标识",
            example = "322663105157926912",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private Long parentId;

    @Schema(
            description = "排序号",
            example = "0",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private Integer sortOrder;

    @Schema(
            description = "是否启用",
            example = "true",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private boolean isEnabled;

    @Schema(
            description = "备注",
            example = "Hello World",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private String remark;

    @Schema(
            description = "租户标识",
            example = "1",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private Long tenantId;

    @Schema(
            description = "创建时间",
            example = "2026-07-20T12:33:49+08:00",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private LocalDateTime createdAt;

    @Schema(
            description = "最后更新时间",
            example = "2026-07-20T12:33:49+08:00",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private LocalDateTime updatedAt;

    @Schema(
            description = "创建人ID",
            example = "1",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private Long createdBy;

    @Schema(
            description = "最后更新人ID",
            example = "1",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private Long updatedBy;

    @Schema(
            description = "软删标记",
            example = "false",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    @TableLogic(value = "false", delval = "true")
    private Boolean isDeleted;

    @Schema(
            description = "删除时间",
            example = "2026-07-20T12:33:49+08:00",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private LocalDateTime deletedAt;

    @Schema(
            description = "删除人ID",
            example = "1000000000000002",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private Long deletedBy;
}
