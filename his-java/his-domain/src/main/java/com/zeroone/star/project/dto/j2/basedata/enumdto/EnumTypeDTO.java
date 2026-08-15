package com.zeroone.star.project.dto.j2.basedata.enumdto;

import com.zeroone.star.project.dto.PageDTO;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.ToString;
import org.springframework.data.domain.Page;

import java.util.Date;

/**
 * <p>
 * 描述：枚举类型DTO
 * </p>
 * @author 逆光 ni_guang
 * @version 1.0
 */
@Data
@Schema(description = "枚举类型DTO，包含ID、租户ID、创建时间、更新时间、创建人、更新人，用于表示枚举类型的基本信息")
public class EnumTypeDTO extends EnumTypeModifyDTO {

    @Schema(description = "租户ID", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String tenantId;

    @Schema(description = "创建时间", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Date createdAt;

    @Schema(description = "更新时间", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Date updatedAt;

    @Schema(description = "创建人", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String createdBy;

    @Schema(description = "更新人", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String updatedBy;

    public static PageDTO<EnumTypeDTO> create(Page<EnumTypeDTO> page) {
        PageDTO<EnumTypeDTO> pageDTO = new PageDTO<>();

        pageDTO.setPageIndex((long) page.getNumber());
        pageDTO.setPageSize((long) page.getSize());
        pageDTO.setTotal(page.getTotalElements());
        pageDTO.setPages((long) page.getTotalPages());
        pageDTO.setRows(page.getContent());

        return pageDTO;
    }
}
