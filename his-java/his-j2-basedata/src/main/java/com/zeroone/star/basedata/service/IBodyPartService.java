package com.zeroone.star.basedata.service;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.bodypartdto.BodyPartAddDTO;
import com.zeroone.star.project.query.j2.basedata.BodyPartPageQuery;
import com.zeroone.star.project.vo.j2.basedata.bodypart.BodyPartListVO;
import com.zeroone.star.project.vo.j2.basedata.bodypart.BodyPartTreeVO;

import java.util.List;

public interface IBodyPartService {

    List<BodyPartTreeVO> getTree();

    PageDTO<BodyPartListVO> getList(BodyPartPageQuery query);

    String add(BodyPartAddDTO dto);
}