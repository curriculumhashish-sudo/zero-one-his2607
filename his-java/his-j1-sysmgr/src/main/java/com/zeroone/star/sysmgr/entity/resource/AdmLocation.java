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
@TableName("adm_location")
public class AdmLocation extends BaseDO {
    @TableField("name")
    private String name;
    @TableField("form_enum")
    private String formEnum;
    @TableField("status_enum")
    private String statusEnum;
    @TableField("organization_id")
    private String organizationId;
}
