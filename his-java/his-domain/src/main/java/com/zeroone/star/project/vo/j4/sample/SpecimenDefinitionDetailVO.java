package com.zeroone.star.project.vo.j4.sample;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.time.LocalDateTime;

/**
 * <p>
 * 描述：样本定义详情视图对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Data
@Schema(description = "样本定义详情响应")
public class SpecimenDefinitionDetailVO {

    /**
     * 样本定义主键
     */
    @Schema(description = "样本定义主键", example = "328090253175296000")
    private String id;

    /**
     * 租户标识
     */
    @Schema(description = "租户标识", example = "1")
    private String tenantId;

    /**
     * 业务编码（样本定义聚合根）
     */
    @Schema(description = "业务编码", example = "SPD0001")
    private String busNo;

    /**
     * 样本名称（面向临床与维护页的显示名称）
     */
    @Schema(description = "样本名称", example = "血")
    private String name;

    /**
     * 拼音检索码
     */
    @Schema(description = "拼音检索码", example = "X")
    private String pyStr;

    /**
     * 五笔检索码
     */
    @Schema(description = "五笔检索码", example = "T")
    private String wbStr;

    /**
     * 标本种类编码（对应 SpecimenType 枚举）
     */
    @Schema(description = "标本种类编码（如：BLOOD-血液、URINE-尿液、STOOL-粪便、SPUTUM-痰液）", example = "BLOOD")
    private String typeEnum;

    /**
     * 发布状态编码（对应 PublicationStatus 枚举）
     */
    @Schema(description = "发布状态编码（如：ACTIVE-已启用、INACTIVE-已停用、DRAFT-草稿）", example = "ACTIVE")
    private String statusEnum;

    /**
     * 创建时间
     */
    @Schema(description = "创建时间", example = "2026-06-24 16:33:31")
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     */
    @Schema(description = "最后更新时间", example = "2026-06-24 16:33:31")
    private LocalDateTime updatedAt;

    /**
     * 创建人
     */
    @Schema(description = "创建人", example = "1")
    private String createdBy;

    /**
     * 最后更新人
     */
    @Schema(description = "最后更新人", example = "1")
    private String updatedBy;

    /**
     * 乐观锁版本号
     */
    @Schema(description = "乐观锁版本号（修改/删除时需回传）", example = "0")
    private Integer version;
}
