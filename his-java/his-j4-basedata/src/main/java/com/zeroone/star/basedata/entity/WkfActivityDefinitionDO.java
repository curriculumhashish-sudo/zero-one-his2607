package com.zeroone.star.basedata.entity;

import com.baomidou.mybatisplus.annotation.*;
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
public class WkfActivityDefinitionDO implements Serializable {

    private static final long serialVersionUID = 1L;

    /** 主键 */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /** 租户标识 */
    private String tenantId;

    /** 创建时间 */
    @TableField(fill = FieldFill.INSERT)
    private LocalDateTime createdAt;

    /** 最后更新时间 */
    @TableField(fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

    /** 创建人 */
    private String createdBy;

    /** 最后更新人 */
    private String updatedBy;

    /** 软删标记 0-未删除 1-已删除 */
    @TableLogic
    private Integer isDeleted;

    /** 删除时间 */
    private LocalDateTime deletedAt;

    /** 删除人 */
    private String deletedBy;

    /** 乐观锁版本号 */
    @Version
    private Integer version;

    /** 诊疗项目定义聚合根 */
    private String categoryCode;

    /** 业务编码（全局唯一） */
    private String busNo;

    /** 项目名称 */
    private String name;

    /** 拼音检索码 */
    private String pyStr;

    /** 五笔检索码 */
    private String wbStr;

    /** 销售/计价单位编码 */
    private String packageUnitCode;

    /** 诊疗类型枚举编码 */
    private String typeEnum;

    /** 发布状态枚举编码 */
    private String statusEnum;

    /** 说明文本 */
    private String descriptionText;

    /** 身体部位编码 */
    private String bodySiteCode;

    /** 标本编码 */
    private String specimenCode;

    /** 所属机构标识 */
    private String orgId;

    /** 位置标识 */
    private String locationId;

    /** 计费规则标识 */
    private Integer ruleId;

    /** 划价标记 */
    private Integer pricingFlag;

    /** 医保标记 */
    private Integer ybFlag;

    /** 医保编码 */
    private String ybNo;

    /** 医保对码标记 */
    private Integer ybMatchFlag;

    /** 医保收费等级 */
    private Integer chrgitmLv;

    /** 子项 JSON 快照 */
    private String childrenJson;

    /** 子项标记（需与子项子表事实一致） */
    private Integer childrenFlag;

    /** 诊疗项目业务版本（非乐观锁version） */
    private String activityVersion;

    /** 商品名称（可与项目名称分列维护） */
    private String merchandiseName;

    /** 商品名拼音检索码 */
    private String merchandisePyStr;

    /** 商品名五笔检索码 */
    private String merchandiseWbStr;

    /** 默认门诊/就诊单位编码 */
    private String defEncounterUnitCode;

    /** 默认住院单位编码 */
    private String defInhospitalUnitCode;
}
