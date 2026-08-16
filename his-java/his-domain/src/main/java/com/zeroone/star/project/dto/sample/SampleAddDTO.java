package com.zeroone.star.project.dto.sample;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Pattern;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;
import org.hibernate.validator.constraints.Range;

/**
 * <p>
 * 描述：添加数据传输对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Getter
@Setter
@ToString
@Schema(description = "添加数据传输对象")
public class SampleAddDTO {
    @Schema(description = "姓名", example = "张三")
    @NotBlank(message = "姓名不能为空")
    private String name;
    @Schema(description = "性别", example = "男")
    @Pattern(regexp = "[男女]", message = "性别只能是男或女")
    private String sex;
    @Schema(description = "年龄", example = "18")
    @Range(min = 0, max = 300, message = "年龄范围在0-300岁之间")
    private Integer age;
}
