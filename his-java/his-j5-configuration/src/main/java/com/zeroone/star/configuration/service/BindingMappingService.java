package com.zeroone.star.configuration.service;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.bindingMapping.TreatmentBindingDTO;
import com.zeroone.star.project.dto.j5.bindingMapping.UsageBindingDTO;
import com.zeroone.star.project.query.j5.bindingMapping.TreatmentBindingQuery;
import com.zeroone.star.project.query.j5.bindingMapping.UsageBindingQuery;
import com.zeroone.star.project.vo.JsonVO;

/**
 * <p>
 * 描述：诊疗/用法绑定映射服务接口
 * </p>
 *
 * @author 扶舒
 * @version 1.0.0
 */
public interface BindingMappingService {

    /**
     * 查询诊疗绑定列表
     *
     * @param query 诊疗绑定查询对象
     * @return 分页结果
     */
    JsonVO<PageDTO<TreatmentBindingDTO>> queryAllTreatmentBindings(TreatmentBindingQuery query);

    /**
     * 查询用法绑定列表
     *
     * @param query 用法绑定查询对象
     * @return 分页结果
     */
    JsonVO<PageDTO<UsageBindingDTO>> queryAllUsageBindings(UsageBindingQuery query);

    /**
     * 添加诊疗绑定
     *
     * @param data 诊疗绑定数据
     * @return 操作结果
     */
    JsonVO<String> addTreatmentBinding(TreatmentBindingDTO data);

    /**
     * 添加用法绑定
     *
     * @param data 用法绑定数据
     * @return 操作结果
     */
    JsonVO<String> addUsageBinding(UsageBindingDTO data);

    /**
     * 编辑诊疗绑定
     *
     * @param data 诊疗绑定数据
     * @return 操作结果
     */
    JsonVO<String> modifyTreatmentBinding(TreatmentBindingDTO data);

    /**
     * 编辑用法绑定
     *
     * @param data 用法绑定数据
     * @return 操作结果
     */
    JsonVO<String> modifyUsageBinding(UsageBindingDTO data);

    /**
     * 删除诊疗绑定
     *
     * @param id 诊疗绑定主键ID
     * @return 操作结果
     */
    JsonVO<String> deleteTreatmentBinding(String id);

    /**
     * 删除用法绑定
     *
     * @param id 用法绑定主键ID
     * @return 操作结果
     */
    JsonVO<String> deleteUsageBinding(String id);
}
