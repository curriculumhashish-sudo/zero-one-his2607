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
 * 描述：耗材设备请求实体类，对应数据库表 {@code wkf_device_request}
 * </p>
 * <p>
 * 功能说明：记录耗材/设备的请求信息（类似药品处方），包含处方编号、
 * 设备类型编码、数量、设备定义ID、患者标识、request时间、执行位置等。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Getter
@Setter
@TableName("wkf_device_request")
public class WkfDeviceRequest implements Serializable {

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

    /** 处方编号 */
    private String prescriptionNo;

    /** 活动标识 */
    private String activityId;

    /** package标识 */
    private String packageId;

    /** 状态枚举值 */
    private String statusEnum;

    /** intent编码 */
    private String intentCode;

    /** category枚举值 */
    private String categoryEnum;

    /** priority枚举值 */
    private String priorityEnum;

    /** perform标志位 */
    private Integer performFlag;

    /** 分组编号 */
    private String groupNo;

    /** 设备类型编码 */
    private String deviceTypeCode;

    /** 数量 */
    private BigDecimal quantity;

    /** 单元编码 */
    private String unitCode;

    /** 批号 */
    private String lotNumber;

    /** 设备def标识 */
    private String deviceDefId;

    /** 设备specifications */
    private String deviceSpecifications;

    /** requester标识 */
    private String requesterId;

    /** 机构标识 */
    private String orgId;

    /** 位置/科室标识 */
    private String locationId;

    /** perform位置 */
    private Long performLocation;

    /** 就诊标识 */
    private String encounterId;

    /** 患者标识 */
    private String patientId;

    /** rate编码 */
    private String rateCode;

    /** use时间 */
    private LocalDateTime useTime;

    /** use开始时间 */
    private LocalDateTime useStartTime;

    /** use结束时间 */
    private LocalDateTime useEndTime;

    /** 用药时机 */
    private String useTiming;

    /** reqauthored时间 */
    private LocalDateTime reqAuthoredTime;

    /** performer枚举值 */
    private String performerEnum;

    /** performer标识 */
    private String performerId;

    /** performorg标识 */
    private String performOrgId;

    /** asneed标志位 */
    private Integer asNeedFlag;

    /** asneed原因 */
    private String asNeedReason;

    /** 合同编码 */
    private String contractCode;

    /** refund设备标识 */
    private String refundDeviceId;

    /** 医保class枚举值 */
    private String ybClassEnum;

    /** 追踪编号 */
    private String traceNo;

    /** 耗材请求聚合根 */
    private String conditionDefinitionId;

    /** 就诊诊断标识 */
    private String encounterDiagnosisId;

    /** 观察项ID JSON */
    private String observationIdJson;

    /** 辅助信息 */
    private String supportInfo;

    /** 依据表名 */
    private String basedOnTable;

    /** basedon标识 */
    private String basedOnId;

    /** generate来源枚举值 */
    private String generateSourceEnum;

    /** 备注 */
    private String remark;

    /** 排序号 */
    private Integer sortNumber;
}
