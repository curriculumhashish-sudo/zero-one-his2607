# CMakeList.txt: test-sms 的 CMake 项目，在此处包括源代码并定义项目特定的逻辑。

# AliyunSms单元测试
if(USE_ALISMS)
	# 设置文件名称
	set (talisms test-alisms)
	# 添加可执行文件
	add_executable (${talisms} "test-alisms.cpp" "AliKeyConfig.cpp")
	# 链接库
	if (UNIX)
		target_link_libraries (${talisms} alibabacloud-sdk-core alibabacloud-sdk-dysmsapi nacos-cli)
	endif()
	message (STATUS "${talisms}: link aliyun sms lib")
endif()

# AliyunPns单元测试
if(USE_ALISMS)
	# 设置文件名称
	set (talipns test-alipns)
	# 添加可执行文件
	add_executable (${talipns} "test-alipns.cpp" "AliKeyConfig.cpp")
	# 链接库
	if (UNIX)
		target_link_libraries (${talipns} alibabacloud-sdk-core alibabacloud-sdk-dypnsapi nacos-cli)
	endif()
	message (STATUS "${talipns}: link aliyun pns lib")
endif()