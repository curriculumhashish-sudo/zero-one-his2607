package com.zeroone.star.project.vo.j1.permission;

import com.zeroone.star.project.utils.tree.TreeNode;
import lombok.Data;
import lombok.EqualsAndHashCode;
import java.util.List;

@Data
@EqualsAndHashCode(callSuper = true)
public class PermissionTreeVO extends TreeNode {
    /** 权限标识 → 映射 tnId，TreeUtils.listToTree() 用它做父子关联 */
    public String getName() { return tnId; }

    private String displayName;

    /** 子节点列表 */
    public List<PermissionTreeVO> getChildren() { return childrenElementTrans(); }
}