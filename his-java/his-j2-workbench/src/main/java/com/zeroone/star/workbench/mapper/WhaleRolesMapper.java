package com.zeroone.star.workbench.mapper;

import com.baomidou.mybatisplus.core.conditions.Wrapper;
import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.project.vo.j2.workbench.UserWorkbenchVO;
import com.zeroone.star.workbench.entity.WhaleRoles;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.session.ResultHandler;

import java.io.Serializable;
import java.util.Collection;
import java.util.List;
import java.util.Map;

/**
 * <p>
 * 系统角色 Mapper 接口
 * </p>
 *
 * @author 鹿
 * @since 2026-07-24
 */
public interface WhaleRolesMapper extends BaseMapper<WhaleRoles> {
    /**
     * 查询用户工作台菜单（按角色分组，支持关键字模糊搜索）
     * @param tenantId 租户ID
     * @param userId   用户ID
     * @param keyword  菜单名称模糊搜索关键字（可选）
     * @return 按角色分组的工作台菜单列表
     */
    List<UserWorkbenchVO> selectUserWorkbench(@Param("tenantId") String tenantId,
                                              @Param("userId") String userId,
                                              @Param("keyword") String keyword);

    @Override
    int insert(WhaleRoles entity);

    @Override
    default int deleteById(Serializable id) {
        return BaseMapper.super.deleteById(id);
    }

    @Override
    default int deleteById(Object obj, boolean useFill) {
        return BaseMapper.super.deleteById(obj, useFill);
    }

    @Override
    int deleteById(WhaleRoles entity);

    @Override
    int delete(Wrapper<WhaleRoles> queryWrapper);

    @Override
    int updateById(WhaleRoles entity);

    @Override
    int update(WhaleRoles entity, Wrapper<WhaleRoles> updateWrapper);

    @Override
    WhaleRoles selectById(Serializable id);

    @Override
    List<WhaleRoles> selectByIds(Collection<? extends Serializable> idList);

    @Override
    void selectByIds(Collection<? extends Serializable> idList, ResultHandler<WhaleRoles> resultHandler);

    @Override
    Long selectCount(Wrapper<WhaleRoles> queryWrapper);

    @Override
    List<WhaleRoles> selectList(Wrapper<WhaleRoles> queryWrapper);

    @Override
    void selectList(Wrapper<WhaleRoles> queryWrapper, ResultHandler<WhaleRoles> resultHandler);

    @Override
    List<WhaleRoles> selectList(IPage<WhaleRoles> page, Wrapper<WhaleRoles> queryWrapper);

    @Override
    void selectList(IPage<WhaleRoles> page, Wrapper<WhaleRoles> queryWrapper, ResultHandler<WhaleRoles> resultHandler);

    @Override
    List<Map<String, Object>> selectMaps(Wrapper<WhaleRoles> queryWrapper);

    @Override
    void selectMaps(Wrapper<WhaleRoles> queryWrapper, ResultHandler<Map<String, Object>> resultHandler);

    @Override
    List<Map<String, Object>> selectMaps(IPage<? extends Map<String, Object>> page, Wrapper<WhaleRoles> queryWrapper);

    @Override
    void selectMaps(IPage<? extends Map<String, Object>> page, Wrapper<WhaleRoles> queryWrapper, ResultHandler<Map<String, Object>> resultHandler);

    @Override
    <E> List<E> selectObjs(Wrapper<WhaleRoles> queryWrapper);

    @Override
    <E> void selectObjs(Wrapper<WhaleRoles> queryWrapper, ResultHandler<E> resultHandler);
}
