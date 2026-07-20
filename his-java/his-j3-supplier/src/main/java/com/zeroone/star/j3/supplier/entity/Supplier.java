package com.zeroone.star.j3.supplier.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 供应商表
 * </p>
 *
 * @author 业之
 * @since 2026-07-20
 */
@Getter
@Setter
@ToString
public class Supplier implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 供应商ID
     */
    @TableId(value = "id", type = IdType.AUTO)
    private Long id;

    /**
     * 供应商编号
     */
    private String supplierCode;

    /**
     * 供应商名称
     */
    private String supplierName;

    /**
     * 供应商类型（生产厂家/供应商）
     */
    private String supplierType;

    /**
     * 是否启用 1 启用 0 禁用
     */
    private Boolean isEnabled;

    /**
     * 信用代码
     */
    private String creditCode;

    /**
     * 联系人
     */
    private String contactPerson;

    /**
     * 联系电话
     */
    private String contactPhone;

    /**
     * 地址
     */
    private String address;

    /**
     * 资质信息
     */
    private String qualificationInfo;

    /**
     * 创建时间
     */
    private LocalDateTime createdAt;

    /**
     * 更新时间
     */
    private LocalDateTime updatedAt;
}
