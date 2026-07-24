package com.zeroone.star.sysmgr.controller.tenant;

import com.zeroone.star.project.j1.login.LoginApi;
import com.zeroone.star.project.dto.j1.login.SelectHospitalDTO;
import com.zeroone.star.project.dto.j1.login.SelectTenantDTO;
import com.zeroone.star.project.query.j1.login.HospitalQuery;
import com.zeroone.star.project.query.j1.login.TenantQuery;
import com.zeroone.star.project.vo.j1.login.HospitalVO;
import com.zeroone.star.project.vo.j1.login.SelectResultVO;
import com.zeroone.star.project.vo.j1.login.TenantVO;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@Tag(name = "租户医院选择")
@RestController
public class TenantController implements LoginApi {

    @Override
    public JsonVO<List<TenantVO>> getTenantList(TenantQuery query) {
        return null;
    }

    @Override
    public JsonVO<SelectResultVO> selectTenant(SelectTenantDTO dto) {
        return null;
    }

    @Override
    public JsonVO<List<HospitalVO>> getHospitalList(HospitalQuery query) {
        return null;
    }

    @Override
    public JsonVO<SelectResultVO> selectHospital(SelectHospitalDTO dto) {
        return null;
    }
}