package com.zeroone.star.sysmgr.service.impl;

import cn.hutool.core.util.StrUtil;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.doo.WhaleSettingsDO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.sysmgr.settingdto.SettingDefinitionDTO;
import com.zeroone.star.project.dto.j2.sysmgr.settingdto.SettingsAddDTO;
import com.zeroone.star.project.dto.j2.sysmgr.settingdto.SettingsDTO;
import com.zeroone.star.project.query.j2.sysmgr.settings.SettingDefinitionQuery;
import com.zeroone.star.project.query.j2.sysmgr.settings.SettingsQuery;
import com.zeroone.star.sysmgr.enums.SettingDefinitionEnum;
import com.zeroone.star.sysmgr.mapper.SettingsMapper;
import com.zeroone.star.sysmgr.service.SettingsService;
import jakarta.annotation.Resource;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.time.LocalDateTime;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.stream.Collectors;

/**
 * <p>
 * 描述：系统配置项服务实现
 * </p>
 *
 * @author L
 * @version 1.0.0
 */
@Slf4j
@Service
public class SettingsServiceImpl implements SettingsService {

    @Resource
    private SettingsMapper settingsMapper;

    @Resource
    private UserHolder userHolder;

    // ==================== 获取配置定义列表 ====================

    @Override
    public PageDTO<SettingDefinitionDTO> listDefinitions(SettingDefinitionQuery query) {
        AtomicInteger counter = new AtomicInteger(0);
        List<SettingDefinitionDTO> rows = Arrays.stream(SettingDefinitionEnum.values())
                .filter(e -> {
                    if (StrUtil.isBlank(query.getSettingDefinitionKeywords())) {
                        return true;
                    }
                    String kw = query.getSettingDefinitionKeywords().toLowerCase();
                    return e.getName().toLowerCase().contains(kw)
                            || e.getTitle().toLowerCase().contains(kw);
                })
                .map(e -> {
                    SettingDefinitionDTO dto = new SettingDefinitionDTO();
                    dto.setId(String.valueOf(counter.incrementAndGet()));
                    dto.setName(e.getName());
                    dto.setDescription(e.getTitle());
                    return dto;
                })
                .collect(Collectors.toList());

        PageDTO<SettingDefinitionDTO> result = new PageDTO<>();
        result.setPageIndex(1L);
        result.setPageSize((long) rows.size());
        result.setTotal((long) rows.size());
        result.setPages(rows.isEmpty() ? 0L : 1L);
        result.setRows(rows);
        return result;
    }

    // ==================== 获取配置项列表 ====================

    @Override
    public PageDTO<SettingsDTO> listSettings(SettingsQuery query) {
        String tenantId = resolveTenantId();
        int pageIndex = query.getPageIndex() > 0 ? query.getPageIndex() : 1;
        int pageSize = query.getPageSize() > 0 ? query.getPageSize() : 10;

        Page<WhaleSettingsDO> page = new Page<>(pageIndex, pageSize);

        Page<WhaleSettingsDO> result;
        if (hasCondition(query)) {
            result = settingsMapper.queryByCondition(page, query, tenantId);
        } else {
            result = settingsMapper.queryAll(page, tenantId);
        }

        List<SettingsDTO> dtoList = result.getRecords().stream()
                .map(doObj -> {
                    SettingsDTO dto = new SettingsDTO();
                    dto.setValue(doObj.getValue());
                    dto.setProvider_name(doObj.getProviderName());
                    dto.setProvider_key(doObj.getProviderKey());
                    return dto;
                })
                .collect(Collectors.toList());

        PageDTO<SettingsDTO> pageDTO = new PageDTO<>();
        pageDTO.setPageIndex(result.getCurrent());
        pageDTO.setPageSize(result.getSize());
        pageDTO.setTotal(result.getTotal());
        pageDTO.setPages(result.getPages());
        pageDTO.setRows(dtoList);
        return pageDTO;
    }

    // ==================== 新增配置项 ====================

    @Override
    @Transactional(rollbackFor = Exception.class)
    public String addSetting(SettingsAddDTO dto) {
        String tenantId = resolveTenantId();
        UserDTO currentUser;
        try {
            currentUser = userHolder.getCurrentUser();
        } catch (Exception e) {
            log.error("获取当前用户失败", e);
            return "获取当前用户失败";
        }

        // 校验是否在配置定义中存在
        String title = findTitleByName(dto.getName());
        if (title == null) {
            log.warn("不受支持的配置项: name={}", dto.getName());
            return "配置项 [" + dto.getName() + "] 不在配置定义中，不被支持";
        }

        // 校验是否已存在
        if (settingsMapper.countByNameAndTenant(dto.getName(), tenantId) > 0) {
            log.warn("配置项已存在: name={}, tenantId={}", dto.getName(), tenantId);
            return "配置项 [" + dto.getName() + "] 已存在";
        }

        WhaleSettingsDO doObj = new WhaleSettingsDO();
        doObj.setName(dto.getName());
        doObj.setValue(dto.getValue());
        doObj.setProviderName(dto.getProvider_name());
        doObj.setProviderKey(dto.getProvider_key());
        doObj.setTenantId(tenantId);
        doObj.setDescription(title);
        doObj.setCreatedAt(LocalDateTime.now());
        doObj.setCreatedBy(currentUser.getId());
        doObj.setIsDeleted(0);

        if (settingsMapper.insert(doObj) != 1) {
            log.error("新增配置项失败: name={}, tenantId={}", dto.getName(), tenantId);
            return "新增配置项失败";
        }
        log.info("新增配置项成功: name={}, tenantId={}", dto.getName(), tenantId);
        return null;
    }

    // ==================== 内部工具方法 ====================

    /**
     * 解析当前租户ID
     */
    private String resolveTenantId() {
        return "1";
    }

    /**
     * 通过 name 查找枚举定义的标题
     */
    private String findTitleByName(String name) {
        if (StrUtil.isBlank(name)) {
            return null;
        }
        return Arrays.stream(SettingDefinitionEnum.values())
                .filter(e -> e.getName().equals(name))
                .findFirst()
                .map(SettingDefinitionEnum::getTitle)
                .orElse(null);
    }

    /**
     * 判断查询条件是否为空
     */
    private boolean hasCondition(SettingsQuery query) {
        return StrUtil.isNotBlank(query.getProvider_name())
                || StrUtil.isNotBlank(query.getProvider_key())
                || StrUtil.isNotBlank(query.getValue());
    }
}
