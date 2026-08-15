package com.zeroone.star.j3.storehouse.entity;

import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

/**
 * 科室实体（映射 adm_organization 表，仅用于查询）
 * @author mie
 * @version 1.0.0
 */
@Data
@TableName("adm_organization")
public class DepartmentDO {

    private String id;

    private String tenantId;

    private String name;

    private String typeEnum;

    private String statusEnum;

    private Boolean activeFlag;

    @TableLogic(value = "false", delval = "true")
    private Boolean isDeleted;
}
