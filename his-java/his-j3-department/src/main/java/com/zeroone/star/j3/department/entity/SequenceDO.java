package com.zeroone.star.j3.department.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.time.LocalDateTime;

@Data
@AllArgsConstructor
@NoArgsConstructor
@Builder
@Schema(description = "业务序号表，按 key 记录当前序号", name = "Sequence")
@TableName(value = "whale_sequence")
public class SequenceDO {
    @Schema(description = "序号键（如 ORDER、PATIENT、或 ORDER:2025-01-30 表示按天）", example = "1:OR")
    private String seqKey;

    @Schema(description = "当前序号值（递增后返回）", example = "1")
    private Long currentValue;

    @Schema(description = "最后更新时间", example = "2026-07-24 16:00:48.908868")
    private LocalDateTime updatedAt;
}
