package com.zeroone.star.pharmacy.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 管理域 地点
 * </p>
 *
 * @author leisurely
 * @since 2026-07-24
 */
@Getter
@Setter
@ToString
@TableName("adm_location")
public class AdmLocation implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键
     */
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
    private Boolean isDeleted;

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
     * 位置聚合根实体
     */
    private String name;

    /**
     * bus 编号
     */
    private String busNo;

    /**
     * status 枚举值
     */
    private String statusEnum;

    /**
     * operational 枚举值
     */
    private String operationalEnum;

    /**
     * form 枚举值
     */
    private String formEnum;

    /**
     * mode 枚举值
     */
    private String modeEnum;

    /**
     * type JSON 快照
     */
    private String typeJson;

    /**
     * 机构 标识
     */
    private String organizationId;

    /**
     * 显示顺序
     */
    private Integer displayOrder;

    /**
     * 拼音码
     */
    private String pyStr;

    /**
     * 五笔码
     */
    private String wbStr;

    /**
     * parent 标识
     */
    private String parentId;
}
