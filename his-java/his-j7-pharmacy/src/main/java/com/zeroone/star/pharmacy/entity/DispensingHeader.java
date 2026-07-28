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
 * 描述：发药单据主表实体类，对应数据库表 {@code dispensing_header}
 * </p>
 * <p>
 * 功能说明：用于记录药房发药操作的主单据信息，包含发药单编号、患者信息、
 * 发药类型（住院/门诊）、单据状态（待发药/已发药/已驳回）等核心字段。
 * 与发药明细表 dispensing_item 为一对多关系，通过 dispensingId 关联。
 * 在住院发药驳回业务流程中，通过更新该表状态实现发药驳回操作。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Getter
@Setter
@TableName("dispensing_header")
public class DispensingHeader implements Serializable {
    private static final long serialVersionUID = 1L;

    /**
     * 主键ID
     * 对应表字段：id，采用雪花算法自动生成
     */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /**
     * 发药单编号
     * 对应表字段：dispensing_code，业务唯一标识
     */
    private String dispensingCode;

    /**
     * 汇总单ID
     * 对应表字段：summary_id，关联 summary_order 表
     */
    private String summaryId;

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
     * 科室ID
     * 对应表字段：dept_id
     */
    private String deptId;

    /**
     * 发药类型
     * 对应表字段：dispensing_type
     * 0-住院发药 1-门诊发药
     */
    private Integer dispensingType;

    /**
     * 单据状态
     * 对应表字段：status
     * 0-待发药 1-已发药 2-已驳回
     */
    private Integer status;

    /**
     * 驳回原因
     * 对应表字段：reject_reason，仅在状态为"已驳回"时有值
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
     * 更新人
     * 对应表字段：update_by，由 MyBatis-Plus 自动填充
     */
    private String updateBy;

    /**
     * 更新时间
     * 对应表字段：update_time，由 MyBatis-Plus 自动填充
     */
    private LocalDateTime updateTime;
}
