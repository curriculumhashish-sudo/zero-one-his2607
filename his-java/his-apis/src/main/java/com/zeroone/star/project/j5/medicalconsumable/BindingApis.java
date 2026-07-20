package com.zeroone.star.project.j5.medicalconsumable;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.medicalconsumable.BindingDTO;
import com.zeroone.star.project.query.PageQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.*;

/**
 * <p>
 * 描述：诊疗耗材绑定api
 * </p>
 *
 * @author 扶舒
 * @version 1.0.0
 */
@Tag(name = "诊疗耗材绑定")
@RequestMapping("/binding")
public interface BindingApis {

    @Operation(summary = "分页查询绑定列表")
    @GetMapping("/page")
    JsonVO<PageDTO<BindingDTO>> queryPage(PageQuery pageQuery);

    @Operation(summary = "添加绑定")
    @PostMapping("/binding")
    JsonVO<String> addBinding(@RequestBody BindingDTO data);

    @Operation(summary = "修改绑定")
    @PutMapping("/binding")
    JsonVO<String> updateBinding(@RequestBody BindingDTO data);

    @Operation(summary = "删除绑定")
    @DeleteMapping("/binding/{id}")
    JsonVO<String> delBinding(@PathVariable String id);

    @Operation(summary = "切换绑定状态")
    @PutMapping("/binding/status")
    JsonVO<String> toggleStatus(@RequestBody BindingDTO data);
}