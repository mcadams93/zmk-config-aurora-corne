target_sources_ifdef(CONFIG_ZMK_INPUT_PROCESSOR_TAP_CLICK app PRIVATE src/input_processor_tap_click.c)

if(CONFIG_ZMK_CORNE_STATUS_SCREEN)
  target_sources(app PRIVATE
    src/display/custom_status_screen.c
    src/display/widgets/status.c
    src/display/widgets/util.c
    src/display/widgets/bolt.c
  )
endif()
