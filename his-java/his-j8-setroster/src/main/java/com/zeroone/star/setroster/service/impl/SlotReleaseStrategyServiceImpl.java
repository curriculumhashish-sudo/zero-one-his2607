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
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyAddDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyIdVO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyModifyDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyPageVO;
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
    public SlotReleaseStrategyPageVO<SlotReleaseStrategyDTO> listStrategies(SlotReleaseStrategyQuery query) {
        Page<BkgReleasePolicy> page = new Page<>(query.getPageIndex(), query.getPageSize());
        LambdaQueryWrapper<BkgReleasePolicy> wrapper = buildQueryWrapper(query);
        Page<BkgReleasePolicy> result = releasePolicyMapper.selectPage(page, wrapper);
        return toPageVO(result);
    }

    @Override
    public SlotReleaseStrategyPageVO<SlotReleaseStrategyDTO> listStrategyNames(SlotReleaseStrategyQuery query) {
        Page<BkgReleasePolicy> page = new Page<>(query.getPageIndex(), query.getPageSize());
        LambdaQueryWrapper<BkgReleasePolicy> wrapper = buildQueryWrapper(query);
        Page<BkgReleasePolicy> result = releasePolicyMapper.selectPage(page, wrapper);
        return toPageVO(result);
    }

    @Override
    public SlotReleaseStrategyIdVO saveStrategy(SlotReleaseStrategyAddDTO addDTO) {
        BkgReleasePolicy policy = new BkgReleasePolicy();
        policy.setId(IdWorker.getIdStr());
        policy.setPolicyCode(generatePolicyCode());
        copyEditableFields(addDTO, policy);
        LocalDateTime now = LocalDateTime.now();
        policy.setCreateTime(now);
        policy.setUpdateTime(now);
        String userId = currentUserId();
        policy.setCreatedBy(userId);
        policy.setUpdatedBy(userId);
        policy.setIsDeleted(0);
        policy.setVersion(0);

        if (existsByPolicyName(policy.getPolicyName(), null)) {
            throw new IllegalArgumentException("策略名称已存在：" + policy.getPolicyName());
        }
        if (releasePolicyMapper.insert(policy) != 1) {
            throw new IllegalStateException("新建放号策略失败");
        }
        return new SlotReleaseStrategyIdVO(policy.getId());
    }

    @Override
    public SlotReleaseStrategyIdVO updateStrategy(SlotReleaseStrategyModifyDTO modifyDTO) {
        BkgReleasePolicy policy = releasePolicyMapper.selectById(modifyDTO.getId());
        if (policy == null) {
            throw new IllegalArgumentException("更新失败：策略不存在");
        }
        copyEditableFields(modifyDTO, policy);
        policy.setUpdateTime(LocalDateTime.now());
        policy.setUpdatedBy(currentUserId());
        if (existsByPolicyName(policy.getPolicyName(), policy.getId())) {
            throw new IllegalArgumentException("策略名称已存在：" + policy.getPolicyName());
        }
        if (releasePolicyMapper.updateById(policy) != 1) {
            throw new IllegalStateException("修改放号策略失败");
        }
        return new SlotReleaseStrategyIdVO(policy.getId());
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

    private LambdaQueryWrapper<BkgReleasePolicy> buildQueryWrapper(SlotReleaseStrategyQuery query) {
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
                .orderByDesc(BkgReleasePolicy::getCreateTime);

        if (StringUtils.hasText(query.getOrganizationId())) {
            wrapper.eq(BkgReleasePolicy::getOrganizationId, query.getOrganizationId().trim());
        }
        return wrapper;
    }

    private SlotReleaseStrategyPageVO<SlotReleaseStrategyDTO> toPageVO(Page<BkgReleasePolicy> result) {
        List<SlotReleaseStrategyDTO> content = result.getRecords().stream()
                .map(this::toListDTO)
                .toList();
        SlotReleaseStrategyPageVO<SlotReleaseStrategyDTO> pageVO = new SlotReleaseStrategyPageVO<>();
        pageVO.setContent(content);
        pageVO.setTotalCount(result.getTotal());
        pageVO.setPageIndex(result.getCurrent());
        pageVO.setPageSize(result.getSize());
        return pageVO;
    }

    private void copyEditableFields(SlotReleaseStrategyAddDTO source, BkgReleasePolicy target) {
        target.setPolicyName(source.getPolicyName().trim());
        target.setAdvanceDays(source.getAdvanceDays());
        target.setSlotMinutes(source.getSlotMinutes());
        target.setCapacityPerSlot(source.getCapacityPerSlot());
        target.setChannelQuotaJson(writeChannelQuota(source.getChannelQuotaJson()));
    }

    private SlotReleaseStrategyDTO toListDTO(BkgReleasePolicy policy) {
        SlotReleaseStrategyDTO dto = new SlotReleaseStrategyDTO();
        dto.setId(policy.getId());
        dto.setPolicyCode(policy.getPolicyCode());
        dto.setPolicyName(policy.getPolicyName());
        dto.setAdvanceDays(policy.getAdvanceDays());
        dto.setSlotMinutes(policy.getSlotMinutes());
        dto.setCapacityPerSlot(policy.getCapacityPerSlot());
        dto.setChannelQuotas(readChannelQuota(policy.getChannelQuotaJson()));
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

    private boolean existsByPolicyName(String policyName, String excludeId) {
        LambdaQueryWrapper<BkgReleasePolicy> wrapper = Wrappers.lambdaQuery(BkgReleasePolicy.class)
                .eq(BkgReleasePolicy::getPolicyName, policyName);
        if (StringUtils.hasText(excludeId)) {
            wrapper.ne(BkgReleasePolicy::getId, excludeId);
        }
        return releasePolicyMapper.selectCount(wrapper) > 0;
    }
}