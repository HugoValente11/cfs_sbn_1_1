project(CFE_DATAVIEW_LIB C)

# Create the app module
# add_cfe_app(sample_lib fsw/src/sample_lib.c)
add_cfe_app(dataview_lib fsw/src/dataview_init.c fsw/src/dataview-uniq.c 
fsw/src/debug_messages.c fsw/src/PrintTypes.c fsw/src/timeInMS.c 
fsw/src/asn1crt_encoding_acn.c fsw/src/asn1crt_encoding_uper.c 
fsw/src/asn1crt_encoding.c fsw/src/asn1crt.c
   fsw/public_inc/dataview-uniq.h
   fsw/public_inc/debug_messages.h
   fsw/public_inc/PrintTypes.h
   fsw/public_inc/timeInMS.h
   fsw/public_inc/asn1crt_encoding_acn.h
   fsw/public_inc/asn1crt_encoding_uper.h
   fsw/public_inc/asn1crt_encoding.h
   fsw/public_inc/asn1crt.h
   )
# add_cfe_app(sample_lib fsw/src/dataview-uniq.c)

# The API to this library (which may be invoked/referenced from other apps)
# is stored in fsw/public_inc.  Using "target_include_directories" is the 
# preferred method of indicating this (vs. directory-scope "include_directories").
target_include_directories(dataview_lib PUBLIC fsw/public_inc)
# target_include_directories(sample_lib PUBLIC fsw/public_inc PUBLIC fsw/src)
# target_include_directories(sample_lib PUBLIC fsw/src)


if (ENABLE_UNIT_TESTS)
  add_subdirectory(ut-stubs)
  add_subdirectory(unit-test)
endif (ENABLE_UNIT_TESTS)


