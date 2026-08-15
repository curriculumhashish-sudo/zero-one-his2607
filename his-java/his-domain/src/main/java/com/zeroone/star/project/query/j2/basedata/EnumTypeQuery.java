package com.zeroone.star.project.query.j2.basedata;


import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.ToString;

/**
 * <p>
 * 描述：枚举类型查询
 * </p>
 * @author 逆光 ni_guang
 * @version 1.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "枚举类型查询")
public class EnumTypeQuery extends PageQuery {

    @Schema(description = "枚举类型关键字，将会查询具有该关键字的枚举类型，不区分大小写",
            example = "gender")
    private String enumTypeKeyword;
}
