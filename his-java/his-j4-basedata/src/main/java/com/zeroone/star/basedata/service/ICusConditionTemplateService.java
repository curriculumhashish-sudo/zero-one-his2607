package com.zeroone.star.basedata.service;

import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisTemplateDTO;
import com.zeroone.star.project.query.j4.diagnosis.DiagnosisTemplateQuery;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisTemplateVO;

import java.util.List;

/**
 * <p>
 * 描述：自定义条件模板（诊断模板）服务接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
public interface ICusConditionTemplateService {

    /**
     * 分页查询诊断模板
     * @param query 查询条件
     * @return 模板列表
     */
    List<DiagnosisTemplateVO> pageQuery(DiagnosisTemplateQuery query);

    /**
     * 新增诊断模板
     * @param dto 模板数据
     * @return 新增模板 ID
     */
    String saveTemplate(DiagnosisTemplateDTO dto);

    /**
     * 编辑诊断模板
     * @param dto 模板数据
     */
    void modifyTemplate(DiagnosisTemplateDTO dto);

    /**
     * 删除诊断模板
     * @param id 模板 ID
     */
    void deleteTemplate(String id);
}
