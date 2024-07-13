QT       += core gui sql printsupport



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    category_model.cpp \
    contractors_model.cpp \
    database.cpp \
    goods_model.cpp \
    information_category.cpp \
    information_goods.cpp \
    information_order.cpp \
    information_realization.cpp \
    information_shipment.cpp \
    main.cpp \
    mainwindow.cpp \
    orders_goods_model.cpp \
    orders_model.cpp \
    realization_goods_shipment_model.cpp \
    realization_model.cpp \
    reportdialog.cpp \
    searhform.cpp \
    shipment_goods_model.cpp \
    shipment_model.cpp \
    status_goods_model.cpp \
    status_realization_model.cpp \
    units_of_measurement_model.cpp

HEADERS += \
    category_model.h \
    contractors_model.h \
    database.h \
    goods_model.h \
    information_category.h \
    information_goods.h \
    information_order.h \
    information_realization.h \
    information_shipment.h \
    mainwindow.h \
    orders_goods_model.h \
    orders_model.h \
    realization_goods_shipment_model.h \
    realization_model.h \
    reportdialog.h \
    searhform.h \
    shipment_goods_model.h \
    shipment_model.h \
    status_goods_model.h \
    status_realization_model.h \
    units_of_measurement_model.h

FORMS += \
    information_category.ui \
    information_goods.ui \
    information_order.ui \
    information_realization.ui \
    information_shipment.ui \
    mainwindow.ui \
    reportdialog.ui \
    searhform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc


# Включите libxlsxwriter
INCLUDEPATH += /usr/local/include
LIBS += -lxlsxwriter

# Включите libxml2
INCLUDEPATH += /usr/include/libxml2
LIBS += -lxml2
