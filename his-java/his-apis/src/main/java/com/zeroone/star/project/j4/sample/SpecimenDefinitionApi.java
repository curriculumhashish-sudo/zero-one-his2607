package com.zeroone.star.project.j4.sample;

import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionDeleteDTO;
import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionStatusDTO;
import com.zeroone.star.project.vo.j4.sample.SpecimenDefinitionOperateVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;

/**
 * 样本管理 - 样本定义 接口契约
 *
 * @author wieyao
 */
@Tag(name = "样本定义管理", description = "样本定义的启停用、删除等操作接口")
public interface SpecimenDefinitionApi {

    @Operation(summary = "启用/停用样本定义", description = "根据主键与乐观锁版本号，更新样本定义的发布状态")
    @PostMapping("/specimen-definition/update-status")
    SpecimenDefinitionOperateVO updateStatus(@Valid @RequestBody SpecimenDefinitionStatusDTO dto);

    @Operation(summary = "删除样本定义", description = "根据主键与乐观锁版本号，软删除样本定义")
    @PostMapping("/specimen-definition/delete")
    SpecimenDefinitionOperateVO delete(@Valid @RequestBody SpecimenDefinitionDeleteDTO dto);
}