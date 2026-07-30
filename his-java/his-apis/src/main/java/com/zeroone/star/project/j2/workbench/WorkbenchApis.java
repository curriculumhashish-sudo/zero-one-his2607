package com.zeroone.star.project.j2.workbench;

import com.zeroone.star.project.query.j2.workbench.UserWorkbenchQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j2.workbench.UserWorkbenchVO;
import org.springframework.validation.annotation.Validated;

import java.util.List;

public interface WorkbenchApis {
    /**
     * 获取用户工作台
     * @param query 查询条件
     * @return 用户工作台数据
     */
    JsonVO<List<UserWorkbenchVO>> getUserWorkbench(@Validated UserWorkbenchQuery query);
}
