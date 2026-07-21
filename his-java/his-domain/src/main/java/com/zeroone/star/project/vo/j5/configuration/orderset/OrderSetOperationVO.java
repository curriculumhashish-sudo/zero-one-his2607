package com.zeroone.star.project.vo.j5.configuration.orderset;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * 组套模板写操作结果。
 */
@Data
@Builder
@NoArgsConstructor
@AllArgsConstructor
@Schema(description = "组套模板写操作结果")
public class OrderSetOperationVO {

    @Schema(description = "组套模板编号")
    private String id;

    @Schema(description = "操作后的状态", example = "DRAFT")
    private String status;

    @Schema(description = "是否已经发布")
    private Boolean published;

    @Schema(description = "补充说明")
    private String message;
}
