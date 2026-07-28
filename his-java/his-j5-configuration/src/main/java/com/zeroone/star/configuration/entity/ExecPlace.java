package com.zeroone.star.configuration.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

import java.io.Serializable;
import java.time.LocalDateTime;

@Data
@TableName("his_exec_place")
public class ExecPlace implements Serializable {

    private static final long serialVersionUID = 1L;

    @TableId(value = "id", type = IdType.AUTO)
    private Long id;

    private String placeCode;

    private String placeName;

    private Integer sortOrder;

    private Integer activeFlag;

    private Integer isDeleted;

    private LocalDateTime createdAt;

    private LocalDateTime updatedAt;

    private Integer version;
}
