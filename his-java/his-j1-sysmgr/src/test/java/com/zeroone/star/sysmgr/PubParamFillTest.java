package com.zeroone.star.sysmgr;

import com.zeroone.star.sysmgr.entity.menu.MenuDO;
import com.zeroone.star.sysmgr.mapper.menu.MenusMapper;
import com.zeroone.star.sysmgr.mapper.permission.PermissionMapper;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;

@SpringBootTest
public class PubParamFillTest {

    @Autowired
    private MenusMapper menusMapper;

    @Test
    public void menusInsertTest() {
        MenuDO menuDO = new MenuDO();
        menuDO.setCode("testCode");
        menuDO.setName("testName");
        menuDO.setComponent("testComponent");
        menuDO.setMenuType("testMenuType");
        menuDO.setIcon("testIcon");
        menuDO.setDisplayName("testDisplayName");
        menuDO.setExternalUrl("testExternalUrl");
        menuDO.setIsEnabled(1);
        menuDO.setIsExternal(0);
        menuDO.setIsHidden(0);
        menuDO.setSortOrder(1);
        menuDO.setRemark("testRemark");
        menuDO.setTenantId("23214355");
        menuDO.setPermissionName("testPermissionName");
        menuDO.setKeepAlive(1);
        menuDO.setPath("testPath");
        menusMapper.insert(menuDO);
    }

    @Test
    public void menusUpdateTest() {
        MenuDO menuDO = new MenuDO();
        menuDO.setId("2080546739668418560");
        menuDO.setParentId("279418897187213312");
        menusMapper.updateById(menuDO);
    }
}
