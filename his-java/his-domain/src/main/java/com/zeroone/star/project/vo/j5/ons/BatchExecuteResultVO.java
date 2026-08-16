package com.zeroone.star.project.vo.j5.ons;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

@Data
@Schema(description = "批量执行结果")
public class BatchExecuteResultVO {

    @Schema(description = "总记录数", example = "5")
    private Integer totalCount;

    @Schema(description = "成功数量", example = "3")
    private Integer successCount;

    @Schema(description = "失败数量", example = "2")
    private Integer failedCount;

    @Schema(description = "是否部分成功", example = "true")
    private Boolean hasPartialSuccess;

    @Schema(description = "成功的记录ID列表")
    private List<String> successIds;

    @Schema(description = "失败详情列表")
    private List<FailedItemVO> failedItems;

    @Schema(description = "幂等性Key（用于防止重复提交）", example = "uuid-string")
    private String idempotencyKey;

    @Schema(description = "执行时间", example = "2024-01-15 10:30:00")
    private String executeTime;

    @Data
    @Schema(description = "失败项详情")
    public static class FailedItemVO {

        @Schema(description = "记录ID", example = "1")
        private String recordId;

        @Schema(description = "失败原因", example = "记录不存在或状态不允许执行")
        private String reason;

        @Schema(description = "错误编码", example = "NOT_FOUND")
        private String errorCode;
    }
}
