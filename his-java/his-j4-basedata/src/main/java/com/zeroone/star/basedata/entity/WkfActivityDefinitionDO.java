package com.zeroone.star.basedata.entity;

import com.baomidou.mybatisplus.annotation.*;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：诊疗管理实体
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Data
@TableName("wkf_activity_definition")
@Schema(description = "工作流activity定义（诊疗项目管理）")
public class WkfActivityDefinitionDO implements Serializable {

    private static final long serialVersionUID = 1L;

    /** 主键 */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    @Schema(description = "主键", example = "123456789", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    /** 租户标识 */
    @TableField("tenant_id")
    @Schema(description = "租户标识", example = "tenant_001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String tenantId;

    /** 创建时间 */
    @TableField(value = "created_at", fill = FieldFill.INSERT)
    @Schema(description = "创建时间", example = "2026-01-01 12:00:00", requiredMode = Schema.RequiredMode.REQUIRED)
    private LocalDateTime createdAt;

    /** 最后更新时间 */
    @TableField(value = "updated_at", fill = FieldFill.INSERT_UPDATE)
    @Schema(description = "最后更新时间", example = "2026-01-01 12:00:00", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private LocalDateTime updatedAt;

    /** 创建人 */
    @TableField("created_by")
    @Schema(description = "创建人", example = "admin", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String createdBy;

    /** 最后更新人 */
    @TableField("updated_by")
    @Schema(description = "最后更新人", example = "admin", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String updatedBy;

    /** 软删标记 0-未删除 1-已删除 */
    @TableLogic
    @TableField("is_deleted")
    @Schema(description = "软删标记 0-未删除 1-已删除", example = "0", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer isDeleted;

    /** 删除时间 */
    @TableField("deleted_at")
    @Schema(description = "删除时间", example = "2026-01-01 12:00:00", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private LocalDateTime deletedAt;

    /** 删除人 */
    @TableField("deleted_by")
    @Schema(description = "删除人", example = "admin", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String deletedBy;

    /** 乐观锁版本号 */
    @Version
    @TableField("version")
    @Schema(description = "乐观锁版本号", example = "0", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer version;

    /** 诊疗项目定义聚合根 */
    @TableField("category_code")
    @Schema(description = "诊疗项目定义聚合根", example = "category_001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String categoryCode;

    /** 业务编码（全局唯一） */
    @TableField("bus_no")
    @Schema(description = "业务编码（全局唯一）", example = "BUS20260001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;

    /** 项目名称 */
    @TableField("name")
    @Schema(description = "项目名称", example = "血常规检查", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    /** 拼音检索码 */
    @TableField("py_str")
    @Schema(description = "拼音检索码", example = "xuechanggui", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String pyStr;

    /** 五笔检索码 */
    @TableField("wb_str")
    @Schema(description = "五笔检索码", example = "xtsg", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String wbStr;

    /** 销售/计价单位编码 */
    @TableField("package_unit_code")
    @Schema(description = "销售/计价单位编码", example = "unit_001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String packageUnitCode;

    /** 诊疗类型枚举编码 */
    @TableField("type_enum")
    @Schema(description = "诊疗类型枚举编码", example = "CHECK", requiredMode = Schema.RequiredMode.REQUIRED)
    private String typeEnum;

    /** 发布状态枚举编码 */
    @TableField("status_enum")
    @Schema(description = "发布状态枚举编码", example = "PUBLISH", requiredMode = Schema.RequiredMode.REQUIRED)
    private String statusEnum;

    /** 说明文本 */
    @TableField("description_text")
    @Schema(description = "说明文本", example = "常规血液检测项目", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String descriptionText;

    /** 身体部位编码 */
    @TableField("body_site_code")
    @Schema(description = "身体部位编码", example = "BODY_BLOOD", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String bodySiteCode;

    /** 标本编码 */
    @TableField("specimen_code")
    @Schema(description = "标本编码", example = "SPEC_BLOOD", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String specimenCode;

    /** 所属机构标识 */
    @TableField("org_id")
    @Schema(description = "所属机构标识", example = "org_001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String orgId;

    /** 位置标识 */
    @TableField("location_id")
    @Schema(description = "位置标识", example = "loc_001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String locationId;

    /** 计费规则标识 */
    @TableField("rule_id")
    @Schema(description = "计费规则标识", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer ruleId;

    /** 划价标记 */
    @TableField("pricing_flag")
    @Schema(description = "划价标记 0-否 1-是", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer pricingFlag;

    /** 医保标记 */
    @TableField("yb_flag")
    @Schema(description = "医保标记 0-否 1-是", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer ybFlag;

    /** 医保编码 */
    @TableField("yb_no")
    @Schema(description = "医保编码", example = "YB20260001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String ybNo;

    /** 医保对码标记 */
    @TableField("yb_match_flag")
    @Schema(description = "医保对码标记 0-未对码 1-已对码", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer ybMatchFlag;

    /** 医保收费等级 */
    @TableField("chrgitm_lv")
    @Schema(description = "医保收费等级", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer chrgitmLv;

    /** 子项 JSON 快照 */
    @TableField("children_json")
    @Schema(description = "子项JSON快照", example = "[]", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String childrenJson;

    /** 子项标记（需与子项子表事实一致） */
    @TableField("children_flag")
    @Schema(description = "子项标记 0-无子项 1-有子项", example = "0", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer childrenFlag;

    /** 诊疗项目业务版本（非乐观锁version） */
    @TableField("activity_version")
    @Schema(description = "诊疗项目业务版本（非乐观锁version）", example = "V1.0", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String activityVersion;

    /** 商品名称（可与项目名称分列维护） */
    @TableField("merchandise_name")
    @Schema(description = "商品名称（可与项目名称分列维护）", example = "血常规检测套餐", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String merchandiseName;

    /** 商品名拼音检索码 */
    @TableField("merchandise_py_str")
    @Schema(description = "商品名拼音检索码", example = "xuechangguetaocan", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String merchandisePyStr;

    /** 商品名五笔检索码 */
    @TableField("merchandise_wb_str")
    @Schema(description = "商品名五笔检索码", example = "xtsgte", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String merchandiseWbStr;

    /** 默认门诊/就诊单位编码 */
    @TableField("def_encounter_unit_code")
    @Schema(description = "默认门诊/就诊单位编码", example = "out_unit_01", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String defEncounterUnitCode;

    /** 默认住院单位编码 */
    @TableField("def_inhospital_unit_code")
    @Schema(description = "默认住院单位编码", example = "in_unit_01", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String defInhospitalUnitCode;
}
