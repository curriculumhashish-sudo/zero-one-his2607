package com.zeroone.star.statdata.entity.mongodb;

import lombok.Data;
import org.springframework.data.annotation.Id;
import org.springframework.data.mongodb.core.mapping.Document;
import org.springframework.data.mongodb.core.mapping.Field;

import java.util.List;

/**
 * <p>
 * 访问日趋势文档
 * </p>
 *
 * @author highxin
 * @since 2026-07-29
 */
@Data
@Document(collection = "access_daily_trend")
public class AccessDailyTrend {

    @Id
    private String id;

    @Field("date")
    private String date;

    @Field("tenantId")
    private String tenantId;

    @Field("hospitalId")
    private String hospitalId;

    @Field("hourlyCounts")
    private List<Integer> hourlyCounts;

    @Field("totalCount")
    private Long totalCount;

    @Field("uniqueUsers")
    private Long uniqueUsers;

    @Field("uniqueIps")
    private Long uniqueIps;

    @Field("avgDuration")
    private Long avgDuration;

    @Field("errorCount")
    private Long errorCount;
}