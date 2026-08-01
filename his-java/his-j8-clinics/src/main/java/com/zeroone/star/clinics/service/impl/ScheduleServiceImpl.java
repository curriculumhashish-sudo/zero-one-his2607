package com.zeroone.star.clinics.service.impl;

import com.zeroone.star.clinics.entity.Schedule;
import com.zeroone.star.clinics.entity.ScheduleActor;
import com.zeroone.star.clinics.mapper.ScheduleMapper;
import com.zeroone.star.clinics.service.ScheduleService;
import com.zeroone.star.project.dto.j8.clinics.ScheduleAddDTO;
import com.zeroone.star.project.query.j8.clinics.ScheduleListQuery;
import com.zeroone.star.project.vo.j8.clinics.*;
import org.springframework.beans.BeanUtils;
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
    public void insertSchedule(ScheduleAddDTO dto) {
        Schedule schedule = new Schedule();
        BeanUtils.copyProperties(dto, schedule);
        scheduleMapper.insertSchedule(schedule);
    }

    @Override
    public String generateFormalRoster(com.zeroone.star.project.dto.j8.clinics.FormalRosterDTO formalRosterDTO) {
        try {
            if (formalRosterDTO.getRosterType() == 0) {
                handleDailyRoster(formalRosterDTO);
            } else if (formalRosterDTO.getRosterType() == 1) {
                handleWeeklyRoster(formalRosterDTO);
            } else {
                throw new IllegalArgumentException("无效的排班类型: " + formalRosterDTO.getRosterType());
            }
            return "生成正式排班成功";
        } catch (Exception e) {
            throw new RuntimeException("生成正式排班失败: " + e.getMessage(), e);
        }
    }

    // ---- private helpers ----

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

    private void handleDailyRoster(com.zeroone.star.project.dto.j8.clinics.FormalRosterDTO formalRosterDTO) {
        Schedule schedule = new Schedule();
        schedule.setName(formalRosterDTO.getRosterName());
        schedule.setPlanningHorizonStart(formalRosterDTO.getStartTime());
        schedule.setPlanningHorizonEnd(formalRosterDTO.getEndTime());
        if (formalRosterDTO.getIsPublish() != null && formalRosterDTO.getIsPublish() == 1) {
            schedule.setStatusEnum("FORMAL");
            schedule.setActiveFlag(true);
        } else {
            schedule.setStatusEnum("DRAFT");
            schedule.setActiveFlag(false);
        }
        scheduleMapper.insertSchedule(schedule);
        if (formalRosterDTO.getIsSlotsRelease() != null && formalRosterDTO.getIsSlotsRelease() == 1) {
            releaseSlots(schedule.getId());
        }
    }

    private void handleWeeklyRoster(com.zeroone.star.project.dto.j8.clinics.FormalRosterDTO formalRosterDTO) {
        java.time.LocalDate startDate = formalRosterDTO.getStartTime().toLocalDate();
        java.time.LocalDate endDate = formalRosterDTO.getEndTime().toLocalDate();
        long daysBetween = ChronoUnit.DAYS.between(startDate, endDate);
        for (long i = 0; i <= daysBetween; i++) {
            java.time.LocalDate currentDate = startDate.plusDays(i);
            Schedule schedule = new Schedule();
            schedule.setName(formalRosterDTO.getRosterName() + "-" + currentDate.format(DateTimeFormatter.ofPattern("yyyy-MM-dd")));
            java.time.LocalDateTime scheduleStart = java.time.LocalDateTime.of(currentDate, formalRosterDTO.getStartTime().toLocalTime());
            java.time.LocalDateTime scheduleEnd = java.time.LocalDateTime.of(currentDate, formalRosterDTO.getEndTime().toLocalTime());
            schedule.setPlanningHorizonStart(scheduleStart);
            schedule.setPlanningHorizonEnd(scheduleEnd);
            if (formalRosterDTO.getIsPublish() != null && formalRosterDTO.getIsPublish() == 1) {
                schedule.setStatusEnum("FORMAL");
                schedule.setActiveFlag(true);
            } else {
                schedule.setStatusEnum("DRAFT");
                schedule.setActiveFlag(false);
            }
            scheduleMapper.insertSchedule(schedule);
            if (formalRosterDTO.getIsSlotsRelease() != null && formalRosterDTO.getIsSlotsRelease() == 1) {
                releaseSlots(schedule.getId());
            }
        }
    }

    private void releaseSlots(String scheduleId) {
        System.out.println("对日程ID " + scheduleId + " 进行放号处理");
    }
}
