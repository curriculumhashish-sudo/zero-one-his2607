package com.zeroone.star.setroster.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotProvisionTemplateAddDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotProvisionTemplateDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotProvisionTemplateModifyDTO;
import com.zeroone.star.project.enums.ResultStatus;
import com.zeroone.star.project.j8.setroster.SlotProvisionTemplateApis;
import com.zeroone.star.project.query.j8.setroster.SlotProvisionTemplateQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.setroster.SlotProvisionTemplateOptionVO;
import com.zeroone.star.setroster.service.IBkgSlotProvisionTemplateService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

import java.util.List;

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
@RequestMapping("/j8/setroster") // 统一模块路径前缀
public class SlotProvisionTemplateController implements SlotProvisionTemplateApis {

    @Resource
    private IBkgSlotProvisionTemplateService slotProvisionTemplateService;

    @Operation(summary = "获取放号时段模板名称")
    @GetMapping("/listSlotProvisionTemplateOptions")
    @Override
    public JsonVO<List<SlotProvisionTemplateOptionVO>> listSlotProvisionTemplateOptions(
            @RequestParam(value = "organizationId", required = false) String organizationId) {
        // 调用业务层获取名称下拉选项（只查启用状态的模板）
        List<SlotProvisionTemplateOptionVO> options = slotProvisionTemplateService.listSlotProvisionTemplateOptions(organizationId);
        return JsonVO.create(options, ResultStatus.SUCCESS);
    }

    @Operation(summary = "分页查询放号时段模板列表")
    @GetMapping("/querySlotProvisionTemplates")
    @Override
    public JsonVO<PageDTO<SlotProvisionTemplateDTO>> querySlotProvisionTemplates(
            @Valid @ModelAttribute SlotProvisionTemplateQuery query) {
        // 调用业务层查询（已过滤已删除记录、填充时段列表）
        PageDTO<SlotProvisionTemplateDTO> result = slotProvisionTemplateService.querySlotProvisionTemplatePage(query);
        return JsonVO.create(result, ResultStatus.SUCCESS);
    }

    @Operation(summary = "新建放号时段模板")
    @PostMapping("/addSlotProvisionTemplate")
    @Override
    public JsonVO<String> addSlotProvisionTemplate(
            @Valid @RequestBody SlotProvisionTemplateAddDTO addDTO) {
        // 调用业务层新建（含编码唯一性校验、时段列表序列化）
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
