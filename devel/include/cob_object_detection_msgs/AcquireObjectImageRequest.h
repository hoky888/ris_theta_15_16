// Generated by gencpp from file cob_object_detection_msgs/AcquireObjectImageRequest.msg
// DO NOT EDIT!


#ifndef COB_OBJECT_DETECTION_MSGS_MESSAGE_ACQUIREOBJECTIMAGEREQUEST_H
#define COB_OBJECT_DETECTION_MSGS_MESSAGE_ACQUIREOBJECTIMAGEREQUEST_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PoseStamped.h>

namespace cob_object_detection_msgs
{
template <class ContainerAllocator>
struct AcquireObjectImageRequest_
{
  typedef AcquireObjectImageRequest_<ContainerAllocator> Type;

  AcquireObjectImageRequest_()
    : object_name()
    , reset_image_counter(false)
    , pose()
    , sdh_joints()  {
    }
  AcquireObjectImageRequest_(const ContainerAllocator& _alloc)
    : object_name(_alloc)
    , reset_image_counter(false)
    , pose(_alloc)
    , sdh_joints(_alloc)  {
    }



   typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _object_name_type;
  _object_name_type object_name;

   typedef uint8_t _reset_image_counter_type;
  _reset_image_counter_type reset_image_counter;

   typedef  ::geometry_msgs::PoseStamped_<ContainerAllocator>  _pose_type;
  _pose_type pose;

   typedef std::vector< ::geometry_msgs::PoseStamped_<ContainerAllocator> , typename ContainerAllocator::template rebind< ::geometry_msgs::PoseStamped_<ContainerAllocator> >::other >  _sdh_joints_type;
  _sdh_joints_type sdh_joints;




  typedef boost::shared_ptr< ::cob_object_detection_msgs::AcquireObjectImageRequest_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::cob_object_detection_msgs::AcquireObjectImageRequest_<ContainerAllocator> const> ConstPtr;

}; // struct AcquireObjectImageRequest_

typedef ::cob_object_detection_msgs::AcquireObjectImageRequest_<std::allocator<void> > AcquireObjectImageRequest;

typedef boost::shared_ptr< ::cob_object_detection_msgs::AcquireObjectImageRequest > AcquireObjectImageRequestPtr;
typedef boost::shared_ptr< ::cob_object_detection_msgs::AcquireObjectImageRequest const> AcquireObjectImageRequestConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::cob_object_detection_msgs::AcquireObjectImageRequest_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::cob_object_detection_msgs::AcquireObjectImageRequest_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace cob_object_detection_msgs

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': False}
// {'std_msgs': ['/opt/ros/indigo/share/std_msgs/cmake/../msg'], 'sensor_msgs': ['/opt/ros/indigo/share/sensor_msgs/cmake/../msg'], 'actionlib_msgs': ['/opt/ros/indigo/share/actionlib_msgs/cmake/../msg'], 'geometry_msgs': ['/opt/ros/indigo/share/geometry_msgs/cmake/../msg'], 'cob_object_detection_msgs': ['/home/team_theta/ROS/devel/share/cob_object_detection_msgs/msg', '/home/team_theta/ROS/src/cob_perception_common/cob_object_detection_msgs/msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::cob_object_detection_msgs::AcquireObjectImageRequest_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::cob_object_detection_msgs::AcquireObjectImageRequest_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::cob_object_detection_msgs::AcquireObjectImageRequest_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::cob_object_detection_msgs::AcquireObjectImageRequest_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::cob_object_detection_msgs::AcquireObjectImageRequest_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::cob_object_detection_msgs::AcquireObjectImageRequest_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::cob_object_detection_msgs::AcquireObjectImageRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "a834da64b488488418ecf10d2737befd";
  }

  static const char* value(const ::cob_object_detection_msgs::AcquireObjectImageRequest_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xa834da64b4884884ULL;
  static const uint64_t static_value2 = 0x18ecf10d2737befdULL;
};

template<class ContainerAllocator>
struct DataType< ::cob_object_detection_msgs::AcquireObjectImageRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "cob_object_detection_msgs/AcquireObjectImageRequest";
  }

  static const char* value(const ::cob_object_detection_msgs::AcquireObjectImageRequest_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::cob_object_detection_msgs::AcquireObjectImageRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "string object_name\n\
bool reset_image_counter\n\
geometry_msgs/PoseStamped pose\n\
geometry_msgs/PoseStamped[] sdh_joints\n\
\n\
================================================================================\n\
MSG: geometry_msgs/PoseStamped\n\
# A Pose with reference coordinate frame and timestamp\n\
Header header\n\
Pose pose\n\
\n\
================================================================================\n\
MSG: std_msgs/Header\n\
# Standard metadata for higher-level stamped data types.\n\
# This is generally used to communicate timestamped data \n\
# in a particular coordinate frame.\n\
# \n\
# sequence ID: consecutively increasing ID \n\
uint32 seq\n\
#Two-integer timestamp that is expressed as:\n\
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')\n\
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')\n\
# time-handling sugar is provided by the client library\n\
time stamp\n\
#Frame this data is associated with\n\
# 0: no frame\n\
# 1: global frame\n\
string frame_id\n\
\n\
================================================================================\n\
MSG: geometry_msgs/Pose\n\
# A representation of pose in free space, composed of postion and orientation. \n\
Point position\n\
Quaternion orientation\n\
\n\
================================================================================\n\
MSG: geometry_msgs/Point\n\
# This contains the position of a point in free space\n\
float64 x\n\
float64 y\n\
float64 z\n\
\n\
================================================================================\n\
MSG: geometry_msgs/Quaternion\n\
# This represents an orientation in free space in quaternion form.\n\
\n\
float64 x\n\
float64 y\n\
float64 z\n\
float64 w\n\
";
  }

  static const char* value(const ::cob_object_detection_msgs::AcquireObjectImageRequest_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::cob_object_detection_msgs::AcquireObjectImageRequest_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.object_name);
      stream.next(m.reset_image_counter);
      stream.next(m.pose);
      stream.next(m.sdh_joints);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER;
  }; // struct AcquireObjectImageRequest_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::cob_object_detection_msgs::AcquireObjectImageRequest_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::cob_object_detection_msgs::AcquireObjectImageRequest_<ContainerAllocator>& v)
  {
    s << indent << "object_name: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.object_name);
    s << indent << "reset_image_counter: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.reset_image_counter);
    s << indent << "pose: ";
    s << std::endl;
    Printer< ::geometry_msgs::PoseStamped_<ContainerAllocator> >::stream(s, indent + "  ", v.pose);
    s << indent << "sdh_joints[]" << std::endl;
    for (size_t i = 0; i < v.sdh_joints.size(); ++i)
    {
      s << indent << "  sdh_joints[" << i << "]: ";
      s << std::endl;
      s << indent;
      Printer< ::geometry_msgs::PoseStamped_<ContainerAllocator> >::stream(s, indent + "    ", v.sdh_joints[i]);
    }
  }
};

} // namespace message_operations
} // namespace ros

#endif // COB_OBJECT_DETECTION_MSGS_MESSAGE_ACQUIREOBJECTIMAGEREQUEST_H
