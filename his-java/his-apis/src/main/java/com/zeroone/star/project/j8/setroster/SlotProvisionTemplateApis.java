package com.zeroone.star.project.j8.setroster;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotProvisionTemplateAddDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotProvisionTemplateDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotProvisionTemplateModifyDTO;
import com.zeroone.star.project.query.j8.setroster.SlotProvisionTemplateQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.setroster.SlotProvisionTemplateOptionVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;

import java.util.List;

/**
 * <p>
 * 描述：放号时段模板接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author Gin
 * @version 1.0.0
 */
@Tag(name = "放号时段模板管理")
public interface SlotProvisionTemplateApis {

    /**
     * 获取放号时段模板名称（下拉选项）
     * @param organizationId 机构/科室ID，为空则查询所有
     * @return 放号时段模板名称选项列表
     */
    @Operation(summary = "获取放号时段模板名称")
    JsonVO<List<SlotProvisionTemplateOptionVO>> listSlotProvisionTemplateOptions(String organizationId);

    /**
     * 分页查询放号时段模板列表
     * @param query 查询条件
     * @return 放号时段模板列表
     */
    @Operation(summary = "分页查询放号时段模板列表")
    JsonVO<PageDTO<SlotProvisionTemplateDTO>> querySlotProvisionTemplates(SlotProvisionTemplateQuery query);

    /**
     * 新建放号时段模板
     * @param addDTO 新建放号时段模板参数
     * @return 新建结果
     */
    @Operation(summary = "新建放号时段模板")
    JsonVO<String> addSlotProvisionTemplate(SlotProvisionTemplateAddDTO addDTO);

    /**
     * 修改放号时段模板
     * @param modifyDTO 修改放号时段模板参数
     * @return 修改结果
     */
    @Operation(summary = "修改放号时段模板")
    JsonVO<String> modifySlotProvisionTemplate(SlotProvisionTemplateModifyDTO modifyDTO);

    /**
     * 删除放号时段模板
     * @param id 放号时段模板主键ID
     * @return 删除结果
     */
    @Operation(summary = "删除放号时段模板")
    JsonVO<String> deleteSlotProvisionTemplate(String id);
}
