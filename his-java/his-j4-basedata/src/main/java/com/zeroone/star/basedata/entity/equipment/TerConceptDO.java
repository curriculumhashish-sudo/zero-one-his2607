package com.zeroone.star.basedata.entity.equipment;

import com.baomidou.mybatisplus.annotation.*;

import java.time.LocalDateTime;
import java.io.Serializable;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.experimental.Accessors;

/**
 * <p>
 * 编码概念
 * </p>
 *
 * @author xiaohan
 * @since 2026-07-27
 */
@Data
@EqualsAndHashCode(callSuper = false)
@Accessors(chain = true)
@TableName("ter_concept")
@Schema(description="编码概念")
public class TerConceptDO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "主键")
    @TableId(type = IdType.ASSIGN_ID)
    private String id;

    @Schema(description = "编码概念")
    private String codeSystemId;

    @Schema(description = "编码")
    private String code;

    @Schema(description = "显示名称")
    private String display;

    @Schema(description = "定义")
    private String definition;

    @Schema(description = "parent标识")
    private String parentId;

    @Schema(description = "父概念（支持层级结构，可选） / private Concept parent")
    private Long sortOrder;

    @Schema(description = "是否启用")
    private Boolean isActive;

    @Schema(description = "CSS样式类")
    private String cssClass;

    @Schema(description = "列表样式类")
    private String listClass;

    @Schema(description = "是否默认")
    private String isDefault;

    @Schema(description = "租户标识")
    private String tenantId;

    @Schema(description = "创建时间")
    @TableField(value = "created_at", fill = FieldFill.INSERT)
    private LocalDateTime createdAt;

    @Schema(description = "最后更新时间")
    @TableField(value = "updated_at", fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

    @Schema(description = "创建人")
    @TableField(value = "created_by", fill = FieldFill.INSERT)
    private String createdBy;

    @Schema(description = "最后更新人")
    @TableField("updated_by")
    private String updatedBy;

    @Schema(description = "软删标记")
    @TableLogic(value = "0", delval = "1")
    private Boolean isDeleted;

    @Schema(description = "删除时间")
    private LocalDateTime deletedAt;

    @Schema(description = "删除人")
    private String deletedBy;


}
