package com.zeroone.star.configuration.service;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.configuration.dto.ExecPlaceDTO;
import com.zeroone.star.configuration.entity.ExecPlace;
import com.zeroone.star.project.vo.JsonVO;

public interface IExecPlaceService extends IService<ExecPlace> {

    JsonVO<String> add(ExecPlaceDTO execPlaceDTO);

    JsonVO<String> update(ExecPlaceDTO execPlaceDTO);

    JsonVO<String> delete(Long id);
}
