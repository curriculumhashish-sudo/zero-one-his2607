package com.zeroone.star.project.j5.configuration;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.Metadata.MetadataDetailDTO;
import com.zeroone.star.project.dto.j5.Metadata.MetadataListDTO;
import com.zeroone.star.project.query.j5.Metadata.MetadataQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.validation.annotation.Validated;

import java.util.List;

/**
 * <p>
 * 描述：元数据字段管理API接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zeroone
 * @version 1.0.0
 */
@Tag(name = "配置管理-元数据字段", description = "元数据字段管理接口")
@Validated
public interface MetadataApis {

    @Operation(summary = "获取元数据字段名称", description = "获取所有启用的元数据字段名称列表")
    JsonVO<List<String>> listFieldNames();

    @Operation(summary = "获取元数据字段列表", description = "条件+分页查询元数据字段")
    JsonVO<PageDTO<MetadataListDTO>> listFields(MetadataQuery query);

    @Operation(summary = "获取元数据字段详情", description = "根据ID查询元数据字段详情")
    JsonVO<MetadataDetailDTO> getFieldDetail(String id);
}
