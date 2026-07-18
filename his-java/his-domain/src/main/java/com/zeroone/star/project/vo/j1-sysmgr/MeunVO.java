package com.zeroone.star.project.vo.j1.sysmgr;

import io.swagger.annotations.ApiModel;
import io.swagger.annotations.ApiModelProperty;
import lombok.Data;


@Data
@ApiModel("菜单管理分页树形")
public class MenuVO {

    @ApiModelProperty("序号")
    private Long sortNum;

    @ApiModelProperty("菜单英文名称")
    private String menuName;

    @ApiModelProperty("页面显示中文名称")
    private String showName;

    @ApiModelProperty("菜单图标")
    private String icon;

    @ApiModelProperty("前端路由路径")
    private String path;

    @ApiModelProperty("前端vue组件地址")
    private String component;

    @ApiModelProperty("排序号")
    private Integer sort;

    @ApiModelProperty("状态：0启用 1停用")
    private String status;

    @ApiModelProperty("菜单ID")
    private Long menuId;

    @ApiModelProperty("父菜单ID，0为一级目录")
    private Long parentId;

    @ApiModelProperty("菜单类型：C目录 M菜单 F按钮")
    private String menuType;
}