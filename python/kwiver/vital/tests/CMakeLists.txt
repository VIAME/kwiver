#
# Python support
#

# Define modules with tests in them
set(__testable_modnames
  test_algorithm_factory
  test_activity_type
  test_camera_intrinsics
  test_config
  test_camera_map
  test_color
  test_covariance
  test_descriptor_set
  test_detected_object_type
  test_eigen_numpy
  test_feature
  test_homography
  test_image_container
  test_landmark
  test_object_track_state
  test_object_track_set
  test_rotation
  test_similarity
  test_timestamp
  test_track
  test_track_set
  test_track_state
  )

if(KWIVER_ENABLE_SPROKIT AND KWIVER_ENABLE_ARROWS)
  # ImageObjectDetector test requires config bindings in sprokit
  list(APPEND
      __testable_modnames
      test_detected_object_set_input
      test_image_object_detector
    )
endif()

# All python filenames_we in the vital.tests module
set(__all_modnames
  helpers
  ${__testable_modnames}
  )

# Add files to the vital.tests python module
foreach (modname IN LISTS __all_modnames)
  kwiver_add_python_module( ${CMAKE_CURRENT_SOURCE_DIR}/${modname}.py
    vital/tests
    ${modname})
endforeach()

# Add python vital.tests to CTest if python test runner is available
if (KWIVER_ENABLE_TESTS AND NOSE_RUNNER)
  if (WIN32)
    set(WIN_TEST_CONFIG_TYPE "$<CONFIG>/")
  endif()

  include(kwiver-setup-tests-python)
    foreach (test IN LISTS __testable_modnames)
      kwiver_add_nosetests(${test} python)
    endforeach()

endif()
