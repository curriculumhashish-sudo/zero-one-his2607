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
    @Schema(description = "菜单ID", example = "wb_op_doctor", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "菜单编码", example = "outpatient_doctor")
    private String code;

    @Schema(description = "菜单名称（英文标识）", example = "OutpatientDoctor")
    private String name;

    @Schema(description = "显示名称", example = "门诊医生站")
    private String displayName;

    @Schema(description = "前端图标", example = "lucide:stethoscope")
    private String icon;

    @Schema(description = "前端路由地址", example = "/outpatient/doctor")
    private String path;

    @Schema(description = "前端组件路径", example = "outpatient/doctor/index")
    private String component;

    @Schema(description = "权限标识", example = "outpatient:doctor:view")
    private String permissionName;

    @Schema(description = "排序号", example = "1")
    private Integer sortOrder;
}