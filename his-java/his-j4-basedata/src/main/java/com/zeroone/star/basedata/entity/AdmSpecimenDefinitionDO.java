package com.zeroone.star.basedata.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.TableName;
import com.baomidou.mybatisplus.annotation.Version;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：样本定义实体，对应 adm_specimen_definition 表
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Getter
@Setter
@ToString
@TableName("adm_specimen_definition")
public class AdmSpecimenDefinitionDO implements Serializable {

    private static final long serialVersionUID = 1L;

    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /**
     * 租户标识
     */
    private String tenantId;

    /**
     * 创建时间
     */
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     */
    private LocalDateTime updatedAt;

    /**
     * 创建人
     */
    private String createdBy;

    /**
     * 最后更新人
     */
    private String updatedBy;

    /**
     * 软删标记：0 未删除 / 1 已删除
     */
    @TableLogic
    private Boolean isDeleted;

    /**
     * 删除时间
     */
    private LocalDateTime deletedAt;

    /**
     * 删除人
     */
    private String deletedBy;

    /**
     * 乐观锁版本号
     */
    @Version
    private Integer version;

    /**
     * 样本定义聚合根/业务编号
     */
    private String busNo;

    /**
     * 面向临床与维护页的显示名称
     */
    private String name;

    /**
     * 拼音检索码
     */
    private String pyStr;

    /**
     * 五笔检索码
     */
    private String wbStr;

    /**
     * 标本种类（对应 SpecimenType 枚举）
     */
    private String typeEnum;

    /**
     * 发布状态（对应 PublicationStatus 枚举）
     */
    private String statusEnum;
}