package com.zeroone.star.project.j5.configuration;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.configuration.MetadataDetailDTO;
import com.zeroone.star.project.dto.j5.configuration.MetadataListDTO;
import com.zeroone.star.project.query.j5.configuration.MetadataQuery;
import com.zeroone.star.project.vo.JsonVO;

import java.util.List;

public interface MetadataApis {

    JsonVO<List<String>> listFieldNames();

    JsonVO<PageDTO<MetadataListDTO>> listFields(MetadataQuery query);

    JsonVO<MetadataDetailDTO> getFieldDetail(String id);
}