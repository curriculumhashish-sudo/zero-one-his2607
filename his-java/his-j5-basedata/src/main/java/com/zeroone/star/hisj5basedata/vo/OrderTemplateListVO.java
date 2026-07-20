package com.zeroone.star.hisj5basedata.vo;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.time.LocalDateTime;

/**
 * <p>
 * 组套模板列表项（接口 2 响应）
 * </p>
 * @author keepasdd
 */
@Data
public class OrderTemplateListVO {

    @Schema(description = "组套模板 ID")
    private String id;

    @Schema(description = "模板名称")
    private String name;

    @Schema(description = "模板编码")
    private String templateCode;

    @Schema(description = "绑定类型")
    private String bindingEnum;

    @Schema(description = "适用场景")
    private String sceneEnum;

    @Schema(description = "组套种类")
    private String templateKindEnum;

    @Schema(description = "状态")
    private String statusEnum;

    @Schema(description = "排序权重")
    private Integer sortWeight;

    @Schema(description = "创建时间")
    private LocalDateTime createdAt;

    @Schema(description = "更新时间")
    private LocalDateTime updatedAt;
}
