package com.zeroone.star.project.dto.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;

/**
 * <p>
 * 描述：放号时段模板列表数据
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author Gin
 * @version 1.0.0
 */
@Data
@Schema(description = "放号时段模板列表数据")
public class SlotProvisionTemplateDTO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "放号时段模板主键ID", example = "tpl-001")
    private String id;

    @Schema(description = "模板编码", example = "SPT000001")
    private String templateCode;

    @Schema(description = "模板名称", example = "门诊上午时段")
    private String templateName;

    @Schema(description = "机构/科室ID，空表示全院", example = "dept-001")
    private String organizationId;

    @Schema(description = "是否启用", example = "true")
    private Boolean enabled;

    @Schema(description = "创建时间", example = "2025-01-15 10:30:00")
    private String createdAt;

    @Schema(description = "最后更新时间", example = "2025-06-20 14:00:00")
    private String updatedAt;
}
