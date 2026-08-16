package com.zeroone.star.pharmacy.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serial;
import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：汇总单表实体类，对应数据库表 {@code summary_sheet}
 * </p>
 * <p>
 * 功能说明：记录发药汇总单信息，包含汇总单编号、名称、病区、药房、状态等。
 * 在住院发药流程中，先获取汇总单队列，再根据汇总单查询患者药品项目。
 * 在发药驳回时，同步驳回该汇总单（将状态更新为"已驳回"）。
 * 汇总单与发药单据为一对多关系（一个汇总单下包含多条发药明细）。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Getter
@Setter
@ToString
@TableName("summary_sheet")
public class SummarySheet implements Serializable {

    @Serial
    private static final long serialVersionUID = 1L;

    /**
     * 主键ID
     * 对应表字段：id，采用雪花算法自动生成
     */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /**
     * 汇总单编号
     * 对应表字段：summary_no，业务唯一标识
     * 示例：HZ20260719001
     */
    private String summaryNo;

    /**
     * 汇总单名称
     * 对应表字段：summary_name
     */
    private String summaryName;

    /**
     * 病区ID
     * 对应表字段：ward_id，关联 adm_location.id
     */
    private String wardId;

    /**
     * 药房科室ID
     * 对应表字段：dept_id，关联 adm_organization.id
     */
    private String deptId;

    /**
     * 状态
     * 对应表字段：status
     * 0-待发药 1-已发药 2-已驳回
     */
    private Integer status;

    /**
     * 药品总数量
     * 对应表字段：total_qty
     */
    private BigDecimal totalQty;

    /**
     * 患者数
     * 对应表字段：patient_count
     */
    private Long patientCount;

    /**
     * 操作人ID
     * 对应表字段：operator_id，关联 adm_practitioner.id
     */
    private String operatorId;

    /**
     * 发药时间
     * 对应表字段：dispense_time
     */
    private LocalDateTime dispenseTime;

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
