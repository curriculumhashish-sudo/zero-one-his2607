package com.zeroone.star.statdata.service.impl;

import com.zeroone.star.project.dto.j8.statdata.HourTrendDTO;
import com.zeroone.star.project.query.j8.statdata.HourTrendQuery;
import com.zeroone.star.project.vo.j8.statdata.HourTrendVO;
import com.zeroone.star.statdata.service.IVisitTrendService;
import org.springframework.data.redis.core.Cursor;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.data.redis.core.ScanOptions;
import org.springframework.stereotype.Service;

import java.time.Instant;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.YearMonth;
import java.time.ZoneId;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;

/**
 * <p>
 * 访问趋势服务实现类（基于登录上下文 Redis 数据聚合）
 * </p>
 *
 * @author highxin
 * @since 2026-07-29
 */
@Service
public class VisitTrendServiceImpl implements IVisitTrendService {

    private static final String LOGIN_CONTEXT_KEY_PATTERN = "his:login:context:*";

    private final RedisTemplate<String, Object> redisTemplate;

    public VisitTrendServiceImpl(RedisTemplate<String, Object> redisTemplate) {
        this.redisTemplate = redisTemplate;
    }

    @Override
    public HourTrendVO getHourTrend(HourTrendQuery query) {
        String targetDate = query.getDateFrom();
        String organizationId = query.getOrganizationId();

        int[] hourCounts = new int[24];

        Set<String> keys = redisTemplate.keys(LOGIN_CONTEXT_KEY_PATTERN);
        if (keys != null) {
            for (String key : keys) {
                Map<Object, Object> ctx = redisTemplate.opsForHash().entries(key);
                String loginTimeStr = String.valueOf(ctx.getOrDefault("loginTime", ""));
                String hospitalId = String.valueOf(ctx.getOrDefault("hospitalId", ""));

                if (loginTimeStr.isEmpty()) {
                    continue;
                }

                long loginTimeMillis = Long.parseLong(loginTimeStr);
                LocalDateTime loginDateTime = LocalDateTime.ofInstant(
                        Instant.ofEpochMilli(loginTimeMillis), ZoneId.systemDefault());
                String loginDate = loginDateTime.toLocalDate().format(DateTimeFormatter.ISO_LOCAL_DATE);

                if (!loginDate.equals(targetDate)) {
                    continue;
                }

                if (organizationId != null && !organizationId.isEmpty()
                        && !organizationId.equals(hospitalId)) {
                    continue;
                }

                hourCounts[loginDateTime.getHour()]++;
            }
        }

        HourTrendVO vo = new HourTrendVO();
        vo.setUnit("次");
        List<HourTrendDTO> points = new ArrayList<>();
        for (int i = 0; i < 24; i++) {
            HourTrendDTO dto = new HourTrendDTO();
            dto.setLabel(String.valueOf(i));
            dto.setValue((long) hourCounts[i]);
            points.add(dto);
        }
        vo.setPoints(points);
        return vo;
    }

    @Override
    public HourTrendVO getMonthTrend(HourTrendQuery query) {
        String yearMonth = query.getDateFrom();
        String organizationId = query.getOrganizationId();

        YearMonth ym = YearMonth.parse(yearMonth, DateTimeFormatter.ofPattern("yyyy-MM"));
        int daysInMonth = ym.lengthOfMonth();
        int[] dayCounts = new int[daysInMonth + 1];

        Set<String> keys = redisTemplate.keys(LOGIN_CONTEXT_KEY_PATTERN);
        if (keys != null) {
            for (String key : keys) {
                Map<Object, Object> ctx = redisTemplate.opsForHash().entries(key);
                String loginTimeStr = String.valueOf(ctx.getOrDefault("loginTime", ""));
                String hospitalId = String.valueOf(ctx.getOrDefault("hospitalId", ""));

                if (loginTimeStr.isEmpty()) {
                    continue;
                }

                long loginTimeMillis = Long.parseLong(loginTimeStr);
                LocalDate loginDate = LocalDateTime.ofInstant(
                        Instant.ofEpochMilli(loginTimeMillis), ZoneId.systemDefault()).toLocalDate();

                String loginYearMonth = loginDate.format(DateTimeFormatter.ofPattern("yyyy-MM"));
                if (!loginYearMonth.equals(yearMonth)) {
                    continue;
                }

                if (organizationId != null && !organizationId.isEmpty()
                        && !organizationId.equals(hospitalId)) {
                    continue;
                }

                dayCounts[loginDate.getDayOfMonth()]++;
            }
        }

        HourTrendVO vo = new HourTrendVO();
        vo.setUnit("次");
        List<HourTrendDTO> points = new ArrayList<>();
        for (int i = 1; i <= daysInMonth; i++) {
            HourTrendDTO dto = new HourTrendDTO();
            LocalDate date = ym.atDay(i);
            dto.setLabel(date.format(DateTimeFormatter.ISO_LOCAL_DATE));
            dto.setValue((long) dayCounts[i]);
            points.add(dto);
        }
        vo.setPoints(points);
        return vo;
    }
}