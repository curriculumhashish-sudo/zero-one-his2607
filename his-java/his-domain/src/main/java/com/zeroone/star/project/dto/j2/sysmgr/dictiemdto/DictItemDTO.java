package com.zeroone.star.project.dto.j2.sysmgr.dictiemdto;


import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：用户数据传输对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
//补一下，创建时间什么的
@Data
@Schema(description = "字典编码数据传输对象")
public class DictItemDTO extends DictItemAddDTO {
    @Schema(description = "字典序号", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String dicId;

}
