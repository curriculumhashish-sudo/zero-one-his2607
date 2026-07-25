package com.zeroone.star.workbench.controller;

import com.zeroone.star.project.j2.workbench.WorkbenchApis;
import com.zeroone.star.project.query.j2.workbench.UserWorkbenchQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j2.workbench.UserWorkbenchVO;
import com.zeroone.star.workbench.service.WorkbenchService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import lombok.extern.slf4j.Slf4j;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

import static com.baomidou.mybatisplus.extension.ddl.DdlScriptErrorHandler.PrintlnLogErrorHandler.log;

/**
 * <p>
 * 工作台日常功能管理控制器
 * </p>
 *
 * @author 鹿
 * @since 2026-07-24
 */
@Slf4j
@RestController
@Tag(name = "工作台日常功能管理控制器")
public class WorkbenchController implements WorkbenchApis {

    @Resource
    private WorkbenchService workbenchService;

    @GetMapping("query-workbench")
    @Operation(summary = "获取工作台日常功能")
    @Override
    public JsonVO<List<UserWorkbenchVO>> getUserWorkbench(UserWorkbenchQuery query) {
        try {
            List<UserWorkbenchVO> result = workbenchService.queryUserWorkbench(query);
            return JsonVO.success(result);
        } catch (Exception e) {
            log.error("获取工作台日常功能异常, userId={}, tenantId={}", query.getUserId(), query.getTenantId(), e);
            return JsonVO.fail("获取工作台数据失败，请稍后重试");
        }
    }
}
