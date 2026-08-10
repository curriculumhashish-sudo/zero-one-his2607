package com.zeroone.star.project.vo.j7.dispense;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.ArrayList;
import java.util.List;

@Data
@Schema(description = "门诊批量发药返回结果")
public class BatchDispenseResultVO {

    @Schema(description = "成功发药条数")
    private Integer successCount;

    @Schema(description = "失败条数")
    private Integer failCount;

    @Schema(description = "成功的记录ID列表")
    private List<String> successIds = new ArrayList<>();

    @Schema(description = "失败的记录ID列表")
    private List<String> failIds = new ArrayList<>();

    @Schema(description = "操作结果失败详情消息（与外层 message 区分，避免字段重名）")
    private String failMessage;
}
