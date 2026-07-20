package com.zeroone.star.project.dto.j2.basedata.bodystructure;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

/**
 * <p>
 * 描述：修改部位传输数据
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 雾止洲
 * @version 1.0.0
 */
@Data
@Schema(description = "修改部位上传数据对象")
public class BodyStructureUpdateDTO {

    @NotBlank(message = "部位ID不能为空")
    @Schema(description = "部位主键", example = "322663105141149696", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @NotBlank(message = "部位名称不能为空")
    @Schema(description = "部位名称", example = "头部", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "上级部位主键；为空时表示根节点", example = "322663105141149695")
    private String parentId;

    @Schema(description = "拼音码", example = "TB")
    private String pyStr;

    @Schema(description = "五笔码", example = "UK")
    private String wbStr;

    @NotNull(message = "版本号不能为空")
    @Schema(description = "乐观锁版本号", example = "0", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer version;
}
