package com.zeroone.star.elk.controller;

import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * <p>
 * 描述：ELK示例控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@RestController
@RequestMapping("elk")
@Tag(name = "elk", description = "ELK示例控制器")
public class ElkController {

    @Operation(summary = "测试访问")
    @GetMapping("test")
    public String test() {
        int a = 1 / 0; // 故意除以零，触发异常
        return "hello elk";
    }
}
