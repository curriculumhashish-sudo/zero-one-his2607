package com.zeroone.star.statdata.service.impl;

import com.zeroone.star.project.dto.j8.statdata.HourTrendDTO;
import com.zeroone.star.project.dto.j8.statdata.MonthTrendDTO;
import com.zeroone.star.statdata.entity.mongodb.AccessDailyTrend;
import com.zeroone.star.statdata.entity.mongodb.AccessMonthlyVector;
import com.zeroone.star.statdata.service.IVisitTrendService;
import org.springframework.data.mongodb.core.MongoTemplate;
import org.springframework.data.mongodb.core.query.Criteria;
import org.springframework.data.mongodb.core.query.Query;
import org.springframework.stereotype.Service;

import java.time.LocalDate;
import java.time.YearMonth;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.List;

/**
 * <p>
 * 访问趋势服务实现类
 * </p>
 *
 * @author highxin
 * @since 2026-07-29
 */
@Service
public class VisitTrendServiceImpl implements IVisitTrendService {

    private final MongoTemplate mongoTemplate;

    public VisitTrendServiceImpl(MongoTemplate mongoTemplate) {
        this.mongoTemplate = mongoTemplate;
    }

    @Override
    public List<HourTrendDTO> getHourTrend(String day, String tenantId, String hospitalId) {
        Query query = new Query();
        query.addCriteria(Criteria.where("date").is(day));
        if (tenantId != null && !tenantId.isEmpty()) {
            query.addCriteria(Criteria.where("tenantId").is(tenantId));
        }
        if (hospitalId != null && !hospitalId.isEmpty()) {
            query.addCriteria(Criteria.where("hospitalId").is(hospitalId));
        }
        AccessDailyTrend trend = mongoTemplate.findOne(query, AccessDailyTrend.class);
        if (trend == null || trend.getHourlyCounts() == null) {
            return new ArrayList<>();
        }
        List<HourTrendDTO> result = new ArrayList<>();
        List<Integer> hourlyCounts = trend.getHourlyCounts();
        for (int i = 0; i < hourlyCounts.size(); i++) {
            HourTrendDTO dto = new HourTrendDTO();
            dto.setHour(i);
            dto.setVisitCount(hourlyCounts.get(i).longValue());
            result.add(dto);
        }
        return result;
    }

    @Override
    public List<MonthTrendDTO> getMonthTrend(String yearMonth, String tenantId, String hospitalId) {
        Query query = new Query();
        query.addCriteria(Criteria.where("yearMonth").is(yearMonth));
        if (tenantId != null && !tenantId.isEmpty()) {
            query.addCriteria(Criteria.where("tenantId").is(tenantId));
        }
        if (hospitalId != null && !hospitalId.isEmpty()) {
            query.addCriteria(Criteria.where("hospitalId").is(hospitalId));
        }

        AccessMonthlyVector vector = mongoTemplate.findOne(query, AccessMonthlyVector.class);
        if (vector == null || vector.getDailyCounts() == null) {
            return new ArrayList<>();
        }

        YearMonth ym = YearMonth.parse(yearMonth, DateTimeFormatter.ofPattern("yyyy-MM"));
        int daysInMonth = ym.lengthOfMonth();

        List<MonthTrendDTO> result = new ArrayList<>();
        List<Integer> dailyCounts = vector.getDailyCounts();
        for (int i = 0; i < dailyCounts.size() && i < daysInMonth; i++) {
            MonthTrendDTO dto = new MonthTrendDTO();
            LocalDate date = ym.atDay(i + 1);
            dto.setDate(date.format(DateTimeFormatter.ISO_LOCAL_DATE));
            dto.setVisitCount(dailyCounts.get(i).longValue());
            result.add(dto);
        }
        return result;
    }
}