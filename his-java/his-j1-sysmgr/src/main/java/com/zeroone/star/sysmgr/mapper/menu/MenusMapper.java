package com.zeroone.star.sysmgr.mapper.menu;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.sysmgr.entity.menu.MenuDO;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;

import java.util.List;

/**
* @author xin
* @description 针对表【whale_menus(菜单)】的数据库操作Mapper
* @createDate 2026-07-23 22:43:12
* @Entity com.zeroone.star.sysmgr.WhaleMenus
*/
public interface MenusMapper extends BaseMapper<MenuDO> {

    /**
     * 根据角色列表查询授权菜单，并补齐授权菜单的父目录。
     * roles 支持角色ID、角色编码或角色名称。
     */
    List<MenuDO> selectMenusByRoles(@Param("roles") List<String> roles);

}


