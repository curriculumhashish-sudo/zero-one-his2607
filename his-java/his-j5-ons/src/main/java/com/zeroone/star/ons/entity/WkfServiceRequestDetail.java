package com.zeroone.star.ons.entity;

import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;
import lombok.EqualsAndHashCode;

@Data
@EqualsAndHashCode(callSuper = true)
@TableName("wkf_service_request_detail")
public class WkfServiceRequestDetail extends BaseEntity {

    @TableField("service_req_id")
    private String serviceReqId;

    @TableField("target_table")
    private String targetTable;

    @TableField("target_id")
    private String targetId;

    @TableField("remark")
    private String remark;
}