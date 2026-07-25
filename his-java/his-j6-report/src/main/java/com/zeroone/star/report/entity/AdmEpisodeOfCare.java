package com.zeroone.star.report.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import java.io.Serializable;
import java.time.LocalDateTime;
import lombok.Data;

/**
 * 管理域 诊疗周期
 * @TableName adm_episode_of_care
 * @author wanfengzhiyunyi
 */
@TableName(value ="adm_episode_of_care")
@Data
public class AdmEpisodeOfCare implements Serializable {
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
     * 患者 标识
     */
    private String patientId;

    /**
     * 机构 标识
     */
    private String organizationId;

    /**
     * bus 编号
     */
    private String busNo;

    /**
     * careTeam 标识
     */
    private String careTeamId;

    /**
     * care类型
     */
    private String careType;

    /**
     * 照护目标
     */
    private String careGoal;

    /**
     * status 枚举值
     */
    private String statusEnum;

    /**
     * active 标志位
     */
    private Integer activeFlag;

    /**
     * start 时间
     */
    private LocalDateTime startTime;

    /**
     * end 时间
     */
    private LocalDateTime endTime;

    @TableField(exist = false)
    private static final long serialVersionUID = 1L;
}