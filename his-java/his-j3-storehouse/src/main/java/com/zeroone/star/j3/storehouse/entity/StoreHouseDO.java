package com.zeroone.star.j3.storehouse.entity;

import com.baomidou.mybatisplus.annotation.*;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;
import lombok.experimental.Accessors;

import java.time.LocalDateTime;

/**
 * 库房药房实体（映射 adm_location 表）
 * @author mie
 * @version 1.0.0
 */
@Data
@AllArgsConstructor
@NoArgsConstructor
@Accessors(chain = true)
@Builder
@Schema(description = "库房药房实体")
@TableName("adm_location")
public class StoreHouseDO {

    @Schema(description = "主键ID")
    @TableId(type = IdType.ASSIGN_ID)
    private String id;

    @Schema(description = "租户标识")
    private String tenantId;

    @Schema(description = "创建时间")
    private LocalDateTime createdAt;

    @Schema(description = "最后更新时间")
    private LocalDateTime updatedAt;

    @Schema(description = "创建人")
    private String createdBy;

    @Schema(description = "最后更新人")
    private String updatedBy;

    @Schema(description = "软删标记")
    @TableLogic(value = "false", delval = "true")
    private Boolean isDeleted;

    @Schema(description = "删除时间")
    private LocalDateTime deletedAt;

    @Schema(description = "删除人")
    private String deletedBy;

    @Schema(description = "乐观锁版本号")
    @Version
    private Integer version;

    @Schema(description = "名称")
    private String name;

    @Schema(description = "业务编号")
    private String busNo;

    @Schema(description = "状态枚举值(ACTIVE/INACTIVE)")
    private String statusEnum;

    @Schema(description = "运营状态枚举值")
    private String operationalEnum;

    @Schema(description = "形态枚举值(WAREHOUSE/PHARMACY/DEPT_PHARMACY)")
    private String formEnum;

    @Schema(description = "模式枚举值")
    private String modeEnum;

    @Schema(description = "类型JSON快照")
    private String typeJson;

    @Schema(description = "机构标识")
    private String organizationId;

    @Schema(description = "显示顺序")
    private Integer displayOrder;

    @Schema(description = "拼音码")
    private String pyStr;

    @Schema(description = "五笔码")
    private String wbStr;

    @Schema(description = "父节点标识")
    private String parentId;
}
