package com.zeroone.star.project.vo.j4.diagnosis;

import com.zeroone.star.project.utils.tree.TreeNode;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

import java.util.List;

/**
 * <p>
 * 描述：身体结构树节点视图对象，用于构建身体结构目录树
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@EqualsAndHashCode(callSuper = true)
@Data
@Schema(description = "身体结构树节点视图对象")
public class BodyStructureTreeNodeVO extends TreeNode {

    /**
     * 身体结构主键
     */
    @Schema(description = "身体结构主键", example = "328090067094999040")
    private String id;

    /**
     * 业务编号
     */
    @Schema(description = "业务编号", example = "BS0001")
    private String busNo;

    /**
     * 节点名称
     */
    @Schema(description = "节点名称", example = "头")
    private String name;

    /**
     * 状态枚举：ACTIVE / INACTIVE
     */
    @Schema(description = "状态枚举：ACTIVE / INACTIVE", example = "ACTIVE")
    private String statusEnum;

    /**
     * 获取子节点列表
     */
    @Schema(description = "子节点列表")
    public List<BodyStructureTreeNodeVO> getChildren() {
        return childrenElementTrans();
    }
}
