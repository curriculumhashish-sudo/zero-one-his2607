package com.zeroone.star.project.dto.j3.department;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Pattern;
import lombok.Data;

/**
 * <p>
 * 描述：新增科室传输对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 胡傲宇
 * @version 1.0.0
 */
@Data
@Schema(description = "新增科室DTO")
public class DepartmentCreateDTO {
    @Schema(description = "名称", example = "门诊内科", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotNull(message = "机构名称不能为空")
    private String name;

    @Schema(description = "类型, 1-医院, 2-科室", example = "2", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotNull(message = "机构类型不能为空")
    @Pattern(regexp = "^[12]$", message = "机构类型只能是1-2之间的数字")
    private String typeEnum;

    @Schema(description = "类别, 1-门诊, 2-住院, 3-药房, 4-库房, 5-财务, 6-护士站, 7-管理, 8-后勤支持, 9-其他, 10-医技",
            example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    @NotNull(message = "类别不能为空")
    @Pattern(regexp = "^[1-10]$", message = "类别只能是1-10之间的数字")
    private String classEnum;

    @Schema(description = "上级组织", example = "322663105141149696", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    @Pattern(regexp = "^[0-9]+$", message = "上级组织只能是数字")
    private String parentId;

    @Schema(description = "医保编码", example = "YB110101", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String ybNo;

    @Schema(description = "医保名称", example = "北京协和医院(医保)", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String ybName;

    @Schema(description = "机构标识", example = "H11000001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String medinsId;

    @Schema(description = "医保区划", example = "110000", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String medinsAdmdvs;

    @Schema(description = "医疗机构类型", example = "HOSPITAL", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String medinsType;

    @Schema(description = "医疗机构等级", example = "三级甲等", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String medinsLv;

    @Schema(description = "拼音", example = "xnxnk", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String pyStr;

    @Schema(description = "五笔", example = "tntk", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String wbStr;

    @Schema(description = "默认医师ID", example = "308791351298756608", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    @Pattern(regexp = "^[0-9]+$", message = "默认医师ID只能是数字")
    private String defDoctorId;

    @Schema(description = "科别", example = "内科", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String caty;

    @Schema(description = "显示顺序", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    @Pattern(regexp = "^[0-9]+$", message = "显示顺序只能是数字")
    private String displayOrder;
}