package com.zeroone.star.workbench.controller;

import com.zeroone.star.project.j2.work.WorkbenchApis;
import com.zeroone.star.project.query.j2.work.UserWorkbenchQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j2.work.UserWorkbenchVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@Tag(name = "工作台日常功能管理控制器")
public class WorkbenchController implements WorkbenchApis {
    @GetMapping("query-workbench")
    @Operation(summary = "获取工作台日常功能")
    @Override
    public JsonVO<UserWorkbenchVO> getUserWorkbench(UserWorkbenchQuery query) {
        return null;
    }
}
