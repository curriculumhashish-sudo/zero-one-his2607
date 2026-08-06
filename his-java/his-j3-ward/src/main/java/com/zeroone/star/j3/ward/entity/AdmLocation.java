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
 * 描述：管理域地点实体（病区/病房/床位）
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
@TableName("adm_location")
@Schema(description = "管理域地点实体", name = "AdmLocation")
public class AdmLocation {

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

    @Schema(description = "名称", example = "内科疗区", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "业务编号", example = "LOC0001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;

    @Schema(description = "状态枚举值", example = "ENABLED", requiredMode = Schema.RequiredMode.REQUIRED)
    private String statusEnum;

    @Schema(description = "运营状态枚举值", example = "OPEN")
    private String operationalEnum;

    @Schema(description = "形态枚举值（WARD/ROOM/BED）", example = "WARD", requiredMode = Schema.RequiredMode.REQUIRED)
    private String formEnum;

    @Schema(description = "模式枚举值", example = "INPATIENT")
    private String modeEnum;

    @Schema(description = "类型JSON快照")
    private String typeJson;

    @Schema(description = "机构标识", example = "322663105141149696", requiredMode = Schema.RequiredMode.REQUIRED)
    private String organizationId;

    @Schema(description = "显示顺序", example = "1")
    private Integer displayOrder;

    @Schema(description = "拼音码", example = "nkld")
    private String pyStr;

    @Schema(description = "五笔码", example = "tntk")
    private String wbStr;

    @Schema(description = "父节点标识", example = "322663105157926912")
    private String parentId;
}
