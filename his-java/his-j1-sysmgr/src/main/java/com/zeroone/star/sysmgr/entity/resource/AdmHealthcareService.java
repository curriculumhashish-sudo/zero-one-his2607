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
@TableName("adm_healthcare_service")
public class AdmHealthcareService extends BaseDO {
    @TableField("name")
    private String name;
    @TableField("type_code")
    private String typeCode;
    @TableField("status_enum")
    private String statusEnum;
    @TableField("active_flag")
    private Boolean activeFlag;
}
