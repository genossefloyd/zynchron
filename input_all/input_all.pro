#-------------------------------------------------
#
# Project created by QtCreator 2016-08-26T18:40:03
#
#-------------------------------------------------

QT       += core gui bluetooth network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = input_all
TEMPLATE = app


SOURCES += main.cpp\
        maingui.cpp \
    bluetoothconnector.cpp \
    deviceinfo.cpp \
    metaweardevice.cpp \
    core/cpp/datasignal.cpp \
    core/cpp/debug.cpp \
    core/cpp/event.cpp \
    core/cpp/logging.cpp \
    core/cpp/moduleinfo.cpp \
    core/cpp/responseheader.cpp \
    core/cpp/settings.cpp \
    core/cpp/timer.cpp \
    core/cpp/version.cpp \
    impl/cpp/datainterpreter.cpp \
    impl/cpp/metawearboard.cpp \
    peripheral/cpp/haptic.cpp \
    peripheral/cpp/ibeacon.cpp \
    peripheral/cpp/led.cpp \
    peripheral/cpp/neopixel.cpp \
    platform/cpp/async_creator.cpp \
    platform/cpp/memory.cpp \
    platform/cpp/task.cpp \
    platform/cpp/threadpool.cpp \
    processor/cpp/accumulator.cpp \
    processor/cpp/average.cpp \
    processor/cpp/buffer.cpp \
    processor/cpp/combiner.cpp \
    processor/cpp/comparator.cpp \
    processor/cpp/dataprocessor.cpp \
    processor/cpp/delta.cpp \
    processor/cpp/math.cpp \
    processor/cpp/passthrough.cpp \
    processor/cpp/pulse.cpp \
    processor/cpp/sample.cpp \
    processor/cpp/threshold.cpp \
    processor/cpp/time.cpp \
    sensor/cpp/accelerometer.cpp \
    sensor/cpp/accelerometer_bosch.cpp \
    sensor/cpp/accelerometer_mma8452q.cpp \
    sensor/cpp/ambientlight_ltr329.cpp \
    sensor/cpp/barometer_bosch.cpp \
    sensor/cpp/colordetector_tcs34725.cpp \
    sensor/cpp/gpio.cpp \
    sensor/cpp/gyro_bmi160.cpp \
    sensor/cpp/humidity_bme280.cpp \
    sensor/cpp/magnetometer_bmm150.cpp \
    sensor/cpp/multichanneltemperature.cpp \
    sensor/cpp/proximity_tsl2671.cpp \
    sensor/cpp/serialpassthrough.cpp \
    sensor/cpp/switch.cpp \
    mwdevicecontrol.cpp \
    socketoutput.cpp

HEADERS  += maingui.h \
    bluetoothconnector.h \
    deviceinfo.h \
    core/cpp/constant.h \
    core/cpp/datainterpreter.h \
    core/cpp/datasignal_private.h \
    core/cpp/event_private.h \
    core/cpp/event_register.h \
    core/cpp/logging_private.h \
    core/cpp/logging_register.h \
    core/cpp/metawearboard_def.h \
    core/cpp/metawearboard_macro.h \
    core/cpp/moduleinfo.h \
    core/cpp/register.h \
    core/cpp/responseheader.h \
    core/cpp/settings_private.h \
    core/cpp/settings_register.h \
    core/cpp/timer_private.h \
    core/cpp/timer_register.h \
    core/cpp/version.h \
    core/data.h \
    core/datasignal.h \
    core/datasignal_fwd.h \
    core/debug.h \
    core/event.h \
    core/event_fwd.h \
    core/logging.h \
    core/logging_fwd.h \
    core/metawearboard.h \
    core/metawearboard_fwd.h \
    core/module.h \
    core/settings.h \
    core/status.h \
    core/timer.h \
    core/timer_fwd.h \
    core/types.h \
    peripheral/haptic.h \
    peripheral/ibeacon.h \
    peripheral/led.h \
    peripheral/neopixel.h \
    peripheral/peripheral_common.h \
    platform/cpp/async_creator.h \
    platform/cpp/concurrent_queue.h \
    platform/cpp/task.h \
    platform/cpp/threadpool.h \
    platform/btle_connection.h \
    platform/dllmarker.h \
    platform/memory.h \
    processor/cpp/dataprocessor_private.h \
    processor/cpp/dataprocessor_register.h \
    processor/cpp/processor_private_common.h \
    processor/accumulator.h \
    processor/average.h \
    processor/buffer.h \
    processor/comparator.h \
    processor/counter.h \
    processor/dataprocessor.h \
    processor/dataprocessor_fwd.h \
    processor/delta.h \
    processor/math.h \
    processor/passthrough.h \
    processor/processor_common.h \
    processor/pulse.h \
    processor/rms.h \
    processor/rss.h \
    processor/sample.h \
    processor/threshold.h \
    processor/time.h \
    sensor/cpp/accelerometer_bosch_private.h \
    sensor/cpp/accelerometer_bosch_register.h \
    sensor/cpp/accelerometer_mma8452q_private.h \
    sensor/cpp/accelerometer_mma8452q_register.h \
    sensor/cpp/accelerometer_private.h \
    sensor/cpp/ambientlight_ltr329_private.h \
    sensor/cpp/ambientlight_ltr329_register.h \
    sensor/cpp/barometer_bosch_private.h \
    sensor/cpp/barometer_bosch_register.h \
    sensor/cpp/colordetector_tcs34725_private.h \
    sensor/cpp/colordetector_tcs34725_register.h \
    sensor/cpp/gpio_private.h \
    sensor/cpp/gpio_register.h \
    sensor/cpp/gyro_bmi160_private.h \
    sensor/cpp/gyro_bmi160_register.h \
    sensor/cpp/humidity_bme280_private.h \
    sensor/cpp/humidity_bme280_register.h \
    sensor/cpp/magnetometer_bmm150_private.h \
    sensor/cpp/magnetometer_bmm150_register.h \
    sensor/cpp/multichanneltemperature_private.h \
    sensor/cpp/multichanneltemperature_register.h \
    sensor/cpp/proximity_tsl2671_private.h \
    sensor/cpp/proximity_tsl2671_register.h \
    sensor/cpp/serialpassthrough_private.h \
    sensor/cpp/serialpassthrough_register.h \
    sensor/cpp/switch_private.h \
    sensor/cpp/switch_register.h \
    sensor/cpp/utils.h \
    sensor/accelerometer.h \
    sensor/accelerometer_bosch.h \
    sensor/accelerometer_mma8452q.h \
    sensor/ambientlight_ltr329.h \
    sensor/barometer_bosch.h \
    sensor/colordetector_tcs34725.h \
    sensor/gpio.h \
    sensor/gyro_bmi160.h \
    sensor/humidity_bme280.h \
    sensor/i2c.h \
    sensor/magnetometer_bmm150.h \
    sensor/multichanneltemperature.h \
    sensor/proximity_tsl2671.h \
    sensor/sensor_common.h \
    sensor/spi.h \
    sensor/switch.h \
    metaweardevice.h \
    mwdevicecontrol.h \
    socketoutput.h \
    messages.h

FORMS    += maingui.ui \
    mwdevicecontrol.ui
