package com.zeroone.star.j3.register.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 医疗服务表（挂号类型即门诊类医疗服务）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author j3小组
 * @version 1.0.0
 */
@Getter
@Setter
@ToString
@TableName("adm_healthcare_service")
public class HealthcareService implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键（VARCHAR64，雪花ID字符串）
     */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /**
     * 服务名称（挂号类型名称）
     */
    private String name;

    /**
     * 服务类型编码（区分普通门诊、专家门诊等）
     */
    private String typeCode;

    /**
     * 服务类别编码（挂号类型统一标记）
     */
    private String categoryCode;

    /**
     * 服务状态码
     */
    private String statusEnum;

    /**
     * 是否激活 1启用 0禁用
     */
    private Integer activeFlag;

    /**
     * 医保编号
     */
    private String ybNo;

    /**
     * 业务编号
     */
    private String busNo;

    /**
     * 创建时间
     */
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     */
    private LocalDateTime updatedAt;

    /**
     * 软删标记
     */
    @TableLogic(value = "0", delval = "1")
    private Integer isDeleted;
}
