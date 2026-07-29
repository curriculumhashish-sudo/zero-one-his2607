package com.zeroone.star.basedata.entity.diagnosis;

import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

import java.io.Serializable;

/**
 * 诊断目录编码概念实体，对应 ter_concept 表。
 */
@Data
@TableName("ter_concept")
public class TerConceptDO implements Serializable {

    private static final long serialVersionUID = 1L;

    @TableId
    private String id;

    private String codeSystemId;

    private String code;

    private String display;

    private Long sortOrder;

    private Boolean isActive;

    @TableLogic
    private Boolean isDeleted;
}
