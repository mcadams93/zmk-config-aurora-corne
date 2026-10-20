# Compat shim: force-include a header that redefines INPUT_CALLBACK_DEFINE
# to accept the 3-arg form used by tokyo2006/cirque-input-module.
# Only apply to C files (not assembly).
zephyr_compile_options(
  $<$<COMPILE_LANGUAGE:C>:-include>
  $<$<COMPILE_LANGUAGE:C>:${CMAKE_CURRENT_SOURCE_DIR}/compat_module/compat_input.h>
)
