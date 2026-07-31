package com.zeroone.star.clinics.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.clinics.entity.Schedule;
import com.zeroone.star.clinics.mapper.ScheduleMapper;
import com.zeroone.star.clinics.service.ScheduleService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j8.clinics.ScheduleAddDTO;
import com.zeroone.star.project.dto.j8.clinics.ScheduleDTO;
import com.zeroone.star.project.query.j8.clinics.ScheduleListQuery;
import com.zeroone.star.project.vo.JsonVO;
import org.springframework.beans.BeanUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;



@Service
public class ScheduleServiceImpl implements ScheduleService {

    @Autowired
    private ScheduleMapper scheduleMapper;

    /**
     * 查询日程列表
     * @param scheduleListQuery
     * @return
     */
    @Override
    public PageDTO<Schedule> listQuery(ScheduleListQuery scheduleListQuery) {
        // 创建 MyBatis-Plus 分页对象
        Page<Schedule> page = new Page<>(scheduleListQuery.getPageIndex(), scheduleListQuery.getPageSize());

        // 构建查询条件
        LambdaQueryWrapper<Schedule> queryWrapper = new LambdaQueryWrapper<>();
        
        // 科室查询条件 或许需要关联查询
        if (scheduleListQuery.getDepartmentId() != null && !scheduleListQuery.getDepartmentId().isEmpty()) {
            // 尝试在服务类别JSON字段中查找科室信息
            queryWrapper.and(wrapper -> wrapper
                .like(Schedule::getServiceCategoryJson, scheduleListQuery.getDepartmentId())
                .or()
                .like(Schedule::getSpecialtyJson, scheduleListQuery.getDepartmentId())
            );
        }
        
        // 医师查询条件 或许需要关联查询
        if (scheduleListQuery.getDoctorId() != null && !scheduleListQuery.getDoctorId().isEmpty()) {

                Long doctorIdValue = Long.valueOf(scheduleListQuery.getDoctorId());
                queryWrapper.and(wrapper -> wrapper
                    .eq(Schedule::getHealthcareServiceId, doctorIdValue)
                    .or()
                    .eq(Schedule::getSubstitutePractitionerId, doctorIdValue)
                );

        }
        
        // 诊室查询条件 (可能需要通过其他关联字段实现)
        if (scheduleListQuery.getClinicRoomId() != null && !scheduleListQuery.getClinicRoomId().isEmpty()) {
            // 由于实体中没有直接的诊室字段，我们可以通过名称或其他字段模糊匹配
            queryWrapper.like(Schedule::getName, scheduleListQuery.getClinicRoomId());
        }
        
        // 号源类型查询条件 (使用 healthcareServiceId 字段)
        if (scheduleListQuery.getSlotType() != null) {
            queryWrapper.eq(Schedule::getHealthcareServiceId, scheduleListQuery.getSlotType());
        }
        
        Page<Schedule> schedulePage = scheduleMapper.selectPage(page, queryWrapper);

        // 返回分页结果
        return PageDTO.create(schedulePage);
    }

    /**
     * 根据ID查询日程
     * @param scheduleId
     * @return
     */
    @Override
    public ScheduleDTO getById(Long scheduleId) {
        return scheduleMapper.getById(scheduleId);
    }

    /**
     * 新增日程
     * @param dto
     */
    @Override
    public void insertSchedule(ScheduleAddDTO dto) {
        Schedule schedule = new Schedule();
        BeanUtils.copyProperties(dto,schedule);
        scheduleMapper.insertSchedule(schedule);

    }

    /**
     * 生成正式排班
     * @param formalRosterDTO 生成正式排班参数
     * @return 操作结果
     */
    @Override
    public String generateFormalRoster(com.zeroone.star.project.dto.j8.clinics.FormalRosterDTO formalRosterDTO) {
        try {
            // 根据排班类型处理不同的逻辑
            if (formalRosterDTO.getRosterType() == 0) { // 日排班
                handleDailyRoster(formalRosterDTO);
            } else if (formalRosterDTO.getRosterType() == 1) { // 周排班
                handleWeeklyRoster(formalRosterDTO);
            } else {
                throw new IllegalArgumentException("无效的排班类型: " + formalRosterDTO.getRosterType());
            }
            
            return "生成正式排班成功";
        } catch (Exception e) {
            throw new RuntimeException("生成正式排班失败: " + e.getMessage(), e);
        }
    }

    /**
     * 处理日排班逻辑
     * @param formalRosterDTO 排班参数
     */
    private void handleDailyRoster(com.zeroone.star.project.dto.j8.clinics.FormalRosterDTO formalRosterDTO) {
        // 创建单日的日程记录
        Schedule schedule = new Schedule();
        schedule.setName(formalRosterDTO.getRosterName());
        
        // 设置时间范围,日排班则相同的开始时间和结束时间
        schedule.setPlanningHorizonStart(formalRosterDTO.getStartTime());
        schedule.setPlanningHorizonEnd(formalRosterDTO.getEndTime());
        
        // 设置状态 - 如果自动发布则为正式状态，否则为草稿状态
        if (formalRosterDTO.getIsPublish() != null && formalRosterDTO.getIsPublish() == 1) {
            schedule.setStatusEnum("FORMAL"); // 正式状态
            schedule.setActiveFlag(true); // 启用
        } else {
            schedule.setStatusEnum("DRAFT"); // 草稿状态
            schedule.setActiveFlag(false); // 未启用
        }
        
        // 保存到数据库
        scheduleMapper.insertSchedule(schedule);
        
        // 如果需要自动放号
        if (formalRosterDTO.getIsSlotsRelease() != null && formalRosterDTO.getIsSlotsRelease() == 1) {
            releaseSlots(schedule.getId());
        }
    }

    /**
     * 处理周排班逻辑
     * @param formalRosterDTO 排班参数
     */
    private void handleWeeklyRoster(com.zeroone.star.project.dto.j8.clinics.FormalRosterDTO formalRosterDTO) {
        // 计算从开始日期到结束日期之间的所有日期
        java.time.LocalDate startDate = formalRosterDTO.getStartTime().toLocalDate();
        java.time.LocalDate endDate = formalRosterDTO.getEndTime().toLocalDate();
        
        // 遍历每个日期，根据weekday生成对应的排班
        java.time.temporal.ChronoUnit unit = java.time.temporal.ChronoUnit.DAYS;
        long daysBetween = unit.between(startDate, endDate);
        
        for (long i = 0; i <= daysBetween; i++) {
            java.time.LocalDate currentDate = startDate.plusDays(i);
            int dayOfWeek = currentDate.getDayOfWeek().getValue(); // 1-7 对应周一到周日
            
            // 创建当天的日程记录
            Schedule schedule = new Schedule();
            schedule.setName(formalRosterDTO.getRosterName() + "-" + currentDate.format(java.time.format.DateTimeFormatter.ofPattern("yyyy-MM-dd")));
            
            // 设置当天的时间范围
            java.time.LocalDateTime scheduleStart = java.time.LocalDateTime.of(currentDate, formalRosterDTO.getStartTime().toLocalTime());
            java.time.LocalDateTime scheduleEnd = java.time.LocalDateTime.of(currentDate, formalRosterDTO.getEndTime().toLocalTime());
            
            schedule.setPlanningHorizonStart(scheduleStart);
            schedule.setPlanningHorizonEnd(scheduleEnd);
            
            // 设置状态
            if (formalRosterDTO.getIsPublish() != null && formalRosterDTO.getIsPublish() == 1) {
                schedule.setStatusEnum("FORMAL"); // 正式状态
                schedule.setActiveFlag(true); // 启用
            } else {
                schedule.setStatusEnum("DRAFT"); // 草稿状态
                schedule.setActiveFlag(false); // 未启用
            }
            
            // 保存到数据库
            scheduleMapper.insertSchedule(schedule);
            
            // 如果需要自动放号
            if (formalRosterDTO.getIsSlotsRelease() != null && formalRosterDTO.getIsSlotsRelease() == 1) {
                releaseSlots(schedule.getId());
            }
        }
    }

    /**
     * 自动放号处理
     * @param scheduleId 日程ID
     */
    private void releaseSlots(Long scheduleId) {
        // 暂时留空，等待具体的放号业务逻辑实现
        System.out.println("对日程ID " + scheduleId + " 进行放号处理");
    }
}