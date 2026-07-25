package com.zeroone.star.pharmacy.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：耗材设备发药实体类，对应数据库表 {@code wkf_device_dispense}
 * </p>
 * <p>
 * 功能说明：记录耗材/设备的发药执行信息，包含设备请求ID、发药状态、
 * 发药类别、数量、批号、设备定义ID、患者标识、发药时间等。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Getter
@Setter
@TableName("wkf_device_dispense")
public class WkfDeviceDispense implements Serializable {

    private static final long serialVersionUID = 1L;

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
    private Integer isDeleted;

    /** 删除时间 */
    private LocalDateTime deletedAt;

    /** 删除人 */
    private String deletedBy;

    /** 乐观锁版本号 */
    private Integer version;

    /** 业务编号 */
    private String busNo;

    /** 设备req标识 */
    private String deviceReqId;

    /** 状态枚举值 */
    private String statusEnum;

    /** 依据表名 */
    private String basedOnTable;

    /** basedon标识 */
    private String basedOnId;

    /** notperformed原因枚举值 */
    private String notPerformedReasonEnum;

    /** 发药category枚举值 */
    private String dispenseCategoryEnum;

    /** 数量 */
    private BigDecimal quantity;

    /** 单元编码 */
    private String unitCode;

    /** 批号 */
    private String lotNumber;

    /** 设备def标识 */
    private String deviceDefId;

    /** 患者标识 */
    private String patientId;

    /** 就诊标识 */
    private String encounterId;

    /** performer标识 */
    private String performerId;

    /** 位置/科室标识 */
    private String locationId;

    /** part类型编码 */
    private String partTypeCode;

    /** 发药数量 */
    private BigDecimal dispenseQuantity;

    /** preparer标识 */
    private String preparerId;

    /** prepare时间 */
    private LocalDateTime prepareTime;

    /** 发药时间 */
    private LocalDateTime dispenseTime;

    /** limit时间 */
    private LocalDateTime limitTime;

    /** planned发药时间 */
    private LocalDateTime plannedDispenseTime;

    /** des位置标识 */
    private String desLocationId;

    /** rec医护人员标识 */
    private String recPractitionerId;

    /** 辅助信息 */
    private String supportInfo;

    /** 用法说明 */
    private String usageInstruction;

    /** 发药频次编码 */
    private String dispenseFrequencyCode;

    /** 追踪编号 */
    private String traceNo;

    /** 处置标识 */
    private String procedureId;
}
