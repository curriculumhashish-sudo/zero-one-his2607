package com.zeroone.star.j3.department.entity;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;
import lombok.experimental.Accessors;

import java.time.LocalDateTime;

/**
 * <p>
 * 描述：科室对象实体类
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
@Schema(description = "科室对象实体类", name = "Department")
public class Department {

    @Schema(
            description = "主键ID",
            example = "322663218269917184",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private Long id;

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

    @Schema(
            description = "乐观锁版本号",
            example = "0",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private Integer version;

    @Schema(
            description = "机构/科室名称",
            example = "门诊内科",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private String name;

    @Schema(
            description = "业务编号(bus_no)",
            example = "OR0007.OR0008",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private String busNo;

    @Schema(
            description = "父级机构/科室ID",
            example = "322663105141149696",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private Long parentId;

    @Schema(
            description = "类型枚举值(type_enum)",
            example = "DEPARTMENT",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private String typeEnum;

    @Schema(
            description = "分类枚举值(class_enum)",
            example = "CLINICAL",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private String classEnum;

    @Schema(
            description = "状态枚举值(status_enum)",
            example = "ACTIVE",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private String statusEnum;

    @Schema(
            description = "激活标志位(active_flag)",
            example = "true",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private Boolean activeFlag;

    @Schema(
            description = "显示顺序",
            example = "1",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private Integer displayOrder;

    @Schema(
            description = "默认医生ID(def_doctor_id)",
            example = "308791351298756608",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private Long defDoctorId;

    @Schema(
            description = "拼音码(py_str)",
            example = "xnxnk",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private String pyStr;

    @Schema(
            description = "五笔码(wb_str)",
            example = "tntk",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private String wbStr;

    @Schema(
            description = "医保编号(yb_no)",
            example = "YB-3301-001",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private String ybNo;

    @Schema(
            description = "医保名称(yb_name)",
            example = "心血管内科(医保)",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private String ybName;

    @Schema(
            description = "医疗机构标识(medins_id)",
            example = "MEDINS-2026-001",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private String medinsId;

    @Schema(
            description = "医疗机构医保区划(medins_admdvs)",
            example = "330100",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private String medinsAdmdvs;

    @Schema(
            description = "医疗机构类型(medins_type)",
            example = "GENERAL_HOSPITAL",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private String medinsType;

    @Schema(
            description = "医疗机构等级(medins_lv)",
            example = "THREE_A",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private String medinsLv;

    @Schema(
            description = "科别(caty)",
            example = "内科",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private String caty;
}