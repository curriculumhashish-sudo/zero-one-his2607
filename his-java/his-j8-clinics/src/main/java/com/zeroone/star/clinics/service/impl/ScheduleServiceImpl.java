package com.zeroone.star.clinics.service.impl;

import cn.hutool.core.util.IdUtil;
import com.zeroone.star.clinics.entity.Schedule;
import com.zeroone.star.clinics.entity.ScheduleActor;
import com.zeroone.star.clinics.mapper.ScheduleMapper;
import com.zeroone.star.clinics.service.ScheduleService;
import com.zeroone.star.project.dto.j8.clinics.ScheduleActorAddDTO;
import com.zeroone.star.project.dto.j8.clinics.ScheduleAddDTO;
import com.zeroone.star.project.query.j8.clinics.ScheduleListQuery;
import com.zeroone.star.project.vo.j8.clinics.*;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.time.DayOfWeek;
import java.time.LocalDate;
import java.time.YearMonth;
import java.time.format.DateTimeFormatter;
import java.time.temporal.ChronoUnit;
import java.util.*;
import java.util.stream.Collectors;

@Service
public class ScheduleServiceImpl implements ScheduleService {

    @Autowired
    private ScheduleMapper scheduleMapper;

    @Override
    public ScheduleListVO listQuery(ScheduleListQuery query) {
        // 1. 查询日程数据
        List<Schedule> schedules = scheduleMapper.selectByConditions(query);

        // 2. 批量查询参与者
        List<String> scheduleIds = schedules.stream()
                .map(Schedule::getId)
                .collect(Collectors.toList());
        Map<String, List<ScheduleActor>> actorMap = Collections.emptyMap();
        if (!scheduleIds.isEmpty()) {
            List<ScheduleActor> allActors = scheduleMapper.selectActorsByScheduleIds(scheduleIds);
            actorMap = allActors.stream()
                    .collect(Collectors.groupingBy(ScheduleActor::getScheduleId));
        }

        // 3. 确定日历网格范围
        YearMonth yearMonth = parseYearMonth(query.getYearMonth());
        LocalDate monthStart = yearMonth.atDay(1);
        LocalDate monthEnd = yearMonth.atEndOfMonth();
        LocalDate gridStart = monthStart.with(DayOfWeek.MONDAY);
        LocalDate gridEnd = monthEnd.with(DayOfWeek.SUNDAY);

        // 4. 按日期分组日程
        List<ScheduleDayVO> days = new ArrayList<>();
        Set<Long> activeDaySet = new HashSet<>();
        Set<Long> draftDaySet = new HashSet<>();
        Set<Long> inactiveDaySet = new HashSet<>();

        for (LocalDate date = gridStart; !date.isAfter(gridEnd); date = date.plusDays(1)) {
            ScheduleDayVO dayVO = new ScheduleDayVO();
            dayVO.setDate(date);
            dayVO.setInMonth(!date.isBefore(monthStart) && !date.isAfter(monthEnd));

            List<ScheduleItemVO> daySchedules = new ArrayList<>();
            int activeCount = 0;
            int draftCount = 0;
            int inactiveCount = 0;

            for (Schedule s : schedules) {
                LocalDate scheduleStart = toLocalDate(s.getPlanningHorizonStart());
                LocalDate scheduleEnd = toLocalDate(s.getPlanningHorizonEnd());
                if (scheduleStart == null || scheduleEnd == null) continue;
                if (date.isBefore(scheduleStart) || date.isAfter(scheduleEnd)) continue;

                ScheduleItemVO item = toItemVO(s, actorMap);
                daySchedules.add(item);

                if (isActive(s)) {
                    activeCount++;
                } else if (isDraft(s)) {
                    draftCount++;
                } else {
                    inactiveCount++;
                }
            }

            if (activeCount > 0) activeDaySet.add(date.toEpochDay());
            if (draftCount > 0) draftDaySet.add(date.toEpochDay());
            if (inactiveCount > 0 && activeCount == 0 && draftCount == 0) inactiveDaySet.add(date.toEpochDay());

            dayVO.setActiveCount(activeCount);
            dayVO.setDraftCount(draftCount);
            dayVO.setInactiveCount(inactiveCount);
            dayVO.setSchedules(daySchedules);
            days.add(dayVO);
        }

        // 5. 汇总
        ScheduleSummaryVO summary = new ScheduleSummaryVO();
        summary.setPublishedDayCount(activeDaySet.size());
        summary.setDraftDayCount(draftDaySet.size());
        summary.setInactiveDayCount(inactiveDaySet.size());

        long totalGridDays = ChronoUnit.DAYS.between(gridStart, gridEnd) + 1;
        long scheduledDays = activeDaySet.size() + draftDaySet.size() + inactiveDaySet.size();
        summary.setUnscheduledDayCount((int) (totalGridDays - scheduledDays));
        summary.setBookedAppointmentCount(0);
        summary.setTotalScheduleCount(schedules.size());

        // 6. 组装返回
        ScheduleListVO result = new ScheduleListVO();
        result.setDays(days);
        result.setYearMonth(yearMonth.format(DateTimeFormatter.ofPattern("yyyy-MM")));
        result.setMonthStart(monthStart);
        result.setMonthEnd(monthEnd);
        result.setGridStart(gridStart);
        result.setGridEnd(gridEnd);
        result.setSummary(summary);

        return result;
    }

    @Override
    public ScheduleItemVO getById(String scheduleId) {
        Schedule s = scheduleMapper.getById(scheduleId);
        if (s == null) {
            return null;
        }

        List<ScheduleActor> actors = scheduleMapper.selectActorsByScheduleId(scheduleId);

        ScheduleItemVO vo = new ScheduleItemVO();
        vo.setScheduleId(s.getId());
        vo.setName(s.getName());
        vo.setStatusEnum(s.getStatusEnum());
        vo.setPlanningHorizonStart(s.getPlanningHorizonStart());
        vo.setPlanningHorizonEnd(s.getPlanningHorizonEnd());
        vo.setHealthcareServiceId(s.getHealthcareServiceId());
        vo.setSourceScheduleTemplateId(s.getSourceScheduleTemplateId());
        vo.setSlotProvisionTemplateId(s.getSlotProvisionTemplateId());
        vo.setSubstitutePractitionerId(s.getSubstitutePractitionerId());
        vo.setActors(actors.stream().map(this::toActorVO).collect(Collectors.toList()));

        // 查询医疗服务名称
        if (s.getHealthcareServiceId() != null) {
            vo.setHealthcareServiceName(scheduleMapper.selectHealthcareServiceName(s.getHealthcareServiceId()));
        }

        // 从参与者中提取医师名称和地点名称
        for (ScheduleActor actor : actors) {
            if (actor.getActorId() == null) continue;
            // 尝试从医师表查询
            String practitionerName = scheduleMapper.selectPractitionerName(actor.getActorId());
            if (practitionerName != null) {
                vo.setPractitionerName(practitionerName);
            }
            // 尝试从地点表查询
            String locationName = scheduleMapper.selectLocationName(actor.getActorId());
            if (locationName != null) {
                vo.setLocationName(locationName);
            }
        }

        return vo;
    }

    @Override
    public ScheduleCreateVO insertSchedule(ScheduleAddDTO dto) {
        // 生成日程ID
        String scheduleId = IdUtil.fastSimpleUUID();

        Schedule schedule = new Schedule();
        schedule.setId(scheduleId);
        schedule.setOrganizationId(dto.getOrganizationId());
        schedule.setName(dto.getName());
        schedule.setPlanningHorizonStart(dto.getPlanningHorizonStart());
        schedule.setPlanningHorizonEnd(dto.getPlanningHorizonEnd());
        schedule.setSlotProvisionTemplateId(dto.getSlotProvisionTemplateId());
        schedule.setStatusEnum("DRAFT");
        schedule.setActiveFlag(false);
        schedule.setVersion(0);
        scheduleMapper.insertSchedule(schedule);

        // 插入参与者
        if (dto.getActors() != null) {
            for (ScheduleActorAddDTO actorDto : dto.getActors()) {
                ScheduleActor actor = new ScheduleActor();
                actor.setId(IdUtil.fastSimpleUUID());
                actor.setScheduleId(scheduleId);
                actor.setActorType(actorDto.getActorType() != null ? Integer.valueOf(actorDto.getActorType()) : null);
                actor.setActorId(actorDto.getActorId());
                actor.setSortNo(actorDto.getSortNo() != null ? actorDto.getSortNo() : 0);
                actor.setVersion(0);
                scheduleMapper.insertScheduleActor(actor);
            }
        }

        return new ScheduleCreateVO(scheduleId);
    }

    @Override
    public GenerateRosterVO generateFormalRoster(com.zeroone.star.project.dto.j8.clinics.FormalRosterDTO dto) {
        boolean autoPublish = Boolean.TRUE.equals(dto.getAutoPublish());
        boolean autoProvision = Boolean.TRUE.equals(dto.getAutoProvision());

        List<String> scheduleIds = new ArrayList<>();
        int skippedCount = 0;

        if ("day".equalsIgnoreCase(dto.getTemplateKind())) {
            Map<String, Object> dayTemplate = getDayTemplate(dto.getScheduleTemplateId());
            if (dayTemplate == null) {
                throw new IllegalArgumentException("日排班模板不存在: " + dto.getScheduleTemplateId());
            }
            for (LocalDate date = dto.getRangeStart(); !date.isAfter(dto.getRangeEnd()); date = date.plusDays(1)) {
                String scheduleId = createScheduleFromDayTemplate(dto.getOrganizationId(), dayTemplate, date, autoPublish);
                scheduleIds.add(scheduleId);
            }
        } else if ("week".equalsIgnoreCase(dto.getTemplateKind())) {
            List<Map<String, Object>> weekItems = scheduleMapper.selectWeekItems(dto.getWeekScheduleTemplateId());
            // 构建 weekday → dayTemplateId 映射
            Map<Integer, String> weekdayTemplateMap = new HashMap<>();
            for (Map<String, Object> item : weekItems) {
                Integer weekday = (Integer) item.get("weekday");
                String dayTemplateId = (String) item.get("dayScheduleTemplateId");
                if (weekday != null && dayTemplateId != null) {
                    weekdayTemplateMap.put(weekday, dayTemplateId);
                }
            }
            // 缓存已查询的日模板
            Map<String, Map<String, Object>> dayTemplateCache = new HashMap<>();

            for (LocalDate date = dto.getRangeStart(); !date.isAfter(dto.getRangeEnd()); date = date.plusDays(1)) {
                int weekday = date.getDayOfWeek().getValue(); // 1=Mon .. 7=Sun
                String dayTemplateId = weekdayTemplateMap.get(weekday);
                if (dayTemplateId == null) {
                    skippedCount++;
                    continue;
                }
                Map<String, Object> dayTemplate = dayTemplateCache.computeIfAbsent(
                        dayTemplateId, this::getDayTemplate);
                if (dayTemplate == null) {
                    skippedCount++;
                    continue;
                }
                String scheduleId = createScheduleFromDayTemplate(dto.getOrganizationId(), dayTemplate, date, autoPublish);
                scheduleIds.add(scheduleId);
            }
        } else {
            throw new IllegalArgumentException("不支持的模板类型: " + dto.getTemplateKind());
        }

        int provisionedSlotCount = 0;
        if (autoProvision) {
            for (String scheduleId : scheduleIds) {
                provisionedSlotCount += provisionSlots(scheduleId);
            }
        }

        GenerateRosterVO result = new GenerateRosterVO();
        result.setScheduleIds(scheduleIds);
        result.setGeneratedCount(scheduleIds.size());
        result.setSkippedCount(skippedCount);
        result.setProvisionedSlotCount(provisionedSlotCount);
        return result;
    }

    private String createScheduleFromDayTemplate(String organizationId, Map<String, Object> dayTemplate,
                                                  LocalDate date, boolean autoPublish) {
        String scheduleId = IdUtil.fastSimpleUUID();
        Schedule schedule = new Schedule();
        schedule.setId(scheduleId);
        schedule.setOrganizationId(organizationId);
        schedule.setName((String) dayTemplate.get("templateName"));
        schedule.setPlanningHorizonStart(date.atStartOfDay());
        schedule.setPlanningHorizonEnd(date.atTime(23, 59, 59));
        schedule.setHealthcareServiceId((String) dayTemplate.get("healthcareServiceId"));
        schedule.setSlotProvisionTemplateId((String) dayTemplate.get("slotProvisionTemplateId"));

        if (autoPublish) {
            schedule.setStatusEnum("FORMAL");
            schedule.setActiveFlag(true);
        } else {
            schedule.setStatusEnum("DRAFT");
            schedule.setActiveFlag(false);
        }
        schedule.setVersion(0);
        scheduleMapper.insertSchedule(schedule);

        // 插入模板中的医师作为参与者
        String practitionerId = (String) dayTemplate.get("practitionerId");
        if (practitionerId != null) {
            ScheduleActor actor = new ScheduleActor();
            actor.setId(IdUtil.fastSimpleUUID());
            actor.setScheduleId(scheduleId);
            actor.setActorType(1); // 医师类型
            actor.setActorId(practitionerId);
            actor.setSortNo(0);
            actor.setVersion(0);
            scheduleMapper.insertScheduleActor(actor);
        }

        // 插入模板中的地点作为参与者
        String locationId = (String) dayTemplate.get("locationId");
        if (locationId != null) {
            ScheduleActor actor = new ScheduleActor();
            actor.setId(IdUtil.fastSimpleUUID());
            actor.setScheduleId(scheduleId);
            actor.setActorType(2); // 地点类型
            actor.setActorId(locationId);
            actor.setSortNo(1);
            actor.setVersion(0);
            scheduleMapper.insertScheduleActor(actor);
        }

        return scheduleId;
    }

    private int provisionSlots(String scheduleId) {
        // 放号逻辑后续对接 bkg_slot_provision_template 实现
        return 0;
    }

    // ---- private helpers ----

    private Map<String, Object> getDayTemplate(String id) {
        List<Map<String, Object>> list = scheduleMapper.selectDayTemplate(id);
        return list != null && !list.isEmpty() ? list.get(0) : null;
    }

    private YearMonth parseYearMonth(String ym) {
        if (ym != null && !ym.isEmpty()) {
            return YearMonth.parse(ym, DateTimeFormatter.ofPattern("yyyy-MM"));
        }
        return YearMonth.now();
    }

    private LocalDate toLocalDate(java.time.LocalDateTime dt) {
        return dt != null ? dt.toLocalDate() : null;
    }

    private boolean isActive(Schedule s) {
        return Boolean.TRUE.equals(s.getActiveFlag()) || "FORMAL".equalsIgnoreCase(s.getStatusEnum());
    }

    private boolean isDraft(Schedule s) {
        return "DRAFT".equalsIgnoreCase(s.getStatusEnum());
    }

    private ScheduleItemVO toItemVO(Schedule s, Map<String, List<ScheduleActor>> actorMap) {
        ScheduleItemVO vo = new ScheduleItemVO();
        vo.setScheduleId(s.getId() != null ? s.getId() : null);
        vo.setName(s.getName());
        vo.setStatusEnum(s.getStatusEnum());
        vo.setPlanningHorizonStart(s.getPlanningHorizonStart());
        vo.setPlanningHorizonEnd(s.getPlanningHorizonEnd());
        vo.setHealthcareServiceId(s.getHealthcareServiceId() != null ? s.getHealthcareServiceId() : null);
        vo.setSubstitutePractitionerId(s.getSubstitutePractitionerId() != null ? s.getSubstitutePractitionerId() : null);

        List<ScheduleActor> actors = actorMap.getOrDefault(s.getId(), Collections.emptyList());
        vo.setActors(actors.stream().map(this::toActorVO).collect(Collectors.toList()));

        return vo;
    }

    private ScheduleActorVO toActorVO(ScheduleActor a) {
        ScheduleActorVO vo = new ScheduleActorVO();
        vo.setActorType(a.getActorType() != null ? a.getActorType().toString() : null);
        vo.setActorId(a.getActorId() != null ? a.getActorId() : null);
        vo.setSortNo(a.getSortNo());
        return vo;
    }
}
