package com.zeroone.star.project.vo.j1.login;

import com.fasterxml.jackson.annotation.JsonFormat;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.time.LocalDateTime;

@Data
@Schema(description = "租户/医院选择结果")
public class SelectResultVO {

    @Schema(description = "ID", example = "1")
    private Long id;

    @Schema(description = "名称", example = "默认租户")
    private String name;

    @Schema(description = "编码", example = "DEFAULT")
    private String code;

    @Schema(description = "描述", example = "系统默认租户，用于单租户或主租户场景")
    private String description;

    @Schema(description = "显示名称（医院场景）", example = "XX市人民医院")
    private String displayName;

    @Schema(description = "是否启用", example = "1")
    private Integer isActive;

    @Schema(description = "扩展属性")
    private String extraProperties;

    @Schema(description = "创建时间", example = "2025-01-01 12:00:00")
    @JsonFormat(pattern = "yyyy-MM-dd HH:mm:ss")
    private LocalDateTime createdAt;

    @Schema(description = "更新时间", example = "2025-01-01 12:00:00")
    @JsonFormat(pattern = "yyyy-MM-dd HH:mm:ss")
    private LocalDateTime updatedAt;

    @Schema(description = "创建人ID", example = "1")
    private Long createdBy;

    @Schema(description = "更新人ID", example = "1")
    private Long updatedBy;
}