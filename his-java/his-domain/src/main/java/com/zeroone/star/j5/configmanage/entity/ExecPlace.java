package com.zeroone.star.j5.configmanage.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：执行地点实体
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Schema(description = "执行地点实体")
@Getter
@Setter
@TableName("exec_place")
public class ExecPlace implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键
     */
    @Schema(description = "主键", example = "1")
    @TableId(value = "id", type = IdType.AUTO)
    private Long id;

    /**
     * 执行地点编码
     */
    @Schema(description = "执行地点编码", example = "EP001")
    private String placeCode;

    /**
     * 执行地点名称
     */
    @Schema(description = "执行地点名称", example = "门诊注射室")
    private String placeName;

    /**
     * 排序号
     */
    @Schema(description = "排序号", example = "1")
    private Integer sortOrder;

    /**
     * 是否启用 0-禁用 1-启用
     */
    @Schema(description = "是否启用 0-禁用 1-启用", example = "1")
    private Integer activeFlag;

    /**
     * 创建时间
     */
    @Schema(description = "创建时间", example = "2025-01-01 00:00:00")
    private LocalDateTime createdAt;

    /**
     * 更新时间
     */
    @Schema(description = "更新时间", example = "2025-01-01 00:00:00")
    private LocalDateTime updatedAt;

    /**
     * 创建人
     */
    @Schema(description = "创建人", example = "admin")
    private String createdBy;

    /**
     * 更新人
     */
    @Schema(description = "更新人", example = "admin")
    private String updatedBy;

    /**
     * 是否删除 0-未删除 1-已删除
     */
    @Schema(description = "是否删除 0-未删除 1-已删除", example = "0")
    private Integer isDeleted;

    /**
     * 乐观锁版本号
     */
    @Schema(description = "乐观锁版本号", example = "0")
    private Integer version;
}
