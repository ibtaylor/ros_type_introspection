/*********************************************************************
* Software License Agreement (BSD License)
*
*  Copyright 2016-2017 Davide Faconti
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of Willow Garage, Inc. nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
* *******************************************************************/

#include "ros_type_introspection/ros_field.hpp"
#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/trim.hpp>

namespace RosIntrospection{

ROSField::ROSField(const std::string &definition):
  _array_size(1)
{
  //std::cerr << definition << std::endl;
  static const  boost::regex type_regex("[a-zA-Z][a-zA-Z0-9_]*"
                                        "(/[a-zA-Z][a-zA-Z0-9_]*){0,1}"
                                        "(\\[[0-9]*\\]){0,1}");

  static const  boost::regex field_regex("[a-zA-Z][a-zA-Z0-9_]*");

  static const boost::regex array_regex("(.+)\\[([0-9]*)\\]");

  using boost::regex;
  std::string::const_iterator begin = definition.begin();
  std::string::const_iterator end   = definition.end();
  boost::match_results<std::string::const_iterator> what;

  // Get type and field
  std::string type, fieldname, value;

  //-------------------------------
  // Find type, field and array size
  if( regex_search(begin, end, what, type_regex)) {
    type = what[0];
    begin = what[0].second;
  }
  else {
    throw std::runtime_error("Bad type when parsing message ----\n" + definition);
  }

  if (regex_search(begin, end, what, field_regex))
  {
    fieldname = what[0];
    begin = what[0].second;
  }
  else {
    throw std::runtime_error("Bad field when parsing message ----\n" + definition);
  }


  std::string type_tmp = type;
  if (regex_search(type_tmp, what, array_regex))
  {
    type = what[1];

    if (what.size() == 2) {
      _array_size = -1;
    }
    else if (what.size() == 3) {
      _array_size = what[2].first == what[2].second  ? -1 : std::stoi(what[2]);
    }
    else {
      throw std::runtime_error("Didn't catch bad type string: " + definition);
    }
  }

  //-------------------------------
  // Find if Constant or comment

  // Determine next character
  // if '=' -> constant, if '#' -> done, if nothing -> done, otherwise error
  if (regex_search(begin, end, what, boost::regex("\\S")))
  {
    if (what[0] == "=")
    {
      begin = what[0].second;
      // Copy constant
      if (type == "string") {
        value.assign(begin, end);
      }
      else {
        if (regex_search(begin, end, what, boost::regex("\\s*#")))
        {
          value.assign(begin, what[0].first);
        }
        else {
          value.assign(begin, end);
        }
        // TODO: Raise error if string is not numeric
      }

      boost::algorithm::trim(value);
    } else if (what[0] == "#") {
      // Ignore comment
    } else {
      // Error
      throw std::runtime_error("Unexpected character after type and field  ----\n" +
                               definition);
    }
  }
  _type  = ROSType( type );
  _name  = fieldname;
  _value = value;
}

}
