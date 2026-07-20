package com.zeroone.star.project.vo.j2.work;
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
    @Schema(description = "ID")
    private String id;
    @Schema(description = "前端图标")
    private String icon;
    @Schema(description = "名称")
    private String menuName;
    @Schema(description = "前端路由地址")
    private String routePath;
    @Schema(description = "权限标识，接口鉴权使用")
    private String permissionKey;
    @Schema(description = "排序号")
    private Integer sort;
}