package com.zeroone.star.configuration.service;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.emr.configmanage.dto.ExecPlaceDTO;
import com.zeroone.star.project.emr.configmanage.entity.ExecPlace;
import com.zeroone.star.project.emr.configmanage.query.ExecPlaceQuery;
import com.zeroone.star.project.emr.configmanage.vo.ExecPlaceVO;
import com.zeroone.star.project.vo.JsonVO;

public interface IExecPlaceService extends IService<ExecPlace> {

    JsonVO<String> add(ExecPlaceDTO execPlaceDTO);

    JsonVO<String> update(ExecPlaceDTO execPlaceDTO);

    JsonVO<String> delete(Long id);

    JsonVO<PageDTO<ExecPlaceVO>> queryByPage(ExecPlaceQuery query);

    JsonVO<ExecPlaceVO> queryById(Long id);
}
