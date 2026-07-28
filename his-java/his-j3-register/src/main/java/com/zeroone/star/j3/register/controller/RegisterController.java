package com.zeroone.star.j3.register.controller;

import com.zeroone.star.j3.register.service.IRegisterTypeService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.register.RegisterDTO;
import com.zeroone.star.project.j3.register.RegisterApis;
import com.zeroone.star.project.query.j3.register.RegisterQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j3.register.RegisterNameVO;
import com.zeroone.star.project.vo.j3.register.RegisterVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

/**
 * <p>
 * 描述：挂号类型管理 Controller
 * </p>
 * <p>负责：列表查询、新增、修改、删除挂号类型（如普通号、专家号、特需号等）。
 * 用于门诊挂号页「号源类型」下拉使用。</p>
 *
 * @author j3小组
 * @version 1.0.0
 */
@RestController
@RequestMapping("/register-types")
@Tag(name = "挂号类型管理", description = "挂号类型（普通号、专家号、特需号等）相关接口")
public class RegisterController implements RegisterApis {

    @Resource
    IRegisterTypeService registerTypeService;

    /**
     * 获取挂号类型名称列表（不分页）
     *
     * @return 名称列表（每条包含唯一标识、名称）
     */
    @GetMapping("query-names")
    @Operation(summary = "获取挂号类型名称", description = "返回启用的挂号类型 id + 名称，用于门诊挂号页「号源类型」下拉选择")
    @Override
    public JsonVO<List<RegisterNameVO>> queryRegisterNames() {
        return JsonVO.success(registerTypeService.queryRegisterNames());
    }

    /**
     * 获取挂号类型列表（条件+分页）
     *
     * @param query 分页查询条件（关键词可输入名称、医保编码或id进行模糊查询）
     * @return 分页数据结果
     */
    @GetMapping("query-list")
    @Operation(summary = "获取挂号类型列表", description = "支持按名称、医保编码、id进行模糊查询，分页返回数据")
    @Override
    public JsonVO<PageDTO<RegisterVO>> queryRegisterList(RegisterQuery query) {
        return JsonVO.success(registerTypeService.queryRegisterList(query));
    }

    /**
     * 新增挂号类型
     *
     * @param dto 新增数据
     * @return 新增结果
     */
    @PostMapping("save-add")
    @Operation(summary = "新增挂号类型", description = "新增一条挂号类型记录")
    @Override
    public JsonVO<Boolean> addRegister(@RequestBody RegisterDTO dto) {
        // TODO: 后续由 service 层实现
        return JsonVO.success(Boolean.FALSE);
    }

    /**
     * 修改挂号类型
     *
     * @param dto 修改数据（id 必传）
     * @return 修改结果
     */
    @PutMapping("save-modify")
    @Operation(summary = "修改挂号类型", description = "根据 id 修改一条挂号类型记录")
    @Override
    public JsonVO<Boolean> modifyRegister(@RequestBody RegisterDTO dto) {
        // TODO: 后续由 service 层实现
        return JsonVO.success(Boolean.FALSE);
    }

    /**
     * 删除挂号类型
     *
     * @param id 挂号类型唯一标识
     * @return 删除结果
     */
    @DeleteMapping("remove-by-id/{id}")
    @Operation(summary = "删除挂号类型", description = "根据 id 删除一条挂号类型记录")
    @Parameter(name = "id", description = "挂号类型唯一标识", example = "1", required = true)
    @Override
    public JsonVO<Boolean> removeRegister(@PathVariable Long id) {
        // TODO: 后续由 service 层实现
        return JsonVO.success(Boolean.FALSE);
    }
}
