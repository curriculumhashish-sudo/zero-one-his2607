package com.zeroone.star.sysmgr.entity.tenant;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;
import java.time.LocalDateTime;

@Data
@TableName("whale_tenants")
public class TenantDO {

    @TableId(type = IdType.ASSIGN_ID)
    private Long id;

    private String name;

    private String code;

    private String description;

    private Integer isActive;

    private String extraProperties;

    private LocalDateTime createdAt;

    private LocalDateTime updatedAt;

    private Long createdBy;

    private Long updatedBy;

    @TableLogic(value = "0", delval = "1")
    private Integer isDeleted;

    private LocalDateTime deletedAt;

    private Long deletedBy;
}