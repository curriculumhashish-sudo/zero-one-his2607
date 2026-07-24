package com.zeroone.stat.sysmgr.entity;

import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

import java.io.Serializable;

@Data
@TableName("whale_tenants")
public class Tenant implements Serializable {
    private static final long serialVersionUID = 1L;

    @TableId
    private String id;

    private String name;

    private String code;

    @TableField("is_deleted")
    private Integer isDeleted;
}
