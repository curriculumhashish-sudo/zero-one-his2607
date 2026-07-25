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
    @Select("<script>" +
            "WITH RECURSIVE authorized_menus AS ( " +
            "  SELECT DISTINCT m.* " +
            "  FROM whale_menus m " +
            "  INNER JOIN whale_permissions p ON m.permission_name = p.name AND p.is_deleted = 0 " +
            "  INNER JOIN whale_permission_grants pg ON pg.permission_name = p.name " +
            "    AND pg.provider_name = 'R' " +
            "    AND pg.is_deleted = 0 " +
            "  INNER JOIN whale_roles r ON r.id = pg.provider_key " +
            "    AND r.is_deleted = 0 " +
            "  WHERE m.is_deleted = 0 " +
            "    AND m.is_enabled = 1 " +
            "    AND (r.id IN " +
            "      <foreach collection='roles' item='role' open='(' separator=',' close=')'>#{role}</foreach> " +
            "      OR r.code IN " +
            "      <foreach collection='roles' item='role' open='(' separator=',' close=')'>#{role}</foreach> " +
            "      OR r.name IN " +
            "      <foreach collection='roles' item='role' open='(' separator=',' close=')'>#{role}</foreach> " +
            "    ) " +
            "  UNION DISTINCT " +
            "  SELECT parent.* " +
            "  FROM whale_menus parent " +
            "  INNER JOIN authorized_menus child ON child.parent_id = parent.id " +
            "  WHERE parent.is_deleted = 0 " +
            "    AND parent.is_enabled = 1 " +
            ") " +
            "SELECT DISTINCT id, code, name, display_name, icon, path, component, permission_name, " +
            "parent_id, sort_order, is_enabled, is_hidden, is_external, external_url, menu_type, " +
            "remark, tenant_id, created_at, updated_at, created_by, updated_by, keep_alive " +
            "FROM authorized_menus " +
            "ORDER BY sort_order, id" +
            "</script>")
    List<MenuDO> selectMenusByRoles(@Param("roles") List<String> roles);

}


