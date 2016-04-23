#!/bin/sh

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
    DESTDIR_ARG="--root=$DESTDIR"
fi

echo_and_run() { echo "+ $@" ; "$@" ; }

echo_and_run cd "/home/team_theta/ROS/src/vicos_ros-master/facedetector"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
echo_and_run /usr/bin/env \
    PYTHONPATH="/home/team_theta/ROS/install/lib/python2.7/dist-packages:/home/team_theta/ROS/build/lib/python2.7/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/team_theta/ROS/build" \
    "/usr/bin/python" \
    "/home/team_theta/ROS/src/vicos_ros-master/facedetector/setup.py" \
    build --build-base "/home/team_theta/ROS/build/vicos_ros-master/facedetector" \
    install \
    $DESTDIR_ARG \
    --install-layout=deb --prefix="/home/team_theta/ROS/install" --install-scripts="/home/team_theta/ROS/install/bin"
