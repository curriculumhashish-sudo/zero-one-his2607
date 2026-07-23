package com.zeroone.star.sysmgr.convertor;

import com.zeroone.star.project.vo.menu.MenuVO;
import com.zeroone.star.sysmgr.entity.menu.MenuDO;
import org.mapstruct.Mapper;
import org.mapstruct.Mapping;
import org.mapstruct.Mappings;
import org.mapstruct.NullValueCheckStrategy;
import org.mapstruct.factory.Mappers;

/**
 * 菜单VO、DO实体转换器
 * <p>NullValueCheckStrategy.ALWAYS 给每个映射字段加上非空判断</p>
 * @author 是丫丫
 * @date 2026/7/23
 */
@Mapper(nullValueCheckStrategy = NullValueCheckStrategy.ALWAYS)
public interface MenuConvertor {

    public MenuConvertor INSTANCE = Mappers.getMapper(MenuConvertor.class);

    /**
     * 将menuDO转换成menuVO
     * @author 是丫丫
     * @date 2026/7/23
     * @param menuDO DO
     * @return VO
     */
    @Mappings({
        @Mapping(source = "id", target = "menuId"),
        @Mapping(source = "name", target = "menuName"),
        @Mapping(source = "displayName", target = "showName"),
        @Mapping(source = "status", target = "isEnabled"),
        @Mapping(source = "sortNum", target = "sortOrder")
    })
    public MenuVO toVO(MenuDO menuDO);

    /**
     * 将menuVO转换成menuDO
     * @author 是丫丫
     * @date 2026/7/23
     * @param menuVO VO
     * @return DO
     */
    @Mappings({
            @Mapping(source = "menuId", target = "id"),
            @Mapping(source = "menuName", target = "name"),
            @Mapping(source = "showName", target = "displayName"),
            @Mapping(source = "isEnabled", target = "status"),
            @Mapping(source = "sortOrder", target = "sortNum")
    })
    public MenuDO toDO(MenuVO menuVO);
}
