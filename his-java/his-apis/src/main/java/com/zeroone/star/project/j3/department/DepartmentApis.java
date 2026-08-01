package com.zeroone.star.project.j3.department;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.department.DepartmentBatchDTO;
import com.zeroone.star.project.dto.j3.department.DepartmentCreateDTO;

import com.zeroone.star.project.dto.j3.department.DepartmentUpdateDTO;
import com.zeroone.star.project.query.j3.department.DepartmentQuery;
import com.zeroone.star.project.query.j3.department.DepartmentTreeQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j3.department.DepartmentDetailVO;
import com.zeroone.star.project.vo.j3.department.DepartmentTreeVO;
import com.zeroone.star.project.vo.j3.department.DepartmentVO;
import io.swagger.v3.oas.annotations.Parameter;
import jakarta.validation.Valid;
import jakarta.validation.constraints.Pattern;
import org.springframework.validation.annotation.Validated;

import java.util.List;

/**
 * <p>
 * 描述：科室管理接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 胡傲宇
 * @version 1.0.0
 */
@Validated
public interface DepartmentApis {
    /**
     * 获取科室名称树（支持门诊/住院筛选）
     *
     * @param query 科室名称树筛选条件
     * @return 科室名称树结果
     */
    JsonVO<List<DepartmentTreeVO>> getDepartmentTree(@Valid DepartmentTreeQuery query);

    /**
     * 获取科室详情
     *
     * @param id 科室对象ID
     * @return 科室详情结果
     */
    JsonVO<DepartmentDetailVO> getDepartmentDetail(@Parameter(description = "科室对象ID")
                                                   @Pattern(regexp = "^[0-9]+$", message = "科室对象ID只能是数字") String id);

    /**
     * 新增科室
     *
     * @param dto 新增科室对象
     * @return 提示信息
     */
    JsonVO<String> createDepartment(@Valid DepartmentCreateDTO dto) throws Exception;

    /**
     * 启用/停用科室(支持批量)
     *
     * @param dto 批量操作参数（含id列表+状态枚举）
     * @return 提示信息
     */
    JsonVO<String> activeOrInactiveDepartment(@Valid DepartmentBatchDTO dto);

    /**
     * 获取科室列表（条件+分页）
     *
     * @param query 科室列表查询条件
     * @return 科室分页列表
     */
    JsonVO<PageDTO<DepartmentVO>> getDepartmentPage(@Valid DepartmentQuery query);

    /**
     * 修改科室
     *
     * @param id  科室对象ID
     * @param dto 修改科室对象
     * @return 提示信息
     */
    JsonVO<String> updateDepartment(@Parameter(description = "科室ID", example = "322663105141149696", required = true) String id,
                                    @Valid DepartmentUpdateDTO dto);

    /**
     * 删除科室
     *
     * @param id 科室对象ID
     * @return 提示信息
     */
    JsonVO<String> deleteDepartment(@Parameter(description = "科室ID", example = "322663105141149696", required = true) String id);
}
