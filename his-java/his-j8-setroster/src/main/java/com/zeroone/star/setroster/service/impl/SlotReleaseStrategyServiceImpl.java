package com.zeroone.star.setroster.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.toolkit.IdWorker;
import com.baomidou.mybatisplus.core.toolkit.Wrappers;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyAddDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyModifyDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyNameDTO;
import com.zeroone.star.project.query.j8.setroster.SlotReleaseStrategyQuery;
import com.zeroone.star.setroster.entity.BkgReleasePolicy;
import com.zeroone.star.setroster.mapper.BkgReleasePolicyMapper;
import com.zeroone.star.setroster.service.ISlotReleaseStrategyService;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

import java.time.LocalDateTime;
import java.util.List;
import java.util.Map;
import java.util.UUID;

/**
 * 放号策略业务服务实现。
 */
@Service
@RequiredArgsConstructor
public class SlotReleaseStrategyServiceImpl implements ISlotReleaseStrategyService {

    private static final int POLICY_CODE_RANDOM_LENGTH = 12;
    private static final int POLICY_CODE_MAX_RETRIES = 5;

    private final BkgReleasePolicyMapper releasePolicyMapper;
    private final ObjectMapper objectMapper;
    private final UserHolder userHolder;

    @Override
    public PageDTO<SlotReleaseStrategyDTO> listStrategies(SlotReleaseStrategyQuery query) {
        Page<BkgReleasePolicy> page = new Page<>(query.getPageIndex(), query.getPageSize());
        LambdaQueryWrapper<BkgReleasePolicy> wrapper = Wrappers.lambdaQuery(BkgReleasePolicy.class)
                .select(
                        BkgReleasePolicy::getId,
                        BkgReleasePolicy::getPolicyCode,
                        BkgReleasePolicy::getPolicyName,
                        BkgReleasePolicy::getAdvanceDays,
                        BkgReleasePolicy::getSlotMinutes,
                        BkgReleasePolicy::getCapacityPerSlot,
                        BkgReleasePolicy::getChannelQuotaJson)
                .and(StringUtils.hasText(query.getKeyword()), condition -> condition
                        .like(BkgReleasePolicy::getPolicyCode, query.getKeyword().trim())
                        .or()
                        .like(BkgReleasePolicy::getPolicyName, query.getKeyword().trim()))
                .orderByDesc(BkgReleasePolicy::getCreatedAt);

        Page<BkgReleasePolicy> result = releasePolicyMapper.selectPage(page, wrapper);
        PageDTO<SlotReleaseStrategyDTO> pageDTO = PageDTO.create(result, this::toListDTO);
        if (pageDTO.getRows() == null) {
            pageDTO.setRows(List.of());
        }
        return pageDTO;
    }

    @Override
    public List<SlotReleaseStrategyNameDTO> listStrategyNames() {
        LambdaQueryWrapper<BkgReleasePolicy> wrapper = Wrappers.lambdaQuery(BkgReleasePolicy.class)
                .select(BkgReleasePolicy::getId, BkgReleasePolicy::getPolicyName)
                .orderByAsc(BkgReleasePolicy::getPolicyName);
        return releasePolicyMapper.selectList(wrapper).stream()
                .map(this::toNameDTO)
                .toList();
    }

    @Override
    public String saveStrategy(SlotReleaseStrategyAddDTO addDTO) {
        BkgReleasePolicy policy = new BkgReleasePolicy();
        policy.setId(IdWorker.getIdStr());
        policy.setPolicyCode(generatePolicyCode());
        copyEditableFields(addDTO, policy);
        policy.setIsDeleted(0);
        policy.setVersion(0);

        if (releasePolicyMapper.insert(policy) != 1) {
            throw new IllegalStateException("新增放号策略失败");
        }
        return policy.getId();
    }

    @Override
    public boolean updateStrategy(SlotReleaseStrategyModifyDTO modifyDTO) {
        BkgReleasePolicy policy = releasePolicyMapper.selectById(modifyDTO.getId());
        if (policy == null) {
            return false;
        }
        copyEditableFields(modifyDTO, policy);
        return releasePolicyMapper.updateById(policy) == 1;
    }

    @Override
    public boolean removeStrategy(String id) {
        BkgReleasePolicy policy = releasePolicyMapper.selectById(id);
        if (policy == null) {
            return false;
        }
        return releasePolicyMapper.logicallyDelete(
                id,
                policy.getVersion(),
                LocalDateTime.now(),
                currentUserId()) == 1;
    }

    private void copyEditableFields(SlotReleaseStrategyAddDTO source, BkgReleasePolicy target) {
        target.setPolicyName(source.getStrategyName().trim());
        target.setAdvanceDays(source.getAdvanceDays());
        target.setSlotMinutes(source.getSlotDurationMinutes());
        target.setCapacityPerSlot(source.getSlotCapacity());
        target.setChannelQuotaJson(writeChannelQuota(source.getChannelQuota()));
    }

    private SlotReleaseStrategyDTO toListDTO(BkgReleasePolicy policy) {
        SlotReleaseStrategyDTO dto = new SlotReleaseStrategyDTO();
        dto.setId(policy.getId());
        dto.setStrategyCode(policy.getPolicyCode());
        dto.setStrategyName(policy.getPolicyName());
        dto.setAdvanceDays(policy.getAdvanceDays());
        dto.setSlotDurationMinutes(policy.getSlotMinutes());
        dto.setSlotCapacity(policy.getCapacityPerSlot());
        dto.setChannelQuota(readChannelQuota(policy.getChannelQuotaJson()));
        return dto;
    }

    private SlotReleaseStrategyNameDTO toNameDTO(BkgReleasePolicy policy) {
        SlotReleaseStrategyNameDTO dto = new SlotReleaseStrategyNameDTO();
        dto.setId(policy.getId());
        dto.setStrategyName(policy.getPolicyName());
        return dto;
    }

    private String writeChannelQuota(Map<String, Integer> channelQuota) {
        if (channelQuota == null || channelQuota.isEmpty()) {
            return null;
        }
        try {
            return objectMapper.writeValueAsString(channelQuota);
        } catch (JsonProcessingException e) {
            throw new IllegalArgumentException("渠道配额不是有效的 JSON 数据", e);
        }
    }

    private Map<String, Integer> readChannelQuota(String channelQuotaJson) {
        if (!StringUtils.hasText(channelQuotaJson)) {
            return Map.of();
        }
        try {
            return objectMapper.readValue(channelQuotaJson, new TypeReference<>() {
            });
        } catch (JsonProcessingException e) {
            throw new IllegalStateException("数据库中的渠道配额 JSON 格式错误", e);
        }
    }

    private String generatePolicyCode() {
        for (int i = 0; i < POLICY_CODE_MAX_RETRIES; i++) {
            String randomPart = UUID.randomUUID().toString().replace("-", "")
                    .substring(0, POLICY_CODE_RANDOM_LENGTH);
            String policyCode = "RP-" + randomPart;
            Long count = releasePolicyMapper.selectCount(
                    Wrappers.lambdaQuery(BkgReleasePolicy.class)
                            .eq(BkgReleasePolicy::getPolicyCode, policyCode));
            if (count == 0) {
                return policyCode;
            }
        }
        throw new IllegalStateException("生成放号策略编码失败，请重试");
    }

    private String currentUserId() {
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            if (currentUser != null && StringUtils.hasText(currentUser.getId())) {
                return currentUser.getId();
            }
        } catch (Exception ignored) {
            // 本地无网关请求头时使用系统用户，保证逻辑删除仍可执行。
        }
        return "system";
    }
}
