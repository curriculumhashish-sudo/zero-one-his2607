# CMakeList.txt: test-mongo 的 CMake 项目，在此处包括源代码并定义项目特定的逻辑。

# MongoDB单元测试
if(USE_MONGO)
	# 设置文件名称
	set (tmongo test-mongo)
	# 添加可执行文件
	add_executable (${tmongo} "test-mongo.cpp")
	# 链接库
	if (WIN32)
		if(ZO_BT STREQUAL "r")
			target_link_libraries (${tmongo} bsoncxx-v_noabi-rhi-md mongocxx-v_noabi-rhi-md )
		else()
			target_link_libraries (${tmongo} bsoncxx-v_noabi-dhi-mdd mongocxx-v_noabi-dhi-mdd )
		endif()
	else()
		target_link_libraries (${tmongo} z resolv mongocxx-static mongoc2 bsoncxx-static bson2 utf8proc nacos-cli )
	endif()
	message (STATUS "${tmongo}: link mongodb lib")
endif()