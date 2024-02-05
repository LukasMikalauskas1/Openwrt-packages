include $(TOPDIR)/rules.mk

PKG_NAME:=tuya-sdk
PKG_RELEASE:=1
PKG_VERSION:=1.0.0
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/tuya/tuya-iot-core-sdk
PKG_SOURCE_VERSION:=0208badefb2003d30e70b5b875395e3187c850d9

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

CMAKE_INSTALL:=1
CMAKE_BINARY_SUBDIR:=build

define Package/$(PKG_NAME)
	CATEGORY:=Libraries
	TITLE:=$(PKG_NAME)
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/lib/liblink_core.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/lib/libmiddleware_implementation.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/lib/libplatform_port.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/lib/libutils_modules.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/utils/*.h $(1)/usr/include
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/include/*.h $(1)/usr/include
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/interface/*.h $(1)/usr/include
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/lib/liblink_core.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/lib/libmiddleware_implementation.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/lib/libplatform_port.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/lib/libutils_modules.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/utils/*.h $(1)/usr/include
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/include/*.h $(1)/usr/include
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/interface/*.h $(1)/usr/include
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
