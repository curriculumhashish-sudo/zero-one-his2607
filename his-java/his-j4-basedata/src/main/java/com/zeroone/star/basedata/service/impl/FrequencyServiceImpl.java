package com.zeroone.star.basedata.service.impl;

import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.basedata.entity.frequency.CusFrequencyDO;
import com.zeroone.star.basedata.mapper.FrequencyMapper;
import com.zeroone.star.basedata.service.IFrequencyService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.frequency.FrequencySaveDTO;
import com.zeroone.star.project.dto.j4.frequency.FrequencyUpdateDTO;
import com.zeroone.star.project.dto.j4.frequency.FrequencyStatusDTO;
import com.zeroone.star.project.query.j4.frequency.FrequencyPageQuery;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.vo.j4.frequency.FrequencyOptionVO;
import com.zeroone.star.project.vo.j4.frequency.FrequencyVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.time.LocalDateTime;
import java.util.List;
import java.util.Objects;

/**
 * <p>
 * 描述：频次管理服务实现
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Service
public class FrequencyServiceImpl extends ServiceImpl<FrequencyMapper, CusFrequencyDO> implements IFrequencyService {
    @Resource
    private MsFrequencyMapper msFrequencyMapper;

    // UserHolder用于获取当前租户ID
    @Resource
    private UserHolder userHolder;

    @Override
    public PageDTO<FrequencyVO> queryFrequencyPage(FrequencyPageQuery query) {
        // 1. 获取当前租户ID
        String tenantId = userHolder.getCurrentTenantId();
        // 测试用
//        String tenantId = "1";
        // 2. 构建分页对象并按租户查询
        Page<FrequencyVO> page = new Page<>(query.getPageIndex(), query.getPageSize());
        Page<FrequencyVO> result = baseMapper.selectFrequencyPage(page, query, tenantId);
        return PageDTO.create(result);
    }

    @Override
    public FrequencyVO queryFrequencyDetail(String id) {
        // 1. 获取当前租户ID
        String tenantId = userHolder.getCurrentTenantId();
        // 测试用
//        String tenantId = "1";
        return baseMapper.selectFrequencyById(id, tenantId);
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public FrequencyVO addFrequency(FrequencySaveDTO dto) {
        // 1. 获取当前租户ID
        String tenantId = userHolder.getCurrentTenantId();
        // 测试用
//        String tenantId = "1";
        // 2. 校验同租户内频次代号唯一
        checkRateCodeUnique(dto.getRateCode(), null, tenantId);
        // 3. DTO转实体并绑定租户
        CusFrequencyDO entity = msFrequencyMapper.saveDtoToEntity(dto);
        entity.setTenantId(tenantId);
        // 4. 保存到数据库
        baseMapper.insert(entity);
        // 5. 实体转VO返回
        return msFrequencyMapper.entityToVo(entity);
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public FrequencyVO modifyFrequency(FrequencyUpdateDTO dto) {
        // 1. 获取当前租户ID并校验记录归属
        String tenantId = userHolder.getCurrentTenantId();
        // 测试用
//        String tenantId = "1";
        CusFrequencyDO existing = baseMapper.selectById(dto.getId());
        if (existing == null || !tenantId.equals(existing.getTenantId())) {
            throw new RuntimeException("频次记录不存在");
        }
        // 2. 校验同租户内频次代号唯一（排除自身）
        checkRateCodeUnique(dto.getRateCode(), dto.getId(), tenantId);
        // 3. DTO转实体
        CusFrequencyDO entity = msFrequencyMapper.updateDtoToEntity(dto);
        normalizeCycleFields(entity);
        // 4. 保留系统字段与租户标识
        entity.setTenantId(existing.getTenantId());
        entity.setCreatedAt(existing.getCreatedAt());
        entity.setCreatedBy(existing.getCreatedBy());
        entity.setIsDeleted(existing.getIsDeleted());
        entity.setDeletedAt(existing.getDeletedAt());
        entity.setDeletedBy(existing.getDeletedBy());
        entity.setVersion(existing.getVersion());
        // 5. 更新数据库（乐观锁：version 不匹配时影响0行）
        int updated = baseMapper.updateById(entity);
        if (updated == 0) {
            // 区分并发冲突与"值未变化"：仅当 version 已变化时才判定为并发冲突
            CusFrequencyDO latest = baseMapper.selectById(dto.getId());
            if (latest == null || !Objects.equals(latest.getVersion(), existing.getVersion())) {
                throw new RuntimeException("数据已被他人修改，请刷新后重试");
            }
        }
        // 6. 查询最新数据并返回（限定当前租户）
        return baseMapper.selectFrequencyById(dto.getId(), tenantId);
    }

    /**
     * 根据周期类型清空不适用的执行规则字段，防止周期切换后残留旧类型数据。
     *
     * @param entity 待更新的频次实体
     */
    static void normalizeCycleFields(CusFrequencyDO entity) {
        if (entity == null || entity.getCycleType() == null) {
            return;
        }

        String dayTimes = entity.getDayTimes();
        Integer dayInterval = entity.getDayInterval();
        Integer dayCount = entity.getDayCount();
        Byte weekCycleFlag = entity.getWeekCycleFlag();
        Integer executionPeriod = entity.getExecutionPeriod();
        String executionPeriodUnit = entity.getExecutionPeriodUnit();
        Integer totalExecutionCount = entity.getTotalExecutionCount();
        String allowCrossDay = entity.getAllowCrossDay();

        clearCycleFields(entity);
        switch (entity.getCycleType()) {
            case "daily" -> {
                entity.setDayTimes(dayTimes);
                entity.setDayInterval(dayInterval);
                entity.setDayCount(dayCount);
                entity.setAllowCrossDay(allowCrossDay);
            }
            case "weekly" -> {
                entity.setDayTimes(dayTimes);
                entity.setWeekCycleFlag(weekCycleFlag);
                entity.setAllowCrossDay(allowCrossDay);
            }
            case "interval" -> {
                entity.setDayInterval(dayInterval);
                entity.setExecutionPeriod(executionPeriod);
                entity.setExecutionPeriodUnit(executionPeriodUnit);
                entity.setAllowCrossDay(allowCrossDay);
            }
            case "as-needed" -> entity.setDayTimes(dayTimes);
            case "fixed-count" -> {
                entity.setDayTimes(dayTimes);
                entity.setTotalExecutionCount(totalExecutionCount);
            }
            default -> {
                // 未支持的周期类型不保留任何周期条件，避免形成互相矛盾的规则数据。
            }
        }
    }

    private static void clearCycleFields(CusFrequencyDO entity) {
        entity.setDayTimes(null);
        entity.setDayInterval(null);
        entity.setDayCount(null);
        entity.setWeekCycleFlag(null);
        entity.setExecutionPeriod(null);
        entity.setExecutionPeriodUnit(null);
        entity.setTotalExecutionCount(null);
        entity.setAllowCrossDay(null);
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public void removeFrequency(String id) {
        // 1. 获取当前租户ID
        String tenantId = userHolder.getCurrentTenantId();
        // 2. 带租户条件的逻辑删除（@TableLogic 自动转为 update is_deleted = 1），并填充删除审计信息
        boolean removed = lambdaUpdate()
                .eq(CusFrequencyDO::getId, id)
                .eq(CusFrequencyDO::getTenantId, tenantId)
                .set(CusFrequencyDO::getDeletedAt, LocalDateTime.now())
                .set(CusFrequencyDO::getDeletedBy, currentUsername())
                .remove();
        if (!removed) {
            throw new RuntimeException("频次记录不存在");
        }
    }

    @Override
    public List<FrequencyOptionVO> listFrequencyNames() {
        // 1. 获取当前租户ID
        String tenantId = userHolder.getCurrentTenantId();
        return baseMapper.selectActiveFrequencyOptions(tenantId);
    }

    @Override
    public boolean modifyFrequencyStatus(FrequencyStatusDTO dto) {
        // 1. 获取当前租户ID并校验记录归属
        String tenantId = userHolder.getCurrentTenantId();
        CusFrequencyDO existing = baseMapper.selectById(dto.getId());
        if (existing == null || !tenantId.equals(existing.getTenantId())) {
            return false;
        }
        // 2. 状态未变化时直接返回成功（幂等）
        if (existing.getActiveFlag() != null && existing.getActiveFlag().equals(dto.getActiveFlag())) {
            return true;
        }
        // 3. 更新状态（限定当前租户）
        return lambdaUpdate()
                .eq(CusFrequencyDO::getId, dto.getId())
                .eq(CusFrequencyDO::getTenantId, tenantId)
                .set(CusFrequencyDO::getActiveFlag, dto.getActiveFlag())
                .update();
    }

    /**
     * 校验同租户内频次代号是否唯一
     * @param rateCode  频次代号
     * @param excludeId 需要排除的主键ID（新增传null，修改传当前ID）
     * @param tenantId  当前租户ID
     */
    private void checkRateCodeUnique(String rateCode, String excludeId, String tenantId) {
        Long count = lambdaQuery()
                .eq(CusFrequencyDO::getTenantId, tenantId)
                .eq(CusFrequencyDO::getRateCode, rateCode)
                .ne(excludeId != null, CusFrequencyDO::getId, excludeId)
                .count();
        if (count != null && count > 0) {
            throw new RuntimeException("频次代号 [" + rateCode + "] 已存在");
        }
    }

    /**
     * 获取当前登录用户名，获取失败时回退为 system
     * @return 用户名
     */
    private String currentUsername() {
        try {
            return userHolder.getCurrentUser() != null
                    ? userHolder.getCurrentUser().getUsername()
                    : "system";
        } catch (Exception e) {
            return "system";
        }
    }
}
