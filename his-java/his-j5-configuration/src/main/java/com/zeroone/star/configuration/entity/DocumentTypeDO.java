package com.zeroone.star.configuration.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * <p>
 * 描述：文书类型实体
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zeroone
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@TableName("emr_document_type")
public class DocumentTypeDO extends BaseEntity {

    /**
     * 文书类型编码
     */
    private String code;

    /**
     * 文书类型名称
     */
    private String name;

    /**
     * 默认模板ID
     */
    private String defaultTemplateId;

    /**
     * 是否启用
     */
    private Integer isEnabled;

    /**
     * 排序号
     */
    private Integer sortOrder;
}
