package com.zeroone.star.project.vo.j3.ward;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
import java.util.List;

/**
 * <p>
 * 描述：病区-病房-床位树形结构返回VO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zhujian
 * @version 1.0.0
 */
@Data
@Schema(description = "病区树形节点")
public class WardTreeVO implements Serializable {

    @Schema(description = "节点ID", example = "322663218269917184", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "节点名称", example = "内科病区", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "节点类型 ward-病区 room-病房 bed-床位", example = "ward")
    private String type;

    @Schema(description = "状态 0-停用 1-启用", example = "1")
    private Integer status;

    @Schema(description = "下级数量", example = "3")
    private Integer subCount;

    @Schema(description = "子节点列表")
    private List<WardTreeVO> children;
}
