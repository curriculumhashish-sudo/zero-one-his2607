package com.zeroone.star.workbench.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.project.doo.j2.workbench.WhaleRoles;
import com.zeroone.star.project.vo.j2.workbench.UserWorkbenchVO;
import org.apache.ibatis.annotations.Param;

import java.util.List;

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

}
