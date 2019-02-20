#-------------------------------------------------
#
# Project created by QtCreator 2016-05-12T11:19:15
#
#-------------------------------------------------

QT     +=network widgets\
            multimedia\
            sql\
            axcontainer
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET =SPPServer
TEMPLATE = app


SOURCES += main.cpp\
        login.cpp \
    clientsocket.cpp \
    readcard.cpp \
    operatefile.cpp \
    homepage.cpp \
    purchase.cpp \
    unionpay.cpp \
    cashpay.cpp \
    msgconfirm.cpp \
    acceptcash.cpp \
    cashconfirm.cpp \
    dealfinish.cpp \
    inputpassword.cpp \
    inputamount.cpp \
    common/commondialog.cpp \
    manager.cpp \
    lookup.cpp \
    khmx.cpp \
    writecard.cpp \
    writecardlist.cpp \
    ydmx.cpp \
    first.cpp \
    src/qlightboxwidget.cpp \
    keyboard/QKeyPushButton.cpp \
    keyboard/widgetKeyBoard.cpp \
    heater/heaterhomepage.cpp \
    heater/heaterlookup.cpp \
    heater/readnum.cpp \
    heater/heatermsgconfirm.cpp \
    heater/heaterinputmaount.cpp \
    heater/heaterkhmx.cpp \
    heater/heaterdealfinished.cpp \
    src/effect.cpp \
    khxx.cpp \
    devlist.cpp \
    src/unionthread.cpp \
    accountdatabase.cpp \
    src/timertask.cpp \
    src/heartbeattask.cpp \
    chiocedialog.cpp \
    writecardfinished.cpp \
    choiceuser.cpp \
    webpage.cpp \
    advertisement/adwidget.cpp \
    common/commonwidget.cpp \
    ykgl/jmsh1.cpp \
    ykgl/jmsh2.cpp \
    ykgl/jmsh3.cpp \
    ykgl/jmsh4.cpp \
    ykgl/zdbdhomepage.cpp

HEADERS  += login.h \
    clientsocket.h \
    readcard.h \
    operatefile.h \
    homepage.h \
    purchase.h \
    unionpay.h \
    cashpay.h \
    msgconfirm.h \
    acceptcash.h \
    cashconfirm.h \
    dealfinish.h \
    inputpassword.h \
    inputamount.h \
    usermsg.h \
    common/commondialog.h \
    manager.h \
    lookup.h \
    khmx.h \
    writecard.h \
    writecardlist.h \
    ydmx.h \
    first.h \
    src/qlightboxwidget.h \
    keyboard/QKeyPushButton.h \
    keyboard/widgetKeyBoard.h \
    heater/heaterhomepage.h \
    heater/heaterlookup.h \
    heater/readnum.h \
    heater/heatermsgconfirm.h \
    heater/heaterinputmaount.h \
    heater/heaterkhmx.h \
    heater/heaterdealfinished.h \
    src/effect.h \
    khxx.h \
    devlist.h \
    src/unionthread.h \
    accountdatabase.h \
    src/timertask.h \
    src/heartbeattask.h \
    chiocedialog.h \
    writecardfinished.h \
    choiceuser.h \
    webpage.h \
    advertisement/adwidget.h \
    common/commonwidget.h \
    ykgl/jmsh1.h \
    ykgl/jmsh2.h \
    ykgl/jmsh3.h \
    ykgl/jmsh4.h \
    ykgl/zdbdhomepage.h

FORMS    += login.ui \
    readcard.ui \
    homepage.ui \
    purchase.ui \
    unionpay.ui \
    cashpay.ui \
    msgconfirm.ui \
    acceptcash.ui \
    cashconfirm.ui \
    dealfinish.ui \
    inputpassword.ui \
    inputamount.ui \
    common/commondialog.ui \
    lookup.ui \
    khmx.ui \
    writecard.ui \
    writecardlist.ui \
    ydmx.ui \
    first.ui \
    heaterhomepage.ui \
    heaterlookup.ui \
    heater/readnum.ui \
    heater/heatermsgconfirm.ui \
    heater/heaterinputmaount.ui \
    heater/heaterkhmx.ui \
    heater/heaterdealfinished.ui \
    khxx.ui \
    devlist.ui \
    chiocedialog.ui \
    writecardfinished.ui \
    choiceuser.ui \
    webpage.ui \
    ykgl/jmsh1.ui \
    ykgl/jmsh2.ui \
    ykgl/jmsh3.ui \
    ykgl/jmsh4.ui \
    ykgl/zdbdhomepage.ui

RESOURCES += \
    screen.qrc
RC_FILE = sppserver.rc

include     ($$PWD/input2018/input2018.pri)
