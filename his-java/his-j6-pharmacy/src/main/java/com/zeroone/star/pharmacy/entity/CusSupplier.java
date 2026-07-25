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
 * 自定义 供应商
 * </p>
 *
 * @author leisurely
 * @since 2026-07-24
 */
@Getter
@Setter
@ToString
@TableName("cus_supplier")
public class CusSupplier implements Serializable {

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
     * 业务编号
     */
    private String busNo;

    /**
     * 名称
     */
    private String name;

    /**
     * 类型枚举值
     */
    private String typeEnum;

    /**
     * credit编码
     */
    private String creditCode;

    /**
     * 法定代表人
     */
    private String legalRepresentative;

    /**
     * 地址
     */
    private String address;

    /**
     * 拼音码
     */
    private String pyStr;

    /**
     * 五笔码
     */
    private String wbStr;

    /**
     * 电话
     */
    private String phone;

    /**
     * 邮箱
     */
    private String email;

    /**
     * bank名称
     */
    private String bankName;

    /**
     * bank账户
     */
    private String bankAccount;

    /**
     * 机构标识
     */
    private String orgId;

    /**
     * 显示顺序
     */
    private Integer displayOrder;

    /**
     * 状态枚举值
     */
    private String statusEnum;

    /**
     * 备注说明
     */
    private String note;
}
