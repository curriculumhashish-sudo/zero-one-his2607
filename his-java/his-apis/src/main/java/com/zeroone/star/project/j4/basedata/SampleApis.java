package com.zeroone.star.project.j4.basedata;

import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionDeleteDTO;
import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionStatusDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.sample.SpecimenDefinitionOperateVO;
import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionUpdateDTO;

/**
 * <p>
 * 描述：样本管理 API 接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author wieyao
 * @version 1.0.0
 */
public interface SampleApis {

    /**
     * 启用/停用样本定义
     * 根据主键与乐观锁版本号，更新样本定义的发布状态
     * @param dto 启停用请求数据
     * @return 操作结果
     */
    JsonVO<SpecimenDefinitionOperateVO> updateStatus(SpecimenDefinitionStatusDTO dto);

    /**
     * 删除样本定义
     * 根据主键与乐观锁版本号，软删除样本定义
     * @param dto 删除请求数据
     * @return 操作结果
     */
    JsonVO<SpecimenDefinitionOperateVO> delete(SpecimenDefinitionDeleteDTO dto);

    /**
     * 修改样本定义
     * 根据主键与乐观锁版本号，修改样本名称、标本种类
     * @param dto 修改请求数据
     * @return 操作结果
     */
    JsonVO<SpecimenDefinitionOperateVO> update(SpecimenDefinitionUpdateDTO dto);
}