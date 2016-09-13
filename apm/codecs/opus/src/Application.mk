#--------------------------------------#
#        Sys-build Version: 1.3        #
#--------------------------------------#


# Point to a script file.the APP_PROJECT_PATH to record current path.
APP_BUILD_SCRIPT := $(APP_PROJECT_PATH)/make.mk
APP_OPTIM := release

# define output directory ,the defalut as follow: 
# /home2/lichao/work/source/projects/Ipcv7/webrtc/src/third_party/opus/out.
APP_OUTPUT_DIR := out

#APP_DEBUG_MODULES := 1
APP_DEBUG_MODULES :=
APP_MODULES :=
APP_PLATFORM := x86
APP_ARCH := x86

# defined current ABI(Application Binary Interface),example 
# armeabi(embedded-application binary interface) or armeabi-v7a.
APP_ABI := x86
APP_BOARD := ipc121

# 这个变量总是被描述成一个完整的sysdev工程的顶层目录,即，如果目 
# 前只是在某个模块里面进行编译，也应该根据其相对sysdev的位置而计 
# 算出sysdev的顶层目录，并赋值给这个变量. 如果你就在完整的sysdev 
# 工程的顶层目录下进行编译，这个值默认就是当前目录，不需要再另外 
# 赋值. 
# 这个值之所以需要这么确定，是因为工程的sdk和linux_lsp的路径是通 
# 过该值计算出来的,如果该变量给的不正确，sys-build会给出警告信息， 
# 如果你确定当前模块不需要使用SDK/LINUX_LSP等路径，可以忽略这些警 
# 告.
APP_WORKSPACE := $(APP_PROJECT_PATH)

# SDK相对sysdev的路径，每个平台有所差别，统一提供给需要的模块使用模 
# 块本身不应该覆盖定义该变量，以免造成其他模块无法正常引用该变量.
APP_SDK_DIR := $(APP_PROJECT_PATH)

# Linux_lsp相对sysdev的路径，统一提供给需要的模块使用模块本身不应该 
# 覆盖定义该变量，以免造成其他模块无法正常引用该变量.
APP_LSP_DIR := $(APP_PROJECT_PATH)

# 发布路径的顶层目录，默认就存放在APP_WORKSPACE之下各个模块需要根据 
#《版本发布说明》当中的规则，自行构建自身的发布结构，比如 
# 29120(APP_RELEASE_DIR)/cbb/sysdbg/include
APP_RELEASE_DIR := $(APP_WORKSPACE)/release

# Reserved function.
APP_STL :=

# Specifyed toolchain sysroot dirctory, 
# it's equivalent to '--sysroot' option.
APP_TOOLCHAIN_SYSROOT :=

# defiend current Cross-compilation prefix, 
# example 'APP_X86_TOOLCHAIN := /opt/x86/bin/x86-'.
APP_X86_TOOLCHAIN :=
