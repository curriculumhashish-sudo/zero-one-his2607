package com.zeroone.star.statdata.entity.mongodb;

import lombok.Data;
import org.springframework.data.annotation.Id;
import org.springframework.data.mongodb.core.mapping.Document;
import org.springframework.data.mongodb.core.mapping.Field;

import java.util.List;
/**
 * <p>
 * 访问月趋势文档
 * </p>
 *
 * @author highxin
 * @since 2026-07-29
 */
@Data
@Document(collection = "access_monthly_vector")
public class AccessMonthlyVector {

    @Id
    private String id;

    @Field("yearMonth")
    private String yearMonth;

    @Field("tenantId")
    private String tenantId;

    @Field("hospitalId")
    private String hospitalId;

    @Field("dailyCounts")
    private List<Integer> dailyCounts;

    @Field("totalCount")
    private Long totalCount;

    @Field("dailyAvg")
    private Long dailyAvg;

    @Field("peakDay")
    private String peakDay;

    @Field("peakCount")
    private Long peakCount;

    @Field("topPaths")
    private List<TopPath> topPaths;

    @Data
    public static class TopPath {
        @Field("path")
        private String path;
        @Field("count")
        private Long count;
    }
}