package com.zeroone.star.project.query.j3.patient;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
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

    @Schema(description = "患者姓名")
    private String name;

    @Schema(description = "身份证号")
    private String idCard;

    @Schema(description = "联系电话")
    private String phone;

    @Schema(description = "性别编码")
    private String genderEnum;

    @Schema(description = "患者状态编码")
    private String statusEnum;
}
