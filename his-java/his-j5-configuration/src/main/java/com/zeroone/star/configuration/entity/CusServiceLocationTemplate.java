package com.zeroone.star.configuration.entity;

import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

import java.time.LocalTime;

@Data
@TableName("cus_service_location_template")
public class CusServiceLocationTemplate extends BaseEntity {

    @TableField("organization_id")
    private String organizationId;

    @TableField("item_table")
    private String itemTable;

    @TableField("distribution_category_code")
    private String distributionCategoryCode;

    @TableField("def_location_id")
    private String defLocationId;

    @TableField("item_definition_id")
    private String itemDefinitionId;

    @TableField("method_code")
    private String methodCode;

    @TableField("start_time")
    private LocalTime startTime;

    @TableField("end_time")
    private LocalTime endTime;

    @TableField("priority")
    private Integer priority;

    @TableField("display_order")
    private Integer displayOrder;

    @TableField("remark")
    private String remark;


}