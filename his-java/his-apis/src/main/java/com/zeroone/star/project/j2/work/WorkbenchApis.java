package com.zeroone.star.project.j2.work;

import com.zeroone.star.project.query.j2.work.UserWorkbenchQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j2.work.UserWorkbenchVO;
import org.springframework.validation.annotation.Validated;

public interface WorkbenchApis {
    /**
     * 获取用户工作台
     * @param query 查询条件
     * @return 用户工作台数据
     */
     JsonVO<UserWorkbenchVO> getUserWorkbench(@Validated UserWorkbenchQuery query);
}
