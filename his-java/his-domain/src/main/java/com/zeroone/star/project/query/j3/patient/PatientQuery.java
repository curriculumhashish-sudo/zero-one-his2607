package com.zeroone.star.project.query.j3.patient;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Size;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

/**
 * <p>
 * 描述：患者查询条件
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 波小夏
 * @version 1.0.0
 */
@Getter
@Setter
@ToString(callSuper = true)
public class PatientQuery extends PageQuery {

    @Schema(description = "综合关键词，匹配姓名、患者编号、身份证号、拼音码和五笔码")
    @Size(max = 255, message = "综合关键词长度不能超过255个字符")
    private String keyword;

    @Schema(description = "患者姓名")
    @Size(max = 255, message = "患者姓名长度不能超过255个字符")
    private String name;

    @Schema(description = "患者编号")
    @Size(max = 255, message = "患者编号长度不能超过255个字符")
    private String busNo;

    @Schema(description = "身份证号")
    @Size(max = 32, message = "身份证号长度不能超过32个字符")
    private String idCard;

    @Schema(description = "联系电话")
    @Size(max = 32, message = "联系电话长度不能超过32个字符")
    private String phone;

    @Schema(description = "性别编码")
    private String genderEnum;

    @Schema(description = "患者状态编码")
    private String statusEnum;

    @Schema(description = "启用标记")
    private Boolean activeFlag;

    @Schema(description = "临时建档标记")
    private Boolean tempFlag;

    @Schema(description = "建档机构ID")
    private String organizationId;
}
