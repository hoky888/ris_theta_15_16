// Generated by gencpp from file ferns_ros/ModelStatus.msg
// DO NOT EDIT!


#ifndef FERNS_ROS_MESSAGE_MODELSTATUS_H
#define FERNS_ROS_MESSAGE_MODELSTATUS_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <std_msgs/Header.h>

namespace ferns_ros
{
template <class ContainerAllocator>
struct ModelStatus_
{
  typedef ModelStatus_<ContainerAllocator> Type;

  ModelStatus_()
    : header()
    , model()
    , loaded(false)
    , active(false)  {
    }
  ModelStatus_(const ContainerAllocator& _alloc)
    : header(_alloc)
    , model(_alloc)
    , loaded(false)
    , active(false)  {
    }



   typedef  ::std_msgs::Header_<ContainerAllocator>  _header_type;
  _header_type header;

   typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _model_type;
  _model_type model;

   typedef uint8_t _loaded_type;
  _loaded_type loaded;

   typedef uint8_t _active_type;
  _active_type active;




  typedef boost::shared_ptr< ::ferns_ros::ModelStatus_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::ferns_ros::ModelStatus_<ContainerAllocator> const> ConstPtr;

}; // struct ModelStatus_

typedef ::ferns_ros::ModelStatus_<std::allocator<void> > ModelStatus;

typedef boost::shared_ptr< ::ferns_ros::ModelStatus > ModelStatusPtr;
typedef boost::shared_ptr< ::ferns_ros::ModelStatus const> ModelStatusConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::ferns_ros::ModelStatus_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::ferns_ros::ModelStatus_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace ferns_ros

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': True}
// {'std_msgs': ['/opt/ros/indigo/share/std_msgs/cmake/../msg'], 'ferns_ros': ['/home/team_theta/ROS/src/vicos_ros-master/ferns_ros/msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::ferns_ros::ModelStatus_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::ferns_ros::ModelStatus_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::ferns_ros::ModelStatus_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::ferns_ros::ModelStatus_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::ferns_ros::ModelStatus_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::ferns_ros::ModelStatus_<ContainerAllocator> const>
  : TrueType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::ferns_ros::ModelStatus_<ContainerAllocator> >
{
  static const char* value()
  {
    return "6f51fe3ba1a4dacfef45b8036b90a8d3";
  }

  static const char* value(const ::ferns_ros::ModelStatus_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x6f51fe3ba1a4dacfULL;
  static const uint64_t static_value2 = 0xef45b8036b90a8d3ULL;
};

template<class ContainerAllocator>
struct DataType< ::ferns_ros::ModelStatus_<ContainerAllocator> >
{
  static const char* value()
  {
    return "ferns_ros/ModelStatus";
  }

  static const char* value(const ::ferns_ros::ModelStatus_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::ferns_ros::ModelStatus_<ContainerAllocator> >
{
  static const char* value()
  {
    return "Header 								header\n\
string                              model\n\
bool                             loaded\n\
bool                             active\n\
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
";
  }

  static const char* value(const ::ferns_ros::ModelStatus_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::ferns_ros::ModelStatus_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.header);
      stream.next(m.model);
      stream.next(m.loaded);
      stream.next(m.active);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER;
  }; // struct ModelStatus_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::ferns_ros::ModelStatus_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::ferns_ros::ModelStatus_<ContainerAllocator>& v)
  {
    s << indent << "header: ";
    s << std::endl;
    Printer< ::std_msgs::Header_<ContainerAllocator> >::stream(s, indent + "  ", v.header);
    s << indent << "model: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.model);
    s << indent << "loaded: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.loaded);
    s << indent << "active: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.active);
  }
};

} // namespace message_operations
} // namespace ros

#endif // FERNS_ROS_MESSAGE_MODELSTATUS_H
