// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: SquareLine_Project

#ifndef _SQUARELINE_PROJECT_UI_H
#define _SQUARELINE_PROJECT_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined __has_include
  #if __has_include("lvgl.h")
    #include "lvgl.h"
  #elif __has_include("lvgl/lvgl.h")
    #include "lvgl/lvgl.h"
  #else
    #include "lvgl.h"
  #endif
#else
  #include "lvgl.h"
#endif

#include "ui_helpers.h"
#include "components/ui_comp.h"
#include "components/ui_comp_hook.h"
#include "ui_events.h"
// SCREEN: ui_lockScreen
void ui_lockScreen_screen_init(void);
extern lv_obj_t *ui_lockScreen;
extern lv_obj_t *ui_TabView6;
extern lv_obj_t *ui_Label15;
extern lv_obj_t *ui_lockContainer;
extern lv_obj_t *ui_Panel3;
void ui_event_pincode( lv_event_t * e);
extern lv_obj_t *ui_pincode;
void ui_event_Button1( lv_event_t * e);
extern lv_obj_t *ui_Button1;
extern lv_obj_t *ui_Label20;
void ui_event_Keyboard2( lv_event_t * e);
extern lv_obj_t *ui_Keyboard2;
// SCREEN: ui_home
void ui_home_screen_init(void);
void ui_event_home( lv_event_t * e);
extern lv_obj_t *ui_home;
extern lv_obj_t *ui_TabView1;
extern lv_obj_t *ui_Label1;
extern lv_obj_t *ui_Container2;
extern lv_obj_t *ui_mqttHome;
extern lv_obj_t *ui_Label2;
void ui_event_mqttSettingsBtn( lv_event_t * e);
extern lv_obj_t *ui_mqttSettingsBtn;
extern lv_obj_t *ui_Label3;
extern lv_obj_t *ui_Spinner1;
extern lv_obj_t *ui_Container3;
extern lv_obj_t *ui_mqttAddrLabel;
extern lv_obj_t *ui_mqttCredentialsLabel;
extern lv_obj_t *ui_wifiHome;
extern lv_obj_t *ui_Label9;
void ui_event_wifiSettingsBtn( lv_event_t * e);
extern lv_obj_t *ui_wifiSettingsBtn;
extern lv_obj_t *ui_Label12;
extern lv_obj_t *ui_wifiSsidLabel;
extern lv_obj_t *ui_connectionStatusOff;
extern lv_obj_t *ui_connectionStatusOn;
extern lv_obj_t *ui_sensorsHome;
extern lv_obj_t *ui_SensorsLabel;
void ui_event_addSensorBtn( lv_event_t * e);
extern lv_obj_t *ui_addSensorBtn;
extern lv_obj_t *ui_Label13;
extern lv_obj_t *ui_sensorsQty;
extern lv_obj_t *ui_controlsHome;
extern lv_obj_t *ui_Label33;
void ui_event_AddControlBtn( lv_event_t * e);
extern lv_obj_t *ui_AddControlBtn;
extern lv_obj_t *ui_Label34;
void ui_event_goToGeneralSettingsBtn( lv_event_t * e);
extern lv_obj_t *ui_goToGeneralSettingsBtn;
void ui_event_lockBtn( lv_event_t * e);
extern lv_obj_t *ui_lockBtn;
// SCREEN: ui_mqttConfigScreen
void ui_mqttConfigScreen_screen_init(void);
void ui_event_mqttConfigScreen( lv_event_t * e);
extern lv_obj_t *ui_mqttConfigScreen;
extern lv_obj_t *ui_TabView2;
extern lv_obj_t *ui_Label7;
extern lv_obj_t *ui_Container1;
extern lv_obj_t *ui_Ip;
extern lv_obj_t *ui_BrokerIpInputLabel;
void ui_event_mqttIp( lv_event_t * e);
extern lv_obj_t *ui_mqttIp;
extern lv_obj_t *ui_Port;
extern lv_obj_t *ui_BrokerIpInputLabel1;
void ui_event_mqttPort( lv_event_t * e);
extern lv_obj_t *ui_mqttPort;
extern lv_obj_t *ui_Credentials;
extern lv_obj_t *ui_Label6;
void ui_event_mqttUsername( lv_event_t * e);
extern lv_obj_t *ui_mqttUsername;
void ui_event_mqttPassword( lv_event_t * e);
extern lv_obj_t *ui_mqttPassword;
void ui_event_mqttEntityId( lv_event_t * e);
extern lv_obj_t *ui_mqttEntityId;
void ui_event_saveMqttBtn( lv_event_t * e);
extern lv_obj_t *ui_saveMqttBtn;
void ui_event_Label4( lv_event_t * e);
extern lv_obj_t *ui_Label4;
void ui_event_abortBtn( lv_event_t * e);
extern lv_obj_t *ui_abortBtn;
extern lv_obj_t *ui_Label5;
void ui_event_Keyboard( lv_event_t * e);
extern lv_obj_t *ui_Keyboard;
// SCREEN: ui_wifiConfigScreen
void ui_wifiConfigScreen_screen_init(void);
void ui_event_wifiConfigScreen( lv_event_t * e);
extern lv_obj_t *ui_wifiConfigScreen;
extern lv_obj_t *ui_TabView3;
extern lv_obj_t *ui_Label8;
extern lv_obj_t *ui_Container4;
extern lv_obj_t *ui_ssid;
extern lv_obj_t *ui_wifiSsidInputLabel;
void ui_event_wifiSsid( lv_event_t * e);
extern lv_obj_t *ui_wifiSsid;
extern lv_obj_t *ui_Port1;
extern lv_obj_t *ui_wifiPassInputLabel;
void ui_event_wifiPassword( lv_event_t * e);
extern lv_obj_t *ui_wifiPassword;
void ui_event_saveWifiBtn( lv_event_t * e);
extern lv_obj_t *ui_saveWifiBtn;
void ui_event_Label10( lv_event_t * e);
extern lv_obj_t *ui_Label10;
void ui_event_abortBtn1( lv_event_t * e);
extern lv_obj_t *ui_abortBtn1;
extern lv_obj_t *ui_Label11;
void ui_event_Keyboard1( lv_event_t * e);
extern lv_obj_t *ui_Keyboard1;
// SCREEN: ui_sensorsScreen
void ui_sensorsScreen_screen_init(void);
void ui_event_sensorsScreen( lv_event_t * e);
extern lv_obj_t *ui_sensorsScreen;
extern lv_obj_t *ui_TabView4;
extern lv_obj_t *ui_Label14;
extern lv_obj_t *ui_Sensors;
void ui_event_sensorActionsContainer( lv_event_t * e);
extern lv_obj_t *ui_sensorActionsContainer;
void ui_event_abortBtn2( lv_event_t * e);
extern lv_obj_t *ui_abortBtn2;
extern lv_obj_t *ui_Label16;
void ui_event_startScanBtn( lv_event_t * e);
extern lv_obj_t *ui_startScanBtn;
extern lv_obj_t *ui_ScanLabel;
void ui_event_stopScanBtn( lv_event_t * e);
extern lv_obj_t *ui_stopScanBtn;
extern lv_obj_t *ui_Label18;
// SCREEN: ui_edittSensorScreen
void ui_edittSensorScreen_screen_init(void);
void ui_event_edittSensorScreen( lv_event_t * e);
extern lv_obj_t *ui_edittSensorScreen;
extern lv_obj_t *ui_TabView5;
extern lv_obj_t *ui_Label19;
extern lv_obj_t *ui_SensorContainer;
void ui_event_saveSensorBtn( lv_event_t * e);
extern lv_obj_t *ui_saveSensorBtn;
void ui_event_Label24( lv_event_t * e);
extern lv_obj_t *ui_Label24;
void ui_event_goBackBtn( lv_event_t * e);
extern lv_obj_t *ui_goBackBtn;
extern lv_obj_t *ui_Label22;
extern lv_obj_t *ui_Panel2;
void ui_event_sensorName( lv_event_t * e);
extern lv_obj_t *ui_sensorName;
extern lv_obj_t *ui_sensorIsActive;
extern lv_obj_t *ui_Container5;
extern lv_obj_t *ui_sensorSensitivity;
extern lv_obj_t *ui_Label17;
void ui_event_Keyboard3( lv_event_t * e);
extern lv_obj_t *ui_Keyboard3;
// SCREEN: ui_generalSettingsScreen
void ui_generalSettingsScreen_screen_init(void);
void ui_event_generalSettingsScreen( lv_event_t * e);
extern lv_obj_t *ui_generalSettingsScreen;
extern lv_obj_t *ui_TabView7;
extern lv_obj_t *ui_Label21;
extern lv_obj_t *ui_Container6;
extern lv_obj_t *ui_Container7;
extern lv_obj_t *ui_settingsPincode;
extern lv_obj_t *ui_pinlabel;
void ui_event_settingsPincodeInput( lv_event_t * e);
extern lv_obj_t *ui_settingsPincodeInput;
void ui_event_saveGeneralSettingsBtn( lv_event_t * e);
extern lv_obj_t *ui_saveGeneralSettingsBtn;
void ui_event_Label23( lv_event_t * e);
extern lv_obj_t *ui_Label23;
void ui_event_abortBtn3( lv_event_t * e);
extern lv_obj_t *ui_abortBtn3;
extern lv_obj_t *ui_Label25;
extern lv_obj_t *ui_Container8;
extern lv_obj_t *ui_settingsTimeout;
extern lv_obj_t *ui_settingsTimeoutLabel;
void ui_event_settingsTimeoutInput( lv_event_t * e);
extern lv_obj_t *ui_settingsTimeoutInput;
extern lv_obj_t *ui_Container9;
extern lv_obj_t *ui_settingsConnectionTimeout;
extern lv_obj_t *ui_settingsConnectionTimeoutLabel;
void ui_event_settingsConnectionTimeoutInput( lv_event_t * e);
extern lv_obj_t *ui_settingsConnectionTimeoutInput;
extern lv_obj_t *ui_Container10;
extern lv_obj_t *ui_settingsMaxAttempts;
extern lv_obj_t *ui_settingsMaxAttemptsLabel;
void ui_event_settingsMaxAttemptsInput( lv_event_t * e);
extern lv_obj_t *ui_settingsMaxAttemptsInput;
void ui_event_Keyboard4( lv_event_t * e);
extern lv_obj_t *ui_Keyboard4;
// SCREEN: ui_controlsScreen
void ui_controlsScreen_screen_init(void);
void ui_event_controlsScreen( lv_event_t * e);
extern lv_obj_t *ui_controlsScreen;
extern lv_obj_t *ui_TabView8;
extern lv_obj_t *ui_Label26;
extern lv_obj_t *ui_Controls;
void ui_event_controlActionsContainer( lv_event_t * e);
extern lv_obj_t *ui_controlActionsContainer;
void ui_event_abortBtn4( lv_event_t * e);
extern lv_obj_t *ui_abortBtn4;
extern lv_obj_t *ui_Label27;
void ui_event_addControlBtn( lv_event_t * e);
extern lv_obj_t *ui_addControlBtn;
extern lv_obj_t *ui_addControlLabel1;
extern lv_obj_t *ui____initial_actions0;

LV_IMG_DECLARE( ui_img_1515518950);   // assets/wifi (1).png
LV_IMG_DECLARE( ui_img_548092219);   // assets/wifi-connection.png
LV_IMG_DECLARE( ui_img_setting_png);   // assets/setting.png
LV_IMG_DECLARE( ui_img_padlock_png);   // assets/padlock.png



void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
