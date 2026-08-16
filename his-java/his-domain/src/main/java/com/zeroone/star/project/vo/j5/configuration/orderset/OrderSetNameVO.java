package com.zeroone.star.project.vo.j5.configuration.orderset;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * 组套名称选项VO
 */
@Data
@Schema(description = "组套名称选项VO")
public class OrderSetNameVO {

    @Schema(description = "组套模板ID", example = "329888224518344704")
    private String id;

    @Schema(description = "模板名称", example = "普通感冒组套")
    private String name;

    @Schema(description = "模板编码", example = "OT-OUT-001")
    private String templateCode;
}
