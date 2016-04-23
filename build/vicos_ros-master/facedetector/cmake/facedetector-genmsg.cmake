# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "facedetector: 1 messages, 0 services")

set(MSG_I_FLAGS "-Ifacedetector:/home/team_theta/ROS/src/vicos_ros-master/facedetector/msg;-Isensor_msgs:/opt/ros/indigo/share/sensor_msgs/cmake/../msg;-Istd_msgs:/opt/ros/indigo/share/std_msgs/cmake/../msg;-Igeometry_msgs:/opt/ros/indigo/share/geometry_msgs/cmake/../msg;-Ivisualization_msgs:/opt/ros/indigo/share/visualization_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(facedetector_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/team_theta/ROS/src/vicos_ros-master/facedetector/msg/Detection.msg" NAME_WE)
add_custom_target(_facedetector_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "facedetector" "/home/team_theta/ROS/src/vicos_ros-master/facedetector/msg/Detection.msg" "std_msgs/Header:sensor_msgs/Image"
)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(facedetector
  "/home/team_theta/ROS/src/vicos_ros-master/facedetector/msg/Detection.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/indigo/share/sensor_msgs/cmake/../msg/Image.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/facedetector
)

### Generating Services

### Generating Module File
_generate_module_cpp(facedetector
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/facedetector
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(facedetector_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(facedetector_generate_messages facedetector_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/team_theta/ROS/src/vicos_ros-master/facedetector/msg/Detection.msg" NAME_WE)
add_dependencies(facedetector_generate_messages_cpp _facedetector_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(facedetector_gencpp)
add_dependencies(facedetector_gencpp facedetector_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS facedetector_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(facedetector
  "/home/team_theta/ROS/src/vicos_ros-master/facedetector/msg/Detection.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/indigo/share/sensor_msgs/cmake/../msg/Image.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/facedetector
)

### Generating Services

### Generating Module File
_generate_module_lisp(facedetector
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/facedetector
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(facedetector_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(facedetector_generate_messages facedetector_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/team_theta/ROS/src/vicos_ros-master/facedetector/msg/Detection.msg" NAME_WE)
add_dependencies(facedetector_generate_messages_lisp _facedetector_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(facedetector_genlisp)
add_dependencies(facedetector_genlisp facedetector_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS facedetector_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(facedetector
  "/home/team_theta/ROS/src/vicos_ros-master/facedetector/msg/Detection.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/indigo/share/sensor_msgs/cmake/../msg/Image.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/facedetector
)

### Generating Services

### Generating Module File
_generate_module_py(facedetector
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/facedetector
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(facedetector_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(facedetector_generate_messages facedetector_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/team_theta/ROS/src/vicos_ros-master/facedetector/msg/Detection.msg" NAME_WE)
add_dependencies(facedetector_generate_messages_py _facedetector_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(facedetector_genpy)
add_dependencies(facedetector_genpy facedetector_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS facedetector_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/facedetector)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/facedetector
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
add_dependencies(facedetector_generate_messages_cpp sensor_msgs_generate_messages_cpp)
add_dependencies(facedetector_generate_messages_cpp std_msgs_generate_messages_cpp)
add_dependencies(facedetector_generate_messages_cpp geometry_msgs_generate_messages_cpp)
add_dependencies(facedetector_generate_messages_cpp visualization_msgs_generate_messages_cpp)

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/facedetector)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/facedetector
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
add_dependencies(facedetector_generate_messages_lisp sensor_msgs_generate_messages_lisp)
add_dependencies(facedetector_generate_messages_lisp std_msgs_generate_messages_lisp)
add_dependencies(facedetector_generate_messages_lisp geometry_msgs_generate_messages_lisp)
add_dependencies(facedetector_generate_messages_lisp visualization_msgs_generate_messages_lisp)

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/facedetector)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/facedetector\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/facedetector
    DESTINATION ${genpy_INSTALL_DIR}
    # skip all init files
    PATTERN "__init__.py" EXCLUDE
    PATTERN "__init__.pyc" EXCLUDE
  )
  # install init files which are not in the root folder of the generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/facedetector
    DESTINATION ${genpy_INSTALL_DIR}
    FILES_MATCHING
    REGEX "${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/facedetector/.+/__init__.pyc?$"
  )
endif()
add_dependencies(facedetector_generate_messages_py sensor_msgs_generate_messages_py)
add_dependencies(facedetector_generate_messages_py std_msgs_generate_messages_py)
add_dependencies(facedetector_generate_messages_py geometry_msgs_generate_messages_py)
add_dependencies(facedetector_generate_messages_py visualization_msgs_generate_messages_py)
