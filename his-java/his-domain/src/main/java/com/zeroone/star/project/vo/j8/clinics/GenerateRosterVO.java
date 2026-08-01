package com.zeroone.star.project.vo.j8.clinics;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
import java.util.List;

@Data
@Schema(description = "生成排班响应")
public class GenerateRosterVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "生成的日程ID列表")
    private List<String> scheduleIds;

    @Schema(description = "生成数量")
    private Integer generatedCount;

    @Schema(description = "跳过数量")
    private Integer skippedCount;

    @Schema(description = "放号数量")
    private Integer provisionedSlotCount;
}
