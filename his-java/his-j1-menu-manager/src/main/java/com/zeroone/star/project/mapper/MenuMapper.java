package com.zeroone.star.project.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.project.entity.WhaleMenu;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import java.util.List;

/**
 * 菜单 Mapper
 * @author CatSN
 */
@Mapper
public interface MenuMapper extends BaseMapper<WhaleMenu> {

    /**
     * 根据用户ID查询可见菜单列表
     * @param userId 用户ID
     * @return 菜单列表
     */
    List<WhaleMenu> selectMenusByUserId(@Param("userId") String userId);
}
