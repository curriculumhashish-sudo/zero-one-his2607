package com.zeroone.star.pharmacy.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serial;
import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 管理域 机构
 * </p>
 *
 * @author leisurely
 * @since 2026-07-24
 */
@Getter
@Setter
@ToString
@TableName("adm_organization")
public class AdmOrganization implements Serializable {

    @Serial
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
     * 机构聚合根实体
     */
    private String name;

    /**
     * bus 编号
     */
    private String busNo;

    /**
     * parent 标识
     */
    private String parentId;

    /**
     * type 枚举值
     */
    private String typeEnum;

    /**
     * class 枚举值
     */
    private String classEnum;

    /**
     * status 枚举值（{ PublicationStatus#getCode()}）
     */
    private String statusEnum;

    /**
     * active 标志位
     */
    private Boolean activeFlag;

    /**
     * 显示顺序
     */
    private Integer displayOrder;

    /**
     * defDoctor 标识
     */
    private String defDoctorId;

    /**
     * 拼音码
     */
    private String pyStr;

    /**
     * 五笔码
     */
    private String wbStr;

    /**
     * yb 编号
     */
    private String ybNo;

    /**
     * yb 名称
     */
    private String ybName;

    /**
     * medins 标识
     */
    private String medinsId;

    /**
     * 医疗机构医保区划
     */
    private String medinsAdmdvs;

    /**
     * medins类型
     */
    private String medinsType;

    /**
     * 医疗机构等级
     */
    private String medinsLv;

    /**
     * 科别
     */
    private String caty;
}
