package com.zeroone.star.configuration.service;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.configuration.MetadataDetailDTO;
import com.zeroone.star.project.dto.j5.configuration.MetadataListDTO;
import com.zeroone.star.project.query.j5.configuration.MetadataQuery;
import com.zeroone.star.configuration.entity.Metadata;
import com.baomidou.mybatisplus.extension.service.IService;

import java.util.List;

public interface MetadataService extends IService<Metadata> {

    List<String> listFieldNames();

    PageDTO<MetadataListDTO> listFields(MetadataQuery query);

    MetadataDetailDTO getFieldDetail(String id);
}