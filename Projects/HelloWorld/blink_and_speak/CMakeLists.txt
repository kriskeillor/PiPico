if (TARGET tinyusb_device) 
    add_executable(blink_and_speak 
        blink_and_speak.c)

target_link_libraries(blink_and_speak 
    pico_stdlib)

pico_enable_stdio_usb(blink_and_speak 1)
pico_enable_stdio_uart(blink_and_speak 0)

pico_add_extra_outputs(blink_and_speak)

elseif(PICO_ON_DEVICE)
    message(WARNING "Not building blink_and_speak because TinyUSB submodule is not initialized in the SDK")
endif()