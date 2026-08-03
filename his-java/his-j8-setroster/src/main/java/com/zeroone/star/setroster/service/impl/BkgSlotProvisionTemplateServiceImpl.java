package com.zeroone.star.setroster.service.impl;

import cn.hutool.core.bean.BeanUtil;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotProvisionTemplateAddDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotProvisionTemplateDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotProvisionTemplateModifyDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotTemplateItemDTO;
import com.zeroone.star.project.query.j8.setroster.SlotProvisionTemplateQuery;
import com.zeroone.star.project.vo.j8.setroster.SlotProvisionTemplateOptionVO;
import com.zeroone.star.setroster.entity.BkgSlotProvisionTemplate;
import com.zeroone.star.setroster.mapper.BkgSlotProvisionTemplateMapper;
import com.zeroone.star.setroster.service.IBkgSlotProvisionTemplateService;
import jakarta.annotation.Resource;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * <p>
 * T1 放号时段模板 服务实现类
 * </p>
 *
 * @author Gin
 * @since 2026-07-24
 */
@Service
public class BkgSlotProvisionTemplateServiceImpl
        extends ServiceImpl<BkgSlotProvisionTemplateMapper, BkgSlotProvisionTemplate>
        implements IBkgSlotProvisionTemplateService {

    private static final Logger log = LoggerFactory.getLogger(BkgSlotProvisionTemplateServiceImpl.class);

    private static final ObjectMapper objectMapper = new ObjectMapper();

    @Resource
    private UserHolder userHolder;

    @Override
    public List<SlotProvisionTemplateOptionVO> listSlotProvisionTemplateOptions(String organizationId) {
        LambdaQueryWrapper<BkgSlotProvisionTemplate> wrapper = new LambdaQueryWrapper<>();
        // 只查选项需要的字段
        wrapper.select(
                BkgSlotProvisionTemplate::getId,
                BkgSlotProvisionTemplate::getTemplateCode,
                BkgSlotProvisionTemplate::getTemplateName,
                BkgSlotProvisionTemplate::getCreatedAt
        );
        // 按机构筛选
        if (StringUtils.hasText(organizationId)) {
            wrapper.eq(BkgSlotProvisionTemplate::getOrganizationId, organizationId);
        }
        // 只查启用的模板
        wrapper.eq(BkgSlotProvisionTemplate::getEnabled, true);
        // 按创建时间倒序
        wrapper.orderByDesc(BkgSlotProvisionTemplate::getCreatedAt);

        List<BkgSlotProvisionTemplate> entities = this.list(wrapper);
        List<SlotProvisionTemplateOptionVO> options = new ArrayList<>();
        for (BkgSlotProvisionTemplate entity : entities) {
            SlotProvisionTemplateOptionVO option = new SlotProvisionTemplateOptionVO();
            option.setValue(entity.getId());
            option.setLabel(entity.getTemplateName());
            option.setBusNo(entity.getTemplateCode());
            option.setSort(options.size() + 1);
            // 放号时段模板无子节点
            option.setChildren(null);
            options.add(option);
        }
        return options;
    }

    @Override
    public PageDTO<SlotProvisionTemplateDTO> querySlotProvisionTemplatePage(SlotProvisionTemplateQuery query) {
        Page<BkgSlotProvisionTemplate> page = new Page<>(query.getPageIndex(), query.getPageSize());
        LambdaQueryWrapper<BkgSlotProvisionTemplate> wrapper = new LambdaQueryWrapper<>();

        // 关键词搜索：匹配模板编码或模板名称
        if (StringUtils.hasText(query.getKeyword())) {
            wrapper.and(w -> w
                    .like(BkgSlotProvisionTemplate::getTemplateCode, query.getKeyword())
                    .or()
                    .like(BkgSlotProvisionTemplate::getTemplateName, query.getKeyword()));
        }
        // 机构精确匹配
        if (StringUtils.hasText(query.getOrganizationId())) {
            wrapper.eq(BkgSlotProvisionTemplate::getOrganizationId, query.getOrganizationId());
        }
        // 启用状态精确匹配
        if (query.getEnabled() != null) {
            wrapper.eq(BkgSlotProvisionTemplate::getEnabled, query.getEnabled());
        }
        // 按创建时间倒序
        wrapper.orderByDesc(BkgSlotProvisionTemplate::getCreatedAt);

        // is_deleted 由 @TableLogic 自动过滤
        // 使用回调转换，填充 slotTemplates 字段
        return PageDTO.create(this.page(page, wrapper), entity -> {
            SlotProvisionTemplateDTO dto = new SlotProvisionTemplateDTO();
            BeanUtil.copyProperties(entity, dto);
            // 解析 slot_templates_json → List<SlotTemplateItemDTO>
            if (StringUtils.hasText(entity.getSlotTemplatesJson())) {
                try {
                    List<SlotTemplateItemDTO> items = objectMapper.readValue(
                            entity.getSlotTemplatesJson(),
                            new TypeReference<List<SlotTemplateItemDTO>>() {});
                    dto.setSlotTemplates(items);
                } catch (Exception e) {
                    log.error("解析时段模板JSON失败，templateId={}", entity.getId(), e);
                    dto.setSlotTemplates(Collections.emptyList());
                }
            }
            return dto;
        });
    }

    @Override
    public String addSlotProvisionTemplate(SlotProvisionTemplateAddDTO addDTO) {
        // 校验模板编码唯一性（@TableLogic 自动排除已删除记录）
        LambdaQueryWrapper<BkgSlotProvisionTemplate> codeWrapper = new LambdaQueryWrapper<>();
        codeWrapper.eq(BkgSlotProvisionTemplate::getTemplateCode, addDTO.getTemplateCode());
        long count = this.count(codeWrapper);
        if (count > 0) {
            return null;
        }

        BkgSlotProvisionTemplate entity = new BkgSlotProvisionTemplate();
        BeanUtil.copyProperties(addDTO, entity);

        // 时段列表 → JSON 字符串
        try {
            entity.setSlotTemplatesJson(objectMapper.writeValueAsString(addDTO.getSlotTemplates()));
        } catch (Exception e) {
            log.error("序列化时段列表JSON失败", e);
            return null;
        }

        // 新增时默认未删除、版本号为0
        entity.setIsDeleted(false);
        entity.setVersion(0);
        if (entity.getEnabled() == null) {
            entity.setEnabled(true);
        }

        this.save(entity);
        return entity.getId();
    }

    @Override
    public boolean modifySlotProvisionTemplate(SlotProvisionTemplateModifyDTO modifyDTO) {
        // 校验记录是否存在且未删除
        BkgSlotProvisionTemplate existing = this.getById(modifyDTO.getId());
        if (existing == null) {
            return false;
        }

        // 修改时校验编码唯一性（排除自身）
        if (StringUtils.hasText(modifyDTO.getTemplateCode())
                && !modifyDTO.getTemplateCode().equals(existing.getTemplateCode())) {
            LambdaQueryWrapper<BkgSlotProvisionTemplate> codeWrapper = new LambdaQueryWrapper<>();
            codeWrapper.eq(BkgSlotProvisionTemplate::getTemplateCode, modifyDTO.getTemplateCode())
                    .ne(BkgSlotProvisionTemplate::getId, modifyDTO.getId());
            if (this.count(codeWrapper) > 0) {
                return false;
            }
        }

        BkgSlotProvisionTemplate entity = new BkgSlotProvisionTemplate();
        BeanUtil.copyProperties(modifyDTO, entity);
        // 保留原有版本号用于乐观锁
        entity.setVersion(existing.getVersion());

        // 时段列表 → JSON 字符串（允许部分更新：不传时段列表则保留原值）
        if (modifyDTO.getSlotTemplates() != null) {
            try {
                entity.setSlotTemplatesJson(objectMapper.writeValueAsString(modifyDTO.getSlotTemplates()));
            } catch (Exception e) {
                log.error("序列化时段列表JSON失败", e);
                return false;
            }
        }

        return this.updateById(entity);
    }

    @Override
    public boolean removeSlotProvisionTemplate(String id) {
        // 校验记录是否存在且未删除
        BkgSlotProvisionTemplate existing = this.getById(id);
        if (existing == null) {
            return false;
        }

        // 软删除：手动设置删除审计字段（@TableLogic 的 removeById 不会填充 deletedAt/deletedBy）
        LambdaUpdateWrapper<BkgSlotProvisionTemplate> updateWrapper = new LambdaUpdateWrapper<>();
        updateWrapper.eq(BkgSlotProvisionTemplate::getId, id)
                .set(BkgSlotProvisionTemplate::getIsDeleted, true)
                .set(BkgSlotProvisionTemplate::getDeletedAt, LocalDateTime.now());
        try {
            String username = userHolder.getCurrentUser().getUsername();
            updateWrapper.set(BkgSlotProvisionTemplate::getDeletedBy, username);
        } catch (Exception e) {
            // 无用户上下文时跳过 deletedBy
        }

        return this.update(updateWrapper);
    }
}
