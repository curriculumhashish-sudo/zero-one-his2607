package com.zeroone.star.project.dto.j8.setroster;

import com.zeroone.star.project.vo.j8.setroster.AffectedSlotVO;
import com.zeroone.star.project.vo.j8.setroster.DisposalSuggestionVO;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

@Data
@Schema(description = "停诊预览信息")
public class StopClinicPreviewDTO {
    @Schema(description = "排班日程ID", example = "1")
    private Long scheduleId;

    @Schema(description = "停诊类型：FULL_DAY-整日停诊，PARTIAL-部分停诊", example = "FULL_DAY")
    private String stopType;

    @Schema(description = "受影响号源列表")
    private List<AffectedSlotVO> affectedSlots;

    @Schema(description = "处置建议列表")
    private List<DisposalSuggestionVO> disposalSuggestions;
}