package com.zeroone.star.project.dto.j4.sample;

import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

/**
 * <p>
 * 描述：样本定义 修改 请求参数
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author wieyao
 * @version 1.0.0
 */
@Data
public class SpecimenDefinitionUpdateDTO {

    /**
     * 样本定义主键
     */
    @NotBlank(message = "样本定义主键不能为空")
    private String id;

    /**
     * 乐观锁版本号（用于并发控制，取查询结果中的 version）
     */
    @NotNull(message = "乐观锁版本号不能为空")
    private Integer version;

    /**
     * 样本名称
     */
    @NotBlank(message = "样本名称不能为空")
    private String name;

    /**
     * 标本种类编码（对应 SpecimenType 枚举）
     */
    @NotBlank(message = "标本种类不能为空")
    private String typeEnum;
}