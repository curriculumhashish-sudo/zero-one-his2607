package com.zeroone.star.setroster.controller;

import cn.hutool.core.bean.BeanUtil;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotProvisionTemplateAddDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotProvisionTemplateDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotProvisionTemplateDetailDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotProvisionTemplateModifyDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotTemplateItemDTO;
import com.zeroone.star.project.enums.ResultStatus;
import com.zeroone.star.project.j8.setroster.SlotProvisionTemplateApis;
import com.zeroone.star.project.query.j8.setroster.SlotProvisionTemplateQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.setroster.entity.BkgSlotProvisionTemplate;
import com.zeroone.star.setroster.service.IBkgSlotProvisionTemplateService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springframework.util.StringUtils;
import org.springframework.web.bind.annotation.*;

import java.util.Collections;
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
@RequestMapping("/j8/setroster")
public class SlotProvisionTemplateController implements SlotProvisionTemplateApis {

    @Resource
    private IBkgSlotProvisionTemplateService slotProvisionTemplateService;

    private static final ObjectMapper objectMapper = new ObjectMapper();

    @Operation(summary = "分页查询放号时段模板列表")
    @GetMapping("/querySlotProvisionTemplates")
    @Override
    public JsonVO<PageDTO<SlotProvisionTemplateDTO>> querySlotProvisionTemplates(
            @Valid @ModelAttribute SlotProvisionTemplateQuery query) {

        Page<BkgSlotProvisionTemplate> page = new Page<>(query.getPageIndex(), query.getPageSize());
        LambdaQueryWrapper<BkgSlotProvisionTemplate> wrapper = new LambdaQueryWrapper<>();

        // 关键词搜索：匹配模板编码或模板名称
        if (StringUtils.hasText(query.getKeyword())) {
            wrapper.and(w -> w
                    .like(BkgSlotProvisionTemplate::getTemplateCode, query.getKeyword())
                    .or()
                    .like(BkgSlotProvisionTemplate::getTemplateName, query.getKeyword()));
        }
        if (StringUtils.hasText(query.getOrganizationId())) {
            wrapper.eq(BkgSlotProvisionTemplate::getOrganizationId, query.getOrganizationId());
        }
        if (query.getEnabled() != null) {
            wrapper.eq(BkgSlotProvisionTemplate::getEnabled, query.getEnabled());
        }
        wrapper.orderByDesc(BkgSlotProvisionTemplate::getCreatedAt);

        PageDTO<SlotProvisionTemplateDTO> result = PageDTO.create(
                slotProvisionTemplateService.page(page, wrapper),
                SlotProvisionTemplateDTO::new);

        return JsonVO.create(result, ResultStatus.SUCCESS);
    }

    @Operation(summary = "查询放号时段模板详情")
    @GetMapping("/getSlotProvisionTemplate")
    @Override
    public JsonVO<SlotProvisionTemplateDetailDTO> getSlotProvisionTemplate(
            @RequestParam("id") String id) {

        BkgSlotProvisionTemplate entity = slotProvisionTemplateService.getById(id);
        if (entity == null) {
            return JsonVO.create(null, ResultStatus.FAIL);
        }

        SlotProvisionTemplateDetailDTO detail = new SlotProvisionTemplateDetailDTO();
        BeanUtil.copyProperties(entity, detail);

        // 解析 slot_templates_json → List<SlotTemplateItemDTO>
        if (StringUtils.hasText(entity.getSlotTemplatesJson())) {
            try {
                List<SlotTemplateItemDTO> items = objectMapper.readValue(
                        entity.getSlotTemplatesJson(),
                        new TypeReference<List<SlotTemplateItemDTO>>() {});
                detail.setSlotTemplates(items);
            } catch (Exception e) {
                detail.setSlotTemplates(Collections.emptyList());
            }
        }
        return JsonVO.create(detail, ResultStatus.SUCCESS);
    }

    @Operation(summary = "新增放号时段模板")
    @PostMapping("/addSlotProvisionTemplate")
    @Override
    public JsonVO<String> addSlotProvisionTemplate(
            @Valid @RequestBody SlotProvisionTemplateAddDTO addDTO) {

        BkgSlotProvisionTemplate entity = new BkgSlotProvisionTemplate();
        BeanUtil.copyProperties(addDTO, entity);

        // 时段列表 → JSON 字符串
        try {
            entity.setSlotTemplatesJson(objectMapper.writeValueAsString(addDTO.getSlotTemplates()));
        } catch (Exception e) {
            return JsonVO.create(null, ResultStatus.PARAMS_INVALID);
        }

        slotProvisionTemplateService.save(entity);
        return JsonVO.create(entity.getId(), ResultStatus.SUCCESS);
    }

    @Operation(summary = "修改放号时段模板")
    @PutMapping("/modifySlotProvisionTemplate")
    @Override
    public JsonVO<String> modifySlotProvisionTemplate(
            @Valid @RequestBody SlotProvisionTemplateModifyDTO modifyDTO) {

        BkgSlotProvisionTemplate entity = new BkgSlotProvisionTemplate();
        BeanUtil.copyProperties(modifyDTO, entity);

        if (modifyDTO.getSlotTemplates() != null) {
            try {
                entity.setSlotTemplatesJson(objectMapper.writeValueAsString(modifyDTO.getSlotTemplates()));
            } catch (Exception e) {
                return JsonVO.create(null, ResultStatus.PARAMS_INVALID);
            }
        }

        slotProvisionTemplateService.updateById(entity);
        return JsonVO.create("修改成功", ResultStatus.SUCCESS);
    }

    @Operation(summary = "删除放号时段模板")
    @DeleteMapping("/deleteSlotProvisionTemplate/{id}")
    @Override
    public JsonVO<String> deleteSlotProvisionTemplate(
            @PathVariable("id") String id) {

        slotProvisionTemplateService.removeById(id);
        return JsonVO.create("删除成功", ResultStatus.SUCCESS);
    }
}
