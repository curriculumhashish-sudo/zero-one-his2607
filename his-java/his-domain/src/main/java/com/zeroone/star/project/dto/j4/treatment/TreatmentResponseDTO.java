package com.zeroone.star.project.dto.j4.treatment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.time.LocalDateTime;

/**
 * <p>
 * 描述：诊疗管理响应DTO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Data
@Schema(description = "诊疗管理响应DTO")
public class TreatmentResponseDTO {

    @Schema(description = "主键ID", example = "1680000111222333444", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "租户标识", example = "TENANT_001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String tenantId;

    @Schema(description = "创建时间", example = "2026-07-23T14:30:00", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private LocalDateTime createdAt;

    @Schema(description = "最后更新时间", example = "2026-07-23T14:30:00", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private LocalDateTime updatedAt;

    @Schema(description = "创建人", example = "admin", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String createdBy;

    @Schema(description = "最后更新人", example = "admin", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String updatedBy;

    @Schema(description = "软删标记 (0:未删除 1:已删除)", example = "0", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer isDeleted;

    @Schema(description = "删除时间", example = "2026-07-23T14:30:00", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private LocalDateTime deletedAt;

    @Schema(description = "删除人", example = "admin", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String deletedBy;

    @Schema(description = "乐观锁版本号", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer version;

    @Schema(description = "诊疗项目定义聚合根", example = "TREATMENT_ROOT", requiredMode = Schema.RequiredMode.REQUIRED)
    private String categoryCode;

    @Schema(description = "业务编码（全局唯一）", example = "BUS-TR-20260723001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;

    @Schema(description = "项目名称", example = "血常规", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "拼音检索码", example = "XCG", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String pyStr;

    @Schema(description = "五笔检索码", example = "TILQ", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String wbStr;

    @Schema(description = "销售/计价单位编码", example = "次", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String packageUnitCode;

    @Schema(description = "诊疗类型枚举编码", example = "INSPECT", requiredMode = Schema.RequiredMode.REQUIRED)
    private String typeEnum;

    @Schema(description = "发布状态枚举编码", example = "PUBLISHED", requiredMode = Schema.RequiredMode.REQUIRED)
    private String statusEnum;

    @Schema(description = "说明文本", example = "包含白细胞、红细胞等基础血液检测", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String descriptionText;

    @Schema(description = "身体部位编码", example = "ARM_VEIN", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String bodySiteCode;

    @Schema(description = "标本编码", example = "BLOOD", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String specimenCode;

    @Schema(description = "所属机构标识", example = "ORG_999", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String orgId;

    @Schema(description = "位置标识", example = "LOC_ROOM_101", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String locationId;

    @Schema(description = "计费规则标识", example = "1001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer ruleId;

    @Schema(description = "划价标记 (0:否 1:是)", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer pricingFlag;

    @Schema(description = "医保标记 (0:非医保 1:医保)", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer ybFlag;

    @Schema(description = "医保编码", example = "YB392010293", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String ybNo;

    @Schema(description = "医保对码标记 (0:未对码 1:已对码)", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer ybMatchFlag;

    @Schema(description = "医保收费等级 (1:甲类 2:乙类 3:丙类)", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer chrgitmLv;

    @Schema(description = "子项 JSON 快照", example = "[{\"children_id\":\"123\", \"children_num\":1}]", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String childrenJson;

    @Schema(description = "子项标记（0:无子项 1:有子项）", example = "0", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer childrenFlag;

    @Schema(description = "诊疗项目业务版本", example = "V1.0.0", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String activityVersion;

    @Schema(description = "商品名称", example = "血常规(化验)", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String merchandiseName;

    @Schema(description = "商品名拼音检索码", example = "XCGHY", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String merchandisePyStr;

    @Schema(description = "商品名五笔检索码", example = "TILQW", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String merchandiseWbStr;

    @Schema(description = "默认门诊/就诊单位编码", example = "DEPT_OUTPATIENT_01", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String defEncounterUnitCode;

    @Schema(description = "默认住院单位编码", example = "DEPT_INPATIENT_02", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String defInhospitalUnitCode;
}
