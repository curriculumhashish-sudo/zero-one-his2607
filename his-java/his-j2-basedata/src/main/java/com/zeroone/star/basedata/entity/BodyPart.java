package com.zeroone.star.basedata.entity;

import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.TableName;
import com.baomidou.mybatisplus.annotation.Version;
import lombok.Getter;
import lombok.Setter;

import java.time.LocalDateTime;

/**
 * 诊断身体结构（部位）实体。
 */
@Getter
@Setter
@TableName("dia_body_structure")
public class BodyPart {

    @TableId("id")
    private String id;

    private String name;

    private String parentId;

    private String pyStr;

    private String wbStr;

    private String statusEnum;

    @Version
    private Integer version;

    @TableLogic(value = "0", delval = "1")
    private Integer isDeleted;

    private LocalDateTime updatedAt;

    private LocalDateTime deletedAt;
}
