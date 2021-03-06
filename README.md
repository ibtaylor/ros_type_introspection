# Ros Message Introspection
### Or... "If you don't know why you need it, probably you don't need it".

This simple library extracts information from a ROS Message, even when its
type is unknown at compilation time. 

Have you ever wanted to build an app that can subscribe to __any__ 
`topic` and extract its content, or can read data from __any__ `rosbag`? 
What if the topic and/or the bag contains user defined ROS types ignored 
at compilation time?

The common solution in the ROS ecosystem is to use Python, that provides
the needed introspection. Tools, for instance, like __rqt_plot__ and __rqt_bag__ 
took this approach. This library implements a __C++ alternative__.

The library is composed of three main modules:

- __Parser__: it performs introspection of a ROS Message using the schema 
stored in `ros::message_traits::Definition`.
[Read more](../../wiki/The-Parser) 

- __Deserializer__:using the schema built by the parsed, it can extract the 
actual values from a raw message. 
[Read more.](../../wiki/The-Deserializer)

- __Renamer__: last but not least, the library offers as well an easy way to 
remap/rename the data using a simple set of rules. This can be very handy in multiple
scenarios that are very common in ROS. [Read more.](../../wiki/The-Renamer) 

This library is particularly useful to extract data from two type-erasing classes 
provided by ROS itself:

1. [topic_tools::ShapeShifter](http://docs.ros.org/diamondback/api/topic_tools/html/classtopic__tools_1_1ShapeShifter.html):
a type used to subscribe to any topic, regardless of the original type.

2. [rosbag::MessageInstance](http://docs.ros.org/diamondback/api/rosbag/html/c++/classrosbag_1_1MessageInstance.html):
the generic type commonly used to read data from a ROS bag.

# Is this project active or "abandonware"?
This project is considered DONE.
In other words, if you haven't seen any commit in the recent months/years, it is because it just works!

If you have any request for improvement or issue, just let me know.

# Some background
The ROS Message Types can be described as 
a [Interface Description Language](https://en.wikipedia.org/wiki/Interface_description_language).
This approach is very well known and commonly used on the web and in distributed systems in general.

A [rosmsg](http://wiki.ros.org/rosmsg) is defined by the user; an "IDL compiler", i.e. 
[gencpp](http://wiki.ros.org/gencpp), 
reads this schema and generates a header file that contains the source code that the user
shall include in his/her applications.
The inclusion of this header file is needed on both the publisher *and* the subscriber sides.

This approach provides strong and type-safe contracts between the producer and the consumer 
of the message and, additionally, is needed to implements a fast 
serialization / deserialization mechanism.

The only "problem" is that in very few use cases (for instance if you want to build
a plugin to [load ROS bags in MATLAB](https://github.com/bcharrow/matlab_rosbag)) 
you don't know in advance which ROS Messages you will need to read. 
Therefore, you won't be able to include the necessary header files.

# Acknowledgements
This library is inspired by these other libraries 
[matlab_rosbag](https://github.com/bcharrow/matlab_rosbag) and 
[cpp_introspection](https://github.com/tu-darmstadt-ros-pkg/cpp_introspection).
   




 



 
