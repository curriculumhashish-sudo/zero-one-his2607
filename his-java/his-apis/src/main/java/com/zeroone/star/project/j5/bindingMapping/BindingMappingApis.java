package com.zeroone.star.project.j5.bindingMapping;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.bindingMapping.BindingInitDTO;
import com.zeroone.star.project.dto.j5.bindingMapping.TreatmentBindingDTO;
import com.zeroone.star.project.query.j5.bindingMapping.TreatmentBindingQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.*;

/**
 * <p>
 * 描述：诊疗/用法绑定映射API
 * </p>
 *
 * @author 扶舒
 * @version 1.0.0
 */
@Tag(name = "配置管理-整料耗材绑定", description = "诊疗/用法绑定映射接口")
@RequestMapping("/binding-mapping")
public interface BindingMappingApis {

    /**
     * 初始化绑定映射页面，返回下拉选项等初始化数据
     */
    @Operation(summary = "初始化", description = "返回绑定映射页面初始化数据")
    @GetMapping("/init")
    JsonVO<BindingInitDTO> init();

    /**
     * 查询所有当前诊疗绑定信息
     *
     * @param query 当前诊疗绑定查询对象
     * @return 当前诊疗绑定信息查询结果
     */
    @Operation(summary = "获取绑定数据", description = "条件+分页查询诊疗绑定信息")
    @GetMapping("/treatment/page")
    JsonVO<PageDTO<TreatmentBindingDTO>> queryAllTreatmentBindings(TreatmentBindingQuery query);

    /**
     * 添加当前诊疗绑定信息
     *
     * @param data 当前诊疗绑定添加对象
     * @return 添加结果
     */
    @Operation(summary = "添加绑定", description = "添加诊疗绑定信息")
    @PostMapping("/treatment")
    JsonVO<String> addTreatmentBinding(@RequestBody TreatmentBindingDTO data);

    /**
     * 编辑当前诊疗绑定信息
     *
     * @param data 当前诊疗绑定编辑对象
     * @return 编辑结果
     */
    @Operation(summary = "修改绑定", description = "修改诊疗绑定信息")
    @PutMapping("/treatment")
    JsonVO<String> modifyTreatmentBinding(@RequestBody TreatmentBindingDTO data);

    /**
     * 删除当前诊疗绑定信息
     *
     * @param id 诊疗绑定主键ID
     * @return 删除结果
     */
    @Operation(summary = "删除绑定", description = "删除诊疗绑定信息")
    @DeleteMapping("/treatment/{id}")
    JsonVO<String> deleteTreatmentBinding(@PathVariable String id);

}
