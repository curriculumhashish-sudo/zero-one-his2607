package com.zeroone.star.project.j3.register.query;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

/**
 * <p>
 * 描述：挂号类型分页查询条件对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author j3小组
 * @version 1.0.0
 */
@Getter
@Setter
@ToString
public class RegisterQuery extends PageQuery {

    /**
     * 关键词（可输入名称、医保编码或id进行模糊查询）
     */
    @Schema(description = "关键词", example = "普通号")
    private String keyword;
}
