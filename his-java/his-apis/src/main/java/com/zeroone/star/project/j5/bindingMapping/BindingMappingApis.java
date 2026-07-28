package com.zeroone.star.project.j5.bindingMapping;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.bindingMapping.TreatmentBindingDTO;
import com.zeroone.star.project.dto.j5.bindingMapping.UsageBindingDTO;
import com.zeroone.star.project.query.j5.bindingMapping.TreatmentBindingQuery;
import com.zeroone.star.project.query.j5.bindingMapping.UsageBindingQuery;
import com.zeroone.star.project.vo.JsonVO;

/**
 * <p>
 * 描述：诊疗/用法绑定映射API
 * </p>
 *
 * @author 扶舒
 * @version 1.0.0
 */
public interface BindingMappingApis {

    /**
     * 查询所有当前诊疗绑定信息
     *
     * @param query 当前诊疗绑定查询对象
     * @return 当前诊疗绑定信息查询结果
     */
    JsonVO<PageDTO<TreatmentBindingDTO>> queryAllTreatmentBindings(TreatmentBindingQuery query);

    /**
     * 查询所有当前用法绑定信息
     *
     * @param query 当前用法绑定查询对象
     * @return 当前用法绑定信息查询结果
     */
    JsonVO<PageDTO<UsageBindingDTO>> queryAllUsageBindings(UsageBindingQuery query);

    /**
     * 添加当前诊疗绑定信息
     *
     * @param data 当前诊疗绑定添加对象
     * @return 添加结果
     */
    JsonVO<String> addTreatmentBinding(TreatmentBindingDTO data);

    /**
     * 添加当前用法绑定信息
     *
     * @param data 当前用法绑定添加对象
     * @return 添加结果
     */
    JsonVO<String> addUsageBinding(UsageBindingDTO data);

    /**
     * 编辑当前诊疗绑定信息
     *
     * @param data 当前诊疗绑定编辑对象
     * @return 编辑结果
     */
    JsonVO<String> modifyTreatmentBinding(TreatmentBindingDTO data);

    /**
     * 编辑当前用法绑定信息
     *
     * @param data 当前用法绑定编辑对象
     * @return 编辑结果
     */
    JsonVO<String> modifyUsageBinding(UsageBindingDTO data);

    /**
     * 删除当前诊疗绑定信息
     *
     * @param id 诊疗绑定主键ID
     * @return 删除结果
     */
    JsonVO<String> deleteTreatmentBinding(String id);

    /**
     * 删除当前用法绑定信息
     *
     * @param id 用法绑定主键ID
     * @return 删除结果
     */
    JsonVO<String> deleteUsageBinding(String id);
}
