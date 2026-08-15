package com.zeroone.star.sysmgr.mapper.menu;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.sysmgr.entity.menu.MenuDO;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import java.util.List;

/**
 * 菜单数据访问层。
 */
@Mapper
public interface MenuMapper extends BaseMapper<MenuDO> {

    /**
     * 根据角色列表查询已启用的可见菜单（含父菜单补齐）。
     */
    List<MenuDO> selectMenusByRoles(@Param("roles") List<String> roles);
}
