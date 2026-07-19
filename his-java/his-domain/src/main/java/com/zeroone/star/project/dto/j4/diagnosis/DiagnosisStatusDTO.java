package com.zeroone.star.project.dto.j4.diagnosis;

import com.fasterxml.jackson.annotation.JsonIgnore;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.AssertTrue;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotEmpty;
import lombok.Data;

import java.io.Serializable;
import java.util.HashSet;
import java.util.List;

/**
 * 诊断启停请求数据。
 *
 * @author j4组
 * @version 1.0.0
 */
@Data
@Schema(description = "诊断启停请求数据")
public class DiagnosisStatusDTO implements Serializable {

    private static final long serialVersionUID = 1L;

    @NotEmpty(message = "诊断ID集合不能为空")
    @Schema(description = "诊断ID集合", example = "[\"101\", \"102\"]",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private List<@NotBlank(message = "诊断ID不能为空") String> ids;

    @JsonIgnore
    @AssertTrue(message = "诊断ID不能重复")
    public boolean isIdsDistinct() {
        return ids == null || new HashSet<>(ids).size() == ids.size();
    }
}
