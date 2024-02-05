include $(TOPDIR)/rules.mk

PKG_NAME:=argp
PKG_RELEASE:=1
PKG_VERSION:=1.0.0
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/xhebox/libuargp
PKG_SOURCE_VERSION:=1f92296a97ef8fcfeb5e2440f5a62081fe654f75

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
	CATEGORY:=Libraries
	TITLE:=$(PKG_NAME)
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libargp.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/*.h $(1)/usr/include
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libargp.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/*.h $(1)/usr/include
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
