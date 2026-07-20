package com.zeroone.star.hisj5basedata.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 组套模板头（映射 cus_order_template）
 * </p>
 * @author keepasdd
 */
@AllArgsConstructor
@Data
@TableName("cus_order_template")
public class OrderTemplate implements Serializable {

    private static final long serialVersionUID = 1L;


    //主键 ID
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;


    //租户标识
    private String tenantId;


    //创建时间
    private LocalDateTime createdAt;


    //最后更新时间
    private LocalDateTime updatedAt;


    //创始人
    private String createdBy;


    //最后更新人
    private String updatedBy;


    //逻辑删除标志
    private Boolean isDeleted;


    //删除时间
    private LocalDateTime deletedAt;

    //删除人
    private String deletedBy;


    //版本
    private Integer version;


    private String objectId;


    private String bindingEnum;


    //适用场景枚举
    private String sceneEnum;


    private String templateCode;


    //名称
    private String name;


    //描述
    private String description;


    //排序权重
    private Integer sortWeight;

    //状态
    private String statusEnum;


    //备注
    private String remark;

    //组套种类枚举
    private String templateKindEnum;


    private BigDecimal defaultChineseHerbsDoseQuantity;

    /**
     * TCM 组套默认代煎标志
     */
    private Boolean defaultSufferingFlag;

    //模板来源枚举
    private String templateSourceEnum;
}
