LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := hellocpp/main.cpp \
  ../../Classes/AppDelegate.cpp \
 \
  ../../Classes/Scenes/TitleScene.cpp \
  ../../Classes/Scenes/GameView.cpp \
  ../../Classes/Scenes/GameOverScene.cpp \
  ../../Classes/Scenes/LevelPresentation.cpp \
  ../../Classes/Scenes/MenuScene.cpp \
 \
  ../../Classes/Actions/BasicEat.cpp \
  ../../Classes/Actions/Bonus.cpp \
  ../../Classes/Actions/NormalEat.cpp \
 \
  ../../Classes/Actors/Snake.cpp \
  ../../Classes/Actors/Player.cpp \
  ../../Classes/Actors/LocalPlayer.cpp \
  ../../Classes/Actors/Bot.cpp \
 \
  ../../Classes/Layers/GameNavigatorLayer.cpp \
  ../../Classes/Layers/LevelLayer.cpp \
  ../../Classes/Layers/NetworkSettingLayer.cpp \
  ../../Classes/Layers/PauseLayer.cpp \
  ../../Classes/Layers/PregameSettingLayer.cpp \
  ../../Classes/Layers/PregameSwitchTypeGameLayer.cpp \
 \
  ../../Classes/Network/Sockets/TcpSocket.cpp \
  ../../Classes/Network/Sockets/TcpServer.cpp \
  ../../Classes/Network/Sockets/TcpClient.cpp \
  ../../Classes/Network/GameClient.cpp \
  ../../Classes/Network/GameServer.cpp \
  ../../Classes/Network/PlayerData.cpp \
 \
  ../../Classes/Sound/Audio.cpp \
 \
  ../../Classes/Data/DataSetting.cpp \
  ../../Classes/Data/SendData.cpp \
 \
  ../../Classes/Jus.cpp \
  ../../Classes/MyBodyParser.cpp

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
