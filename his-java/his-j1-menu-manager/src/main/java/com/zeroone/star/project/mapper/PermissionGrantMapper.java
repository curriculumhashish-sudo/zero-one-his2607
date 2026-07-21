package com.zeroone.star.project.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.project.entity.WhalePermissionGrant;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import java.util.List;

/**
 * 权限授权 Mapper
 * @author CatSN
 */
@Mapper
public interface PermissionGrantMapper extends BaseMapper<WhalePermissionGrant> {

    /**
     * 物理删除指定角色的所有权限授权
     * @param providerKey 角色ID（字符串形式）
     * @return 删除行数
     */
    int deleteByProviderKey(@Param("providerKey") String providerKey);

    /**
     * 根据菜单ID集合查询对应的权限名称（去重）
     * @param menuIds 菜单ID集合（字符串形式）
     * @return 权限名称集合
     */
    List<String> selectPermissionNamesByMenuIds(@Param("menuIds") List<String> menuIds);

    /**
     * 批量插入权限授权
     * @param grants 授权记录列表
     * @return 插入行数
     */
    int batchInsertGrants(@Param("grants") List<WhalePermissionGrant> grants);
}
