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
 * 描述：汇总单表实体类，对应数据库表 {@code summary_order}
 * </p>
 * <p>
 * 功能说明：记录发药汇总单信息，包含汇总单编号、名称、状态等。
 * 在住院发药流程中，先获取汇总单队列，再根据汇总单查询患者药品项目。
 * 在发药驳回时，同步驳回该汇总单（将状态更新为"已驳回"）。
 * 汇总单与发药单据为一对多关系。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Getter
@Setter
@TableName("summary_order")
public class SummaryOrder implements Serializable {
    private static final long serialVersionUID = 1L;

    /**
     * 主键ID
     * 对应表字段：id，采用雪花算法自动生成
     */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /**
     * 汇总单编号
     * 对应表字段：summary_code，业务唯一标识
     */
    private String summaryCode;

    /**
     * 汇总单名称
     * 对应表字段：summary_name
     */
    private String summaryName;

    /**
     * 状态
     * 对应表字段：status
     * 0-待发药 1-已发药 2-已驳回
     */
    private Integer status;

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
