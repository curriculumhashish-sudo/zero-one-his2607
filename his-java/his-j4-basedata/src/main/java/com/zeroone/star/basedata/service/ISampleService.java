package com.zeroone.star.basedata.service;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.basedata.entity.sample.AdmSpecimenDefinitionDO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionAddDTO;
import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionDeleteDTO;
import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionStatusDTO;
import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionUpdateDTO;
import com.zeroone.star.project.query.j4.sample.SpecimenDefinitionQuery;
import com.zeroone.star.project.vo.j4.sample.SpecimenDefinitionOperateVO;
import com.zeroone.star.project.vo.j4.sample.SpecimenDefinitionDetailVO;
import com.zeroone.star.project.vo.j4.sample.SpecimenOptionVO;

import java.util.List;

/**
 * <p>
 * 描述：样本管理服务接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
public interface ISampleService extends IService<AdmSpecimenDefinitionDO> {

    /**
     * 获取样本名称下拉选项（限定当前租户）
     *
     * @return 样本名称下拉选项列表
     */
    List<SpecimenOptionVO> listSampleNames();

    /**
     * 分页+条件查询样本定义列表（限定当前租户）
     *
     * @param query 分页查询条件
     * @return 分页结果
     */
    PageDTO<SpecimenDefinitionDetailVO> querySamplePage(SpecimenDefinitionQuery query);

    /**
     * 新增样本定义（服务端调用 PublicationStatus 枚举默认置为启用 ACTIVE）
     *
     * @param dto 新增数据
     * @return 操作结果提示
     */
    String addSample(SpecimenDefinitionAddDTO dto);

    /**
     * 修改样本定义（仅修改名称/标本种类/检索码，状态保持原值）
     *
     * @param dto 修改数据
     * @return 操作结果
     */
    SpecimenDefinitionOperateVO updateSample(SpecimenDefinitionUpdateDTO dto);

    /**
     * 软删除样本定义（限定当前租户）
     *
     * @param dto 删除数据
     * @return 操作结果
     */
    SpecimenDefinitionOperateVO deleteSample(SpecimenDefinitionDeleteDTO dto);

    /**
     * 启用/停用样本定义（限定当前租户）
     *
     * @param dto 启停用数据
     * @return 操作结果
     */
    SpecimenDefinitionOperateVO updateSampleStatus(SpecimenDefinitionStatusDTO dto);
}
