package com.zeroone.star.project.vo.j2.workbench;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import java.io.Serializable;
import java.util.List;

/**
 * <p>
 * 描述: 用户工作台完整返回VO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 鹿
 * @version 1.0.0
 */
@Data
@Schema(description = "按角色分组工作台数据")
public class UserWorkbenchVO implements Serializable {
    //whale_roles
    @Schema(description = "角色名称：医生/护士/收费处/药房/管理员", example = "医生", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;
    @Schema(description = "当前角色拥有的全部功能菜单卡片")
    private List<WorkbenchMenuVO> menuList;
}