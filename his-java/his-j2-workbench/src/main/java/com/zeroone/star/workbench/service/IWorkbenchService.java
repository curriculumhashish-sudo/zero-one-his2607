package com.zeroone.star.workbench.service;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.project.query.j2.workbench.UserWorkbenchQuery;
import com.zeroone.star.project.vo.j2.workbench.UserWorkbenchVO;
import com.zeroone.star.project.vo.j2.workbench.WorkbenchMenuVO;

import java.util.List;

/**
 * <p>
 * 工作台业务服务接口
 * </p>
 *
 * @author 鹿
 * @since 2026-07-24
 */
public interface IWorkbenchService extends IService<UserWorkbenchVO> {

    /**
     * 查询用户工作台菜单数据
     * @param query 查询条件（租户ID、用户ID、可选科室ID）
     * @return 按角色分组的工作台菜单列表
     */
    List<UserWorkbenchVO> queryUserWorkbench(UserWorkbenchQuery query);
}