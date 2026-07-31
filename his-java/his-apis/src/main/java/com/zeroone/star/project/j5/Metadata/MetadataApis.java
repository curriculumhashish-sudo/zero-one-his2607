package com.zeroone.star.project.j5.Metadata;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.Metadata.MetadataDetailDTO;
import com.zeroone.star.project.dto.j5.Metadata.MetadataListDTO;
import com.zeroone.star.project.query.j5.Metadata.MetadataQuery;
import com.zeroone.star.project.vo.JsonVO;

import java.util.List;

public interface MetadataApis {

    JsonVO<List<String>> listFieldNames();

    JsonVO<PageDTO<MetadataListDTO>> listFields(MetadataQuery query);

    JsonVO<MetadataDetailDTO> getFieldDetail(String id);
}