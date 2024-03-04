include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/package.mk

PKG_NAME:=tuya-esp-deamon
PKG_RELEASE:=1
PKG_VERSION:=1.0.0

define Package/$(PKG_NAME)
	CATEGORY:=Base system
	TITLE:=$(PKG_NAME)
	DEPENDS:= +tuya-sdk +libubus +libubox +libblobmsg-json +argp 
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tuyad $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/*h $(1)/usr/include
	$(INSTALL_BIN) ./files/tuyad.init $(1)/etc/init.d/tuyad
	$(INSTALL_CONF) ./files/tuyad.config $(1)/etc/config/tuyad
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tuyad $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/*h $(1)/usr/include
	$(INSTALL_BIN) ./files/tuyad.init $(1)/etc/init.d/tuyad
	$(INSTALL_CONF) ./files/tuyad.config $(1)/etc/config/tuyad
endef

$(eval $(call BuildPackage,$(PKG_NAME)))