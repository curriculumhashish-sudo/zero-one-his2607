package com.zeroone.star.project.query.j2.basic;


import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.ToString;

/**
 * <p>
 * 描述：枚举类型查询
 * </p>
 * @author 逆光 ni_guang
 * @version 1.0
 */
@Data
@ToString
@Schema(description = "枚举类型查询")
public class EnumTypeQuery extends PageQuery {

    @Schema(description = "枚举类型代码", example = "enumTypeCode", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String enumTypeCode;
}
