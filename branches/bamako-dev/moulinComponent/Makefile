all: build
CXX    = c++
CPPFLAGS += -fno-rtti -fno-exceptions -shared -D_POSIX_ -O3
TEST_DEFINES = -D_POSIX_
GECKO_SDK_PATH = ../gecko-sdk
GECKO_DEFINES   = -DXPCOM_GLUE -DMOZILLA_STRICT_API
GECKO_IDL_INCLUDES = -I $(GECKO_SDK_PATH)/idl
GECKO_INCLUDES = -I  $(GECKO_SDK_PATH)/include
GECKO_LDFLAGS =   -L $(GECKO_SDK_PATH)/lib -lxpcomglue -lnspr4 -lplds4 -lembed_base_s

nsIMoulin.h: nsIMoulin.idl
	$(GECKO_SDK_PATH)/bin/xpidl -m header $(GECKO_IDL_INCLUDES) nsIMoulin.idl

nsMoulin.xpt: nsIMoulin.idl
	$(GECKO_SDK_PATH)/bin/xpidl -m typelib $(GECKO_IDL_INCLUDES) -o nsMoulin nsIMoulin.idl
	
nsMoulin.so: nsMoulin.cpp nsMoulinModule.cpp nsIMoulin.h nsMoulin.h 
	$(CXX) -o nsMoulin.so -DXPCOM_BUILD $(GECKO_DEFINES) $(GECKO_INCLUDES) $(GECKO_LDFLAGS) $(CPPFLAGS) $(CXXFLAGS) nsMoulin.cpp nsMoulinModule.cpp nsIMoulin.h nsMoulin.h /usr/lib/libbz2.a
	chmod +x nsMoulin.so

build: nsMoulin.so nsMoulin.xpt
	
clean:
	rm nsMoulin.so nsMoulin.xpt

install: nsMoulin.so nsMoulin.xpt
	cp nsMoulin.so ../components
	cp nsMoulin.xpt ../components
