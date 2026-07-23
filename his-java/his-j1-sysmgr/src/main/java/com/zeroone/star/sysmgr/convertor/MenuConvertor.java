package com.zeroone.star.sysmgr.convertor;

import com.zeroone.star.project.vo.menu.MenuVO;
import com.zeroone.star.sysmgr.entity.menu.MenuDO;
import org.mapstruct.Mapper;
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
    })
    public MenuDO toDO(MenuVO menuVO);
}
