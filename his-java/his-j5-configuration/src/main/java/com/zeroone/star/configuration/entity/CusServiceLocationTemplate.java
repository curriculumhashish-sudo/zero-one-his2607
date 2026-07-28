package com.zeroone.star.configuration.entity;

import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableName;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Size;
import lombok.Data;

import java.time.LocalTime;

@Data
@TableName("cus_service_location_template")
public class CusServiceLocationTemplate extends BaseEntity {

    @NotBlank(message = "机构标识不能为空")
    @Size(max = 64, message = "机构标识长度不能超过64个字符")
    @TableField("organization_id")
    private String organizationId;

    @NotBlank(message = "项table不能为空")
    @Size(max = 64, message = "项table长度不能超过64个字符")
    @TableField("item_table")
    private String itemTable;

    @NotBlank(message = "分发类别编码不能为空")
    @Size(max = 64, message = "分发类别编码长度不能超过64个字符")
    @TableField("distribution_category_code")
    private String distributionCategoryCode;

    @NotBlank(message = "默认位置标识不能为空")
    @Size(max = 64, message = "默认位置标识长度不能超过64个字符")
    @TableField("def_location_id")
    private String defLocationId;

    @Size(max = 64, message = "项定义标识长度不能超过64个字符")
    @TableField("item_definition_id")
    private String itemDefinitionId;

    @Size(max = 64, message = "方法编码长度不能超过64个字符")
    @TableField("method_code")
    private String methodCode;

    @TableField("start_time")
    private LocalTime startTime;

    @TableField("end_time")
    private LocalTime endTime;

    @NotNull(message = "优先级不能为空")
    @TableField("priority")
    private Integer priority;

    @NotNull(message = "显示顺序不能为空")
    @TableField("display_order")
    private Integer displayOrder;

    @Size(max = 500, message = "备注长度不能超过500个字符")
    @TableField("remark")
    private String remark;


}