package com.zeroone.star.project.j3.department;

import com.zeroone.star.project.dto.j3.department.DepartmentCreateDTO;
import com.zeroone.star.project.dto.j3.department.DepartmentDTO;
import com.zeroone.star.project.query.j3.department.DepartmentTreeQuery;
import com.zeroone.star.project.vo.JsonVO;

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
public interface DepartmentApis {
    /**
     * 获取科室名称树（支持门诊/住院筛选）
     *
     * @param query 科室名称树筛选条件
     * @return 科室名称树结果
     */
    JsonVO<DepartmentDTO> getDepartmentTree(DepartmentTreeQuery query);

    /**
     * 获取科室详情
     *
     * @param id 科室对象ID
     * @return 科室详情结果
     */
    JsonVO<DepartmentDTO> getDepartmentDetail(Long id);

    /**
     * 新增科室
     *
     * @param dto 新增科室对象
     * @return 提示信息
     */
    JsonVO<String> createDepartment(DepartmentCreateDTO dto);

    /**
     * 启用/停用科室(支持批量)
     *
     * @param ids 启用/停用科室对象ID集合
     * @return 提示信息
     */
    JsonVO<String> activeOrInactiveDepartment(List<Long> ids);
}
