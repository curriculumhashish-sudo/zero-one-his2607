package com.zeroone.star.configuration.service;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.configuration.entity.EmrTemplateDO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.j5.emr.template.query.MedicalTemplateQuery;
import com.zeroone.star.project.j5.emr.template.vo.MedicalTemplateDetailVO;
import com.zeroone.star.project.j5.emr.template.vo.MedicalTemplateVO;

/**
 * 病历模板服务接口
 */
public interface IEmrTemplateService extends IService<EmrTemplateDO> {

    /**
     * 分页查询病历模板列表
     *
     * @param query 分页查询条件
     * @return 分页结果
     */
    PageDTO<MedicalTemplateVO> listTemplates(MedicalTemplateQuery query);

    /**
     * 根据ID获取病历模板详情
     *
     * @param id 模板ID
     * @return 模板详情
     */
    MedicalTemplateDetailVO getTemplateDetail(String id);
}
