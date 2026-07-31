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
import com.zeroone.star.setroster.service.IBkgSlotProvisionTemplateService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
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
@RequestMapping("/j8/setroster")
public class SlotProvisionTemplateController implements SlotProvisionTemplateApis {

    @Resource
    private IBkgSlotProvisionTemplateService slotProvisionTemplateService;

    @Operation(summary = "分页查询放号时段模板列表")
    @GetMapping("/querySlotProvisionTemplates")
    @Override
    public JsonVO<PageDTO<SlotProvisionTemplateDTO>> querySlotProvisionTemplates(
            @Valid @ModelAttribute SlotProvisionTemplateQuery query) {
        // 调用业务层查询（已过滤已删除记录、指定查询字段）
        PageDTO<SlotProvisionTemplateDTO> result = slotProvisionTemplateService.querySlotProvisionTemplatePage(query);
        return JsonVO.create(result, ResultStatus.SUCCESS);
    }

    @Operation(summary = "查询放号时段模板详情")
    @GetMapping("/getSlotProvisionTemplate")
    @Override
    public JsonVO<SlotProvisionTemplateDetailDTO> getSlotProvisionTemplate(
            @RequestParam("id") String id) {
        // 调用业务层查询详情（含时段JSON解析、已删除记录过滤）
        SlotProvisionTemplateDetailDTO detail = slotProvisionTemplateService.getSlotProvisionTemplateDetail(id);
        if (detail == null) {
            return JsonVO.create(null, ResultStatus.FAIL);
        }
        return JsonVO.create(detail, ResultStatus.SUCCESS);
    }

    @Operation(summary = "新增放号时段模板")
    @PostMapping("/addSlotProvisionTemplate")
    @Override
    public JsonVO<String> addSlotProvisionTemplate(
            @Valid @RequestBody SlotProvisionTemplateAddDTO addDTO) {
        // 调用业务层新增（含编码唯一性校验、时段列表序列化）
        String id = slotProvisionTemplateService.addSlotProvisionTemplate(addDTO);
        if (id == null) {
            // 编码重复或序列化失败
            return JsonVO.fail("模板编码已存在或参数异常");
        }
        return JsonVO.create(id, ResultStatus.SUCCESS);
    }

    @Operation(summary = "修改放号时段模板")
    @PutMapping("/modifySlotProvisionTemplate")
    @Override
    public JsonVO<String> modifySlotProvisionTemplate(
            @Valid @RequestBody SlotProvisionTemplateModifyDTO modifyDTO) {
        // 调用业务层修改（含存在性校验、编码唯一性校验、乐观锁）
        boolean success = slotProvisionTemplateService.modifySlotProvisionTemplate(modifyDTO);
        if (!success) {
            return JsonVO.fail("模板不存在或编码已存在");
        }
        return JsonVO.create("修改成功", ResultStatus.SUCCESS);
    }

    @Operation(summary = "删除放号时段模板")
    @DeleteMapping("/deleteSlotProvisionTemplate/{id}")
    @Override
    public JsonVO<String> deleteSlotProvisionTemplate(
            @PathVariable("id") String id) {
        // 调用业务层软删除（设置 is_deleted、deleted_at、deleted_by）
        boolean success = slotProvisionTemplateService.removeSlotProvisionTemplate(id);
        if (!success) {
            return JsonVO.fail("模板不存在或已删除");
        }
        return JsonVO.create("删除成功", ResultStatus.SUCCESS);
    }
}
