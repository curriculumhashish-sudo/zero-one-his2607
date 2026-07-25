package com.zeroone.star.workbench.service.impl;


import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.project.query.j2.workbench.UserWorkbenchQuery;
import com.zeroone.star.project.vo.j2.workbench.UserWorkbenchVO;
import com.zeroone.star.workbench.entity.WhaleMenus;
import com.zeroone.star.workbench.mapper.WhaleMenusMapper;
import com.zeroone.star.workbench.mapper.WhaleRolesMapper;
import com.zeroone.star.workbench.service.IWorkbenchService;
import jakarta.annotation.Resource;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;

import java.util.Collections;
import java.util.List;

/**
 * <p>
 * 工作台业务服务实现类
 * </p>
 *
 * @author 鹿
 * @since 2026-07-24
 */
@Slf4j
@Service
public class WorkbenchServiceImpl extends ServiceImpl<WhaleMenusMapper, WhaleMenus> implements IWorkbenchService {

    @Resource
    private WhaleRolesMapper whaleRolesMapper;

    @Override
    public List<UserWorkbenchVO> queryUserWorkbench(UserWorkbenchQuery query) {
        List<UserWorkbenchVO> result = whaleRolesMapper.selectUserWorkbench(
                query.getTenantId(),
                query.getUserId(),
                query.getKeyword()
        );
        if (result == null || result.isEmpty()) {
            log.info("用户[{}]在租户[{}]下未查询到工作台菜单数据", query.getUserId(), query.getTenantId());
            return Collections.emptyList();
        }
        return result;
    }
}
