package com.zeroone.star.dispense.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.TableName;
import com.baomidou.mybatisplus.annotation.Version;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serial;
import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 位置聚合根实体（adm_location）
 * </p>
 * <p>涵盖药房、科室、病区、床位等物理/逻辑位置</p>
 *
 * @author 再睡一夏
 * @since 2026-07-28
 */
@Getter
@Setter
@ToString
@TableName("adm_location")
public class AdmLocation implements Serializable {

    @Serial
    private static final long serialVersionUID = 1L;

    /** 主键 */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /** 租户标识 */
    private String tenantId;

    /** 创建时间 */
    private LocalDateTime createdAt;

    /** 最后更新时间 */
    private LocalDateTime updatedAt;

    /** 创建人 */
    private String createdBy;

    /** 最后更新人 */
    private String updatedBy;

    /** 软删标记 */
    @TableLogic
    private Boolean isDeleted;

    /** 删除时间 */
    private LocalDateTime deletedAt;

    /** 删除人 */
    private String deletedBy;

    /** 乐观锁版本号 */
    @Version
    private Integer version;

    /** 位置名称（如：门诊西药房、门诊内科） */
    private String name;

    /** 业务编号 */
    private String busNo;

    /** 状态枚举（如：ACTIVE / INACTIVE） */
    private String statusEnum;

    /** 运营状态枚举 */
    private String operationalEnum;

    /** 形态枚举（如：PHARMACY-药房 / CLINIC-诊室 / WARD-病区 / BED-床位） */
    private String formEnum;

    /** 模式枚举（如：OUTPATIENT-门诊 / INPATIENT-住院） */
    private String modeEnum;

    /** 类型 JSON 快照（如：{"type":"药房"}） */
    private String typeJson;

    /** 所属机构标识 */
    private String organizationId;

    /** 显示顺序 */
    private Integer displayOrder;

    /** 拼音码 */
    private String pyStr;

    /** 五笔码 */
    private String wbStr;

    /** 父级位置标识 */
    private String parentId;
}
