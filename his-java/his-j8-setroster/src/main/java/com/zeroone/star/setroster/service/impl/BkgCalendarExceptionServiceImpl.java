package com.zeroone.star.setroster.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.toolkit.IdWorker;
import com.baomidou.mybatisplus.core.toolkit.Wrappers;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.j8.setroster.CalendarExceptionAddDto;
import com.zeroone.star.project.dto.j8.setroster.CalendarExceptionUpdateDto;
import com.zeroone.star.project.query.j8.setroster.CalendarExceptionQuery;
import com.zeroone.star.project.vo.j8.setroster.CalendarExceptionVo;
import com.zeroone.star.setroster.entity.BkgCalendarException;
import com.zeroone.star.setroster.mapper.BkgCalendarExceptionMapper;
import com.zeroone.star.setroster.service.IBkgCalendarExceptionService;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.util.List;
import java.util.Set;

/**
 * <p>
 * T3 日历例外（07 §5.4）业务服务实现
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author lolololo
 * @version 1.0.0
 */
@Service
@RequiredArgsConstructor
public class BkgCalendarExceptionServiceImpl implements IBkgCalendarExceptionService {

    /**
     * 合法的例外类型枚举值
     */
    private static final Set<String> VALID_EXCEPTION_TYPES = Set.of("CLOSED", "REDUCED", "OVERRIDE", "EXTRA");

    private final BkgCalendarExceptionMapper mapper;
    private final UserHolder userHolder;

    @Override
    public List<CalendarExceptionVo> queryCalendarExceptions(CalendarExceptionQuery query) {
        LambdaQueryWrapper<BkgCalendarException> wrapper = Wrappers.lambdaQuery(BkgCalendarException.class)
                .eq(StringUtils.hasText(query.getOrganizationId()),
                        BkgCalendarException::getOrganizationId, query.getOrganizationId())
                .eq(StringUtils.hasText(query.getExceptionType()),
                        BkgCalendarException::getExceptionType, query.getExceptionType())
                .ge(StringUtils.hasText(query.getRangeStart()),
                        BkgCalendarException::getExceptionDate, query.getRangeStart())
                .le(StringUtils.hasText(query.getRangeEnd()),
                        BkgCalendarException::getRangeEnd, query.getRangeEnd())
                .like(StringUtils.hasText(query.getKeyword()),
                        BkgCalendarException::getReason, query.getKeyword())
                .orderByDesc(BkgCalendarException::getExceptionDate);
        return mapper.selectList(wrapper).stream()
                .map(this::toVo)
                .toList();
    }

    @Override
    public String add(CalendarExceptionAddDto dto) {
        validateExceptionType(dto.getExceptionType());
        validateDateRange(dto.getExceptionDate(), dto.getRangeEnd());

        BkgCalendarException entity = new BkgCalendarException();
        entity.setId(IdWorker.getId());
        entity.setExceptionType(dto.getExceptionType());
        entity.setExceptionDate(dto.getExceptionDate());
        entity.setRangeEnd(dto.getRangeEnd());
        entity.setOrganizationId(dto.getOrganizationId());
        entity.setOverrideScheduleTemplateId(dto.getOverrideScheduleTemplateId());
        entity.setReason(dto.getReason());
        entity.setAutoApplyOnGenerate(dto.getAutoApplyOnGenerate() != null
                ? dto.getAutoApplyOnGenerate() : true);
        entity.setIsDeleted(false);
        entity.setVersion(0);
        entity.setCreatedAt(LocalDateTime.now());
        entity.setCreatedBy(currentUserId());

        if (mapper.insert(entity) != 1) {
            throw new IllegalStateException("新增节日例外失败");
        }
        return entity.getId().toString();
    }

    @Override
    public String modify(CalendarExceptionUpdateDto dto) {
        validateExceptionType(dto.getExceptionType());
        validateDateRange(dto.getExceptionDate(), dto.getRangeEnd());

        BkgCalendarException entity = mapper.selectById(Long.parseLong(dto.getId()));
        if (entity == null) {
            throw new IllegalArgumentException("节日例外记录不存在，id=" + dto.getId());
        }

        entity.setExceptionType(dto.getExceptionType());
        entity.setExceptionDate(dto.getExceptionDate());
        entity.setRangeEnd(dto.getRangeEnd());
        entity.setReason(dto.getReason());
        entity.setAutoApplyOnGenerate(dto.getAutoApplyOnGenerate() != null
                ? dto.getAutoApplyOnGenerate() == 1 : entity.getAutoApplyOnGenerate());
        entity.setUpdatedAt(LocalDateTime.now());
        entity.setUpdatedBy(currentUserId());

        if (mapper.updateById(entity) != 1) {
            throw new IllegalStateException("修改节日例外失败，id=" + dto.getId());
        }
        return entity.getId().toString();
    }

    @Override
    public String delete(String id) {
        Long longId = Long.parseLong(id);
        BkgCalendarException entity = mapper.selectById(longId);
        if (entity == null) {
            throw new IllegalArgumentException("节日例外记录不存在，id=" + id);
        }

        if (mapper.deleteById(longId) != 1) {
            throw new IllegalStateException("删除节日例外失败，id=" + id);
        }
        return id;
    }

    /**
     * Entity → VO 转换
     */
    private CalendarExceptionVo toVo(BkgCalendarException entity) {
        CalendarExceptionVo vo = new CalendarExceptionVo();
        vo.setId(entity.getId());
        vo.setOrganizationId(entity.getOrganizationId());
        vo.setExceptionDate(entity.getExceptionDate());
        vo.setRangeEnd(entity.getRangeEnd());
        vo.setExceptionType(entity.getExceptionType());
        vo.setOverrideScheduleTemplateId(entity.getOverrideScheduleTemplateId());
        vo.setReason(entity.getReason());
        vo.setAutoApplyOnGenerate(entity.getAutoApplyOnGenerate());
        return vo;
    }

    /**
     * 校验例外类型是否为合法枚举值
     */
    private void validateExceptionType(String exceptionType) {
        if (!VALID_EXCEPTION_TYPES.contains(exceptionType)) {
            throw new IllegalArgumentException(
                    "例外类型不合法：" + exceptionType + "，有效值：" + VALID_EXCEPTION_TYPES);
        }
    }

    /**
     * 校验日期区间：若指定了 rangeEnd，不得早于 exceptionDate
     */
    private void validateDateRange(LocalDate exceptionDate, LocalDate rangeEnd) {
        if (rangeEnd != null && rangeEnd.isBefore(exceptionDate)) {
            throw new IllegalArgumentException(
                    "结束日期不能早于起始日期：exceptionDate=" + exceptionDate + ", rangeEnd=" + rangeEnd);
        }
    }

    /**
     * 获取当前登录用户 ID，无法获取时返回 "system"
     */
    private String currentUserId() {
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            if (currentUser != null && StringUtils.hasText(currentUser.getId())) {
                return currentUser.getId();
            }
        } catch (Exception ignored) {
            // 本地无网关请求头时使用系统用户
        }
        return "system";
    }
}
