package com.zeroone.star.hisj5basedata.vo;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 组套名称下拉项（接口 1 响应）
 * </p>
 * @author keepasdd
 */
@Data
public class OrderTemplateNameVO {

    @Schema(description = "组套模板 ID")
    private String id;

    @Schema(description = "组套名称")
    private String name;

    @Schema(description = "绑定类型（前端按此字段分三栏）")
    private String bindingEnum;

    @Schema(description = "组套种类")
    private String templateKindEnum;

    @Schema(description = "适用场景")
    private String sceneEnum;
}
