package com.zeroone.star.setroster.service;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotProvisionTemplateAddDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotProvisionTemplateDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotProvisionTemplateDetailDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotProvisionTemplateModifyDTO;
import com.zeroone.star.project.query.j8.setroster.SlotProvisionTemplateQuery;
import com.zeroone.star.setroster.entity.BkgSlotProvisionTemplate;

/**
 * <p>
 * T1 放号时段模板 服务类
 * </p>
 *
 * @author Gin
 * @since 2026-07-24
 */
public interface IBkgSlotProvisionTemplateService extends IService<BkgSlotProvisionTemplate> {

    /**
     * 分页查询放号时段模板列表
     * @param query 查询条件（关键词、机构、启用状态、分页参数）
     * @return 分页数据
     */
    PageDTO<SlotProvisionTemplateDTO> querySlotProvisionTemplatePage(SlotProvisionTemplateQuery query);

    /**
     * 查询放号时段模板详情（含时段列表JSON解析）
     * @param id 模板主键ID
     * @return 详情数据，不存在或已删除返回null
     */
    SlotProvisionTemplateDetailDTO getSlotProvisionTemplateDetail(String id);

    /**
     * 新增放号时段模板
     * @param addDTO 新增参数
     * @return 新增成功返回主键ID，编码重复返回null
     */
    String addSlotProvisionTemplate(SlotProvisionTemplateAddDTO addDTO);

    /**
     * 修改放号时段模板
     * @param modifyDTO 修改参数
     * @return 修改成功返回true，记录不存在返回false
     */
    boolean modifySlotProvisionTemplate(SlotProvisionTemplateModifyDTO modifyDTO);

    /**
     * 删除放号时段模板（软删除，填充删除审计字段）
     * @param id 模板主键ID
     * @return 删除成功返回true，记录不存在返回false
     */
    boolean removeSlotProvisionTemplate(String id);
}
