package com.zeroone.star.sysmgr.entity.resource;

import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableName;
import com.zeroone.star.project.entity.BaseDO;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

@Getter
@Setter
@ToString
@TableName("adm_organization")
public class AdmOrganization extends BaseDO {
    @TableField("name")
    private String name;
    @TableField("type_enum")
    private String typeEnum;
    @TableField("status_enum")
    private String statusEnum;
    @TableField("active_flag")
    private Boolean activeFlag;
}
