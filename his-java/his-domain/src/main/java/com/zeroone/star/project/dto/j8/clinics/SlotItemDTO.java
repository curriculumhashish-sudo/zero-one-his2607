package com.zeroone.star.project.dto.j8.clinics;

import com.fasterxml.jackson.annotation.JsonFormat;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Min;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

import java.time.LocalDateTime;

/**
 * <p>
 * 描述：单条待放号时段明细
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 友人帐
 * @version 1.0.0
 */
@Data
@Schema(description = "单条待放号时段明细DTO")
public class SlotItemDTO {

    @NotNull(message = "时段开始时间不能为空")
    @Schema(description = "时段开始时间", example = "2026-07-22T00:19:38")
    @JsonFormat(pattern = "yyyy-MM-dd'T'HH:mm:ss")
    private LocalDateTime startTime;

    @NotNull(message = "时段结束时间不能为空")
    @Schema(description = "时段结束时间", example = "2026-07-22T01:04:38")
    @JsonFormat(pattern = "yyyy-MM-dd'T'HH:mm:ss")
    private LocalDateTime endTime;

    @NotNull(message = "放号容量不能为空")
    @Min(value = 1, message = "单时段放号容量最小为1")
    @Schema(description = "单时段放号容量", example = "10")
    private Integer capacity;
}
