package com.zeroone.star.pharmacy.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：退药申请主表实体类，对应数据库表 {@code return_header}
 * </p>
 * <p>
 * 功能说明：记录退药申请的主单据信息，包含退药单编号、患者信息、
 * 退药类型（住院退药/门诊退药）、单据状态（待确认/已确认/已驳回/已拒绝）等。
 * 与退药明细表 return_item 为一对多关系，通过 returnId 关联。
 * 在住院退药确认和门诊拒绝退药流程中，通过更新该表状态实现业务操作。
 * 同时该表也用于分页查询退药患者队列。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Getter
@Setter
@TableName("return_header")
public class ReturnHeader implements Serializable {
    private static final long serialVersionUID = 1L;

    /**
     * 主键ID
     * 对应表字段：id，采用雪花算法自动生成
     */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /**
     * 退药单编号
     * 对应表字段：return_code，业务唯一标识
     */
    private String returnCode;

    /**
     * 患者ID
     * 对应表字段：patient_id
     */
    private String patientId;

    /**
     * 患者姓名
     * 对应表字段：patient_name
     */
    private String patientName;

    /**
     * 退药类型
     * 对应表字段：return_type
     * 0-住院退药 1-门诊退药
     */
    private Integer returnType;

    /**
     * 单据状态
     * 对应表字段：status
     * 0-待确认 1-已确认 2-已驳回 3-已拒绝
     */
    private Integer status;

    /**
     * 驳回原因
     * 对应表字段：reject_reason，仅在状态为"已驳回"或"已拒绝"时有值
     */
    private String rejectReason;

    /**
     * 创建人
     * 对应表字段：create_by，由 MyBatis-Plus 自动填充
     */
    private String createBy;

    /**
     * 创建时间
     * 对应表字段：create_time，由 MyBatis-Plus 自动填充
     */
    private LocalDateTime createTime;

    /**
     * 确认人
     * 对应表字段：confirm_by，退药确认时的操作人
     */
    private String confirmBy;

    /**
     * 确认时间
     * 对应表字段：confirm_time，退药确认时的操作时间
     */
    private LocalDateTime confirmTime;
}
