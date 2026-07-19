package com.zeroone.star.project.dto.j4.diagnosis;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * <p>
 * 描述：身体结构节点增/改传输数据对象，对应 dia_body_structure 表
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Schema(description = "身体结构节点增改数据对象")
@Data
@AllArgsConstructor
@NoArgsConstructor
public class BodyStructureDTO {

    /**
     * 主键（编辑时必填，新增时不填）
     */
    @Schema(description = "身体结构主键（编辑时必填）", example = "328090067094999040")
    private String id;

    /**
     * 业务编号
     */
    @Schema(description = "业务编号", example = "BS0001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;

    /**
     * 节点名称
     */
    @Schema(description = "节点名称", example = "头", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    /**
     * 父节点主键（为空表示根节点）
     */
    @Schema(description = "父节点主键（为空表示根节点）", example = "328090067094999040")
    private String parentId;

    /**
     * 状态枚举：ACTIVE / INACTIVE
     */
    @Schema(description = "状态枚举：ACTIVE / INACTIVE", example = "ACTIVE", requiredMode = Schema.RequiredMode.REQUIRED)
    private String statusEnum;
}
