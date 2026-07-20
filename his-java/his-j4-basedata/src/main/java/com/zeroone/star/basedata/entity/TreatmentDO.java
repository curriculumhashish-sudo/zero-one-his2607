package com.zeroone.star.basedata.entity;

import com.baomidou.mybatisplus.annotation.*;
import lombok.Data;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：诊疗管理实体
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Data
@TableName("treatment")
public class TreatmentDO implements Serializable {

    private static final long serialVersionUID = 1L;

    /** 主键ID，雪花算法生成 */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /** 租户ID */
    private String tenantId;

    /** 创建时间 */
    private LocalDateTime createdAt;

    /** 更新时间 */
    private LocalDateTime updatedAt;

    /** 创建人 */
    private String createdBy;

    /** 更新人 */
    private String updatedBy;

    /** 逻辑删除标记，0未删除/1已删除 */
    @TableLogic
    private Boolean isDeleted;

    /** 删除时间 */
    private LocalDateTime deletedAt;

    /** 删除人 */
    private String deletedBy;

    /** 乐观锁版本号 */
    @Version
    private Integer version;

    /** 诊疗项目名称 */
    private String name;

    /** 服务类型编码 */
    private String typeCode;

    /** 分类编码 */
    private String categoryCode;

    /** 提供机构ID */
    private String offeredOrgId;

    /** 专业科室编码 */
    private String specialtyCode;

    /** 执行地点ID */
    private String locationId;

    /** 状态枚举：ACTIVE启用/INACTIVE停用 */
    private String statusEnum;

    /** 是否有效标记 */
    private Boolean activeFlag;

    /** 是否需要预约标记 */
    private Boolean appointmentRequiredFlag;

    /** 医保编码 */
    private String ybNo;

    /** 业务编号（编码） */
    private String busNo;

    /** 备注说明 */
    private String comment;

    /** 扩展详情 */
    private String extraDetails;

    /** 联系方式 */
    private String contact;
}
