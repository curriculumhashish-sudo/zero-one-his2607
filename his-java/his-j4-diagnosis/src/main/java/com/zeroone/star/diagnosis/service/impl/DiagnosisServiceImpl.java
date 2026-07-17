package com.zeroone.star.diagnosis.service.impl;

import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.diagnosis.entity.Diagnosis;
import com.zeroone.star.diagnosis.mapper.DiagnosisMapper;
import com.zeroone.star.diagnosis.service.IDiagnosisService;
import org.springframework.stereotype.Service;

/**
 * <p>
 * 描述：诊断管理服务实现
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Service
public class DiagnosisServiceImpl extends ServiceImpl<DiagnosisMapper, Diagnosis> implements IDiagnosisService {
}
