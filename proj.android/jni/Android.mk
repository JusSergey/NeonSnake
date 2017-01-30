LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := hellocpp/main.cpp \
  ../../Classes/Jus.cpp \
  ../../Classes/AppDelegate.cpp \
  ../../Classes/MyBodyParser.cpp \
  ../../Classes/Layers/PregameSwitchTypeGameLayer.cpp \
  ../../Classes/Layers/PregameSettingLayer.cpp \
  ../../Classes/Layers/NetworkSettingLayer.cpp \
  ../../Classes/Layers/GameNavigatorLayer.cpp \
  ../../Classes/Layers/LevelLayer.cpp \
  ../../Classes/Layers/PauseLayer.cpp \
  ../../Classes/Scenes/GameView.cpp \
  ../../Classes/Scenes/MenuScene.cpp \
  ../../Classes/Scenes/TitleScene.cpp \
  ../../Classes/Scenes/GameOverScene.cpp \
  ../../Classes/Scenes/LevelPresentation.cpp \
  ../../Classes/Network/Sockets/TcpSocket.cpp \
  ../../Classes/Network/Sockets/TcpServer.cpp \
  ../../Classes/Network/Sockets/TcpClient.cpp \
  ../../Classes/Network/GameServer.cpp \
  ../../Classes/Network/GameClient.cpp \
  ../../Classes/Network/PlayerData.cpp \
  ../../Classes/Actors/Bot.cpp \
  ../../Classes/Actors/Player.cpp \
  ../../Classes/Actors/Snake.cpp \
  ../../Classes/Actors/LocalPlayer.cpp \
  ../../Classes/Actions/Bonus.cpp \
  ../../Classes/Actions/NormalEat.cpp \
  ../../Classes/Actions/BasicEat.cpp \
  ../../Classes/Sound/Audio.cpp \
  ../../Classes/Data/DataSetting.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
