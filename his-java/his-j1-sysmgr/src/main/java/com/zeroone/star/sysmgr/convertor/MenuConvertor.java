package com.zeroone.star.sysmgr.convertor;

import com.zeroone.star.project.dto.menu.MenuCreateDTO;
import com.zeroone.star.project.dto.menu.MenuUpdateDTO;
import com.zeroone.star.project.utils.tree.TreeUtils;
import com.zeroone.star.project.vo.login.MenuTreeVO;
import com.zeroone.star.project.vo.menu.MenuVO;
import com.zeroone.star.sysmgr.entity.menu.MenuDO;
import org.mapstruct.Mapper;
import org.mapstruct.Mapping;
import org.mapstruct.Mappings;
import org.mapstruct.NullValueCheckStrategy;
import org.mapstruct.factory.Mappers;

import java.awt.*;
import java.util.List;

/**
 * 菜单VO、DO实体转换器
 * <p>NullValueCheckStrategy.ALWAYS 给每个映射字段加上非空判断</p>
 * @author 是丫丫
 * @date 2026/7/23
 */
@Mapper(nullValueCheckStrategy = NullValueCheckStrategy.ALWAYS)
public interface MenuConvertor {

    public static MenuConvertor INSTANCE = Mappers.getMapper(MenuConvertor.class);

    /**
     * 将 MenuDO 转换成 MenuVO
     * @author 是丫丫
     * @date 2026/7/23
     * @param menuDO DO
     * @return VO
     */
    @Mappings({
        @Mapping(source = "id", target = "menuId"),
        @Mapping(source = "name", target = "menuName"),
        @Mapping(source = "displayName", target = "showName"),
        @Mapping(source = "isEnabled", target = "status"),
        @Mapping(source = "sortOrder", target = "sortNum"),
        @Mapping(target = "menuType", expression = "java(menuDO.getMenuType().name())")
    })
    public MenuVO toVO(MenuDO menuDO);

    /**
     * 将 MenuDO集合 转换成 MenuVO集合
     * @author 是丫丫
     * @date 2026/7/23
     * @param list DO集合
     * @return VO
     */
    @Mappings({})
    public List<MenuVO> toListVO(List<MenuDO> list);

    /**
     * 将 MenuCreateDTO 转换成 MenuDO
     * @author 是丫丫
     * @date 2026/7/23
     * @param dto DTO
     * @return DO
     */
    @Mappings({
        @Mapping(target = "keepAlive", expression = "java(dto.getKeepAlive() ? 1 : 0)"),
        @Mapping(target = "isExternal", expression = "java(dto.getExternal() ? 1 : 0)"),
        @Mapping(target = "isHidden", expression = "java(dto.getHidden() ? 1 : 0)"),
        @Mapping(target = "isEnabled", expression = "java(dto.getEnabled() ? 1 : 0)"),
        @Mapping(target = "parentId", expression = "java(org.apache.commons.lang3.StringUtils." +
                "isNoneBlank(dto.getParentId()) ? dto.getParentId() : null)")
    })
    public MenuDO createDO(MenuCreateDTO dto);

    /**
     * 将 MenuUpdateDTO 转换成 MenuDO
     * @author 是丫丫
     * @date 2026/7/23
     * @param dto DTO
     * @return DO
     */
    default MenuDO updateDO(MenuUpdateDTO dto) {
        MenuDO menuDO = createDO(dto);
        menuDO.setId(dto.getId());
        return menuDO;
    }

    /**
     * 将 MenuDO 转换成 MenuTreeVO
     * @param menuDO DO
     * @return MenuTreeVO
     */
    @Mappings({
        @Mapping(source = "id", target = "tnId"),
        @Mapping(source = "parentId", target = "tnPid"),
        @Mapping(source = "id", target = "id"),
        @Mapping(source = "displayName", target = "text"),
        @Mapping(source = "path", target = "href"),
        @Mapping(source = "parentId", target = "pid"),
    })
    public MenuTreeVO toTreeVO(MenuDO menuDO);

    /**
     * 将 List<MenuDO> 转换成 List<MenuTreeVO>
     * @param list ListDO
     * @return List<MenuTreeVO>
     */
    default List<MenuTreeVO> toListTreeVO(List<MenuDO> list) {
        if (list == null || list.isEmpty()) {
            return List.of();
        }
        return TreeUtils.listToTree(list, MenuConvertor.INSTANCE::toTreeVO);
    }
}
