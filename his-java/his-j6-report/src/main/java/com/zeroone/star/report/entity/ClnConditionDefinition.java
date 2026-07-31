package com.zeroone.star.report.entity;

import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import java.io.Serializable;
import java.time.LocalDateTime;
import lombok.Data;

/**
 * 临床 病情定义
 * @TableName cln_condition_definition
 * @author j6小组-TKORHP
 */
@TableName(value = "cln_condition_definition")
@Data
public class ClnConditionDefinition implements Serializable {
    /**
     * 主键
     */
    @TableId
    private String id;

    /**
     * 租户标识
     */
    private String tenantId;

    /**
     * 创建时间
     */
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     */
    private LocalDateTime updatedAt;

    /**
     * 创建人
     */
    private String createdBy;

    /**
     * 最后更新人
     */
    private String updatedBy;

    /**
     * 软删标记
     */
    private Integer isDeleted;

    /**
     * 删除时间
     */
    private LocalDateTime deletedAt;

    /**
     * 删除人
     */
    private String deletedBy;

    /**
     * 乐观锁版本号
     */
    private Integer version;

    /**
     * 来源枚举值
     */
    private String sourceEnum;

    /**
     * 病情编码
     */
    private String conditionCode;

    /**
     * 名称
     */
    private String name;

    /**
     * 拼音码
     */
    private String pyStr;

    /**
     * 五笔码
     */
    private String wbStr;

    /**
     * 类型编码
     */
    private String typeCode;

    /**
     * 描述
     */
    private String description;

    /**
     * 医保标志位
     */
    private Integer ybFlag;

    /**
     * disease编码
     */
    private String diseaseCode;

    /**
     * 医保编号
     */
    private String ybNo;

    /**
     * 医保match标志位
     */
    private Integer ybMatchFlag;

    /**
     * 状态枚举值
     */
    private String statusEnum;

    @TableField(exist = false)
    private static final long serialVersionUID = 1L;
}
