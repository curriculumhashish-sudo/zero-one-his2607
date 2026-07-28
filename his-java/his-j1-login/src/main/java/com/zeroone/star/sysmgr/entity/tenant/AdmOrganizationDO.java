package com.zeroone.star.sysmgr.entity.tenant;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.TableName;
import com.baomidou.mybatisplus.annotation.Version;
import lombok.Data;
import java.time.LocalDateTime;

@Data
@TableName("adm_organization")
public class AdmOrganizationDO {

    @TableId(type = IdType.ASSIGN_UUID)
    private String id;

    private String tenantId;

    private LocalDateTime createdAt;

    private LocalDateTime updatedAt;

    private String createdBy;

    private String updatedBy;

    @TableLogic(value = "0", delval = "1")
    private Integer isDeleted;

    private LocalDateTime deletedAt;

    private String deletedBy;

    @Version
    private Integer version;

    private String name;

    private String busNo;

    private String parentId;

    private String typeEnum;

    private String classEnum;

    private String statusEnum;

    private Integer activeFlag;

    private Integer displayOrder;

    private String defDoctorId;

    private String pyStr;

    private String wbStr;

    private String ybNo;

    private String ybName;

    private String medinsId;

    private String medinsAdmdvs;

    private String medinsType;

    private String medinsLv;

    private String caty;
}