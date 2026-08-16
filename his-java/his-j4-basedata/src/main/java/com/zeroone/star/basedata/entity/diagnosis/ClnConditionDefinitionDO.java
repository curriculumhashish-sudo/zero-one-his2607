package com.zeroone.star.basedata.entity.diagnosis;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.TableName;
import com.baomidou.mybatisplus.annotation.Version;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.time.LocalDateTime;

/**
 * <p>
 * 描述：临床病情定义实体，对应 cln_condition_definition 表
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Data
@TableName("cln_condition_definition")
@Schema(description = "临床病情定义")
public class ClnConditionDefinitionDO {

    /**
     * 主键标识
     */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    @Schema(description = "主键标识")
    private String id;

    /**
     * 租户标识
     */
    @TableField("tenant_id")
    @Schema(description = "租户标识")
    private String tenantId;

    /**
     * 创建时间
     */
    @TableField("created_at")
    @Schema(description = "创建时间")
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     */
    @TableField("updated_at")
    @Schema(description = "最后更新时间")
    private LocalDateTime updatedAt;

    /**
     * 创建人
     */
    @TableField("created_by")
    @Schema(description = "创建人")
    private String createdBy;

    /**
     * 最后更新人
     */
    @TableField("updated_by")
    @Schema(description = "最后更新人")
    private String updatedBy;

    /**
     * 软删标记：0 未删除 / 1 已删除
     */
    @TableField("is_deleted")
    @TableLogic
    @Schema(description = "软删标记：0 未删除 / 1 已删除")
    private Boolean isDeleted;

    /**
     * 删除时间
     */
    @TableField("deleted_at")
    @Schema(description = "删除时间")
    private LocalDateTime deletedAt;

    /**
     * 删除人
     */
    @TableField("deleted_by")
    @Schema(description = "删除人")
    private String deletedBy;

    /**
     * 乐观锁版本号
     */
    @TableField("version")
    @Version
    @Schema(description = "乐观锁版本号")
    private Integer version;

    /**
     * 来源枚举：general_dx(西医) / tcm_dx(中医) / tcm_syndrome(中医证候)
     */
    @TableField("source_enum")
    @Schema(description = "来源枚举：general_dx(西医) / tcm_dx(中医) / tcm_syndrome(中医证候)")
    private String sourceEnum;

    /**
     * 病情编码，格式如 CDF00000001
     */
    @TableField("condition_code")
    @Schema(description = "病情编码，格式如 CDF00000001")
    private String conditionCode;

    /**
     * 诊断名称
     */
    @TableField("name")
    @Schema(description = "诊断名称")
    private String name;

    /**
     * 拼音检索码
     */
    @TableField("py_str")
    @Schema(description = "拼音检索码")
    private String pyStr;

    /**
     * 五笔检索码
     */
    @TableField("wb_str")
    @Schema(description = "五笔检索码")
    private String wbStr;

    /**
     * 诊断类型编码，关联字典 diag_type(309)
     */
    @TableField("type_code")
    @Schema(description = "诊断类型编码，关联字典 diag_type(309)")
    private String typeCode;

    /**
     * 描述说明
     */
    @TableField("description")
    @Schema(description = "描述说明")
    private String description;

    /**
     * 医保标记：0 否 / 1 是
     */
    @TableField("yb_flag")
    @Schema(description = "医保标记：0 否 / 1 是")
    private Boolean ybFlag;

    /**
     * 疾病编码（ICD 映射）
     */
    @TableField("disease_code")
    @Schema(description = "疾病编码（ICD 映射）")
    private String diseaseCode;

    /**
     * 医保编号
     */
    @TableField("yb_no")
    @Schema(description = "医保编号")
    private String ybNo;

    /**
     * 医保匹配标记：0 未匹配 / 1 已匹配
     */
    @TableField("yb_match_flag")
    @Schema(description = "医保匹配标记：0 未匹配 / 1 已匹配")
    private Boolean ybMatchFlag;

    /**
     * 状态枚举：ACTIVE / INACTIVE
     */
    @TableField("status_enum")
    @Schema(description = "状态枚举：ACTIVE / INACTIVE")
    private String statusEnum;
}
