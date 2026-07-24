package com.zeroone.star.project.vo.j2.workbench;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import java.io.Serializable;


/**
 * <p>
 * 描述: 工作台菜单节点VO（适配通用树形工具）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 鹿
 * @version 1.0.0
 */

@Data
@Schema(description = "工作台功能卡片")
//whale_menus
public class WorkbenchMenuVO implements Serializable {
    @Schema(description = "ID",example = "1000",requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;
    @Schema(description = "前端图标",example = "lucide:folder-tree")
    private String icon;
    @Schema(description = "名称",example = "项目管理")
    private String display_name;
    @Schema(description = "前端路由地址",example = "/system")
    private String path;
    @Schema(description = "权限标识")
    private String permission_name;
    @Schema(description = "排序号")
    private Integer sort_order;
}