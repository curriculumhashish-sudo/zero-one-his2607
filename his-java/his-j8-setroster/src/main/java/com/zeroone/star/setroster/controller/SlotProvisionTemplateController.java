package com.zeroone.star.setroster.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotProvisionTemplateAddDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotProvisionTemplateDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotProvisionTemplateDetailDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotProvisionTemplateModifyDTO;
import com.zeroone.star.project.enums.ResultStatus;
import com.zeroone.star.project.j8.setroster.SlotProvisionTemplateApis;
import com.zeroone.star.project.query.j8.setroster.SlotProvisionTemplateQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

/**
 * <p>
 * 描述：放号时段模板控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author Gin
 * @version 1.0.0
 */
@RestController
@Tag(name = "放号时段模板管理")
public class SlotProvisionTemplateController implements SlotProvisionTemplateApis {

    @Operation(summary = "分页查询放号时段模板列表")
    @GetMapping("j8/setroster/querySlotProvisionTemplates")
    @Override
    public JsonVO<PageDTO<SlotProvisionTemplateDTO>> querySlotProvisionTemplates(
            @Valid @ModelAttribute SlotProvisionTemplateQuery query) {
        return JsonVO.create(null, ResultStatus.API_UN_IMPL);
    }

    @Operation(summary = "查询放号时段模板详情")
    @GetMapping("j8/setroster/getSlotProvisionTemplate")
    @Override
    public JsonVO<SlotProvisionTemplateDetailDTO> getSlotProvisionTemplate(
            @RequestParam("id") String id) {
        return JsonVO.create(null, ResultStatus.API_UN_IMPL);
    }

    @Operation(summary = "新增放号时段模板")
    @PostMapping("j8/setroster/addSlotProvisionTemplate")
    @Override
    public JsonVO<String> addSlotProvisionTemplate(
            @Valid @RequestBody SlotProvisionTemplateAddDTO addDTO) {
        return JsonVO.create(null, ResultStatus.API_UN_IMPL);
    }

    @Operation(summary = "修改放号时段模板")
    @PutMapping("j8/setroster/modifySlotProvisionTemplate")
    @Override
    public JsonVO<String> modifySlotProvisionTemplate(
            @Valid @RequestBody SlotProvisionTemplateModifyDTO modifyDTO) {
        return JsonVO.create(null, ResultStatus.API_UN_IMPL);
    }

    @Operation(summary = "删除放号时段模板")
    @DeleteMapping("j8/setroster/deleteSlotProvisionTemplate/{id}")
    @Override
    public JsonVO<String> deleteSlotProvisionTemplate(
            @PathVariable("id") String id) {
        return JsonVO.create(null, ResultStatus.API_UN_IMPL);
    }
}
