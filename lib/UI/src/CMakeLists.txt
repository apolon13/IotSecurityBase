SET(SOURCES components/ui_comp_controlitem.c
    components/ui_comp.c
    screens/ui_lockScreen.c
    screens/ui_home.c
    screens/ui_mqttConfigScreen.c
    screens/ui_wifiConfigScreen.c
    screens/ui_sensorsScreen.c
    screens/ui_edittSensorScreen.c
    screens/ui_generalSettingsScreen.c
    screens/ui_controlsScreen.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    ui_events.cpp
    images/ui_img_1515518950.c
    images/ui_img_548092219.c
    images/ui_img_setting_png.c
    images/ui_img_padlock_png.c)

add_library(ui ${SOURCES})
