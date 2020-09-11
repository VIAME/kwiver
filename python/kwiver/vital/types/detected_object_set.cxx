/*ckwg +29
 * Copyright 2017-2020 by Kitware, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  * Neither name of Kitware, Inc. nor the names of any contributors may be used
 *    to endorse or promote products derived from this software without specific
 *    prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <vital/types/detected_object_set.h>
#include <python/kwiver/vital/util/pybind11.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/embed.h>

namespace py = pybind11;
namespace kv = kwiver::vital;

typedef kv::detected_object_set det_obj_set;
using detected_object_sptr = std::shared_ptr< kv::detected_object >;
using detected_object_scptr = std::shared_ptr< kv::detected_object const >;

class det_obj_set_trampoline : public det_obj_set {
  
  using det_obj_set::det_obj_set;
  size_t size() const override;
  bool empty() const override;
  detected_object_sptr at(size_t pos) override;
  const detected_object_sptr at(size_t pos) const override;
};

size_t det_obj_set_trampoline::size() const
{
  VITAL_PYBIND11_OVERLOAD(
    size_t,
    det_obj_set,
    size,
  );  
}

bool det_obj_set_trampoline::empty() const
{
  VITAL_PYBIND11_OVERLOAD(
    bool,
    det_obj_set,
    empty,
  );
}

const detected_object_sptr det_obj_set_trampoline::at(size_t pos) const
{
  VITAL_PYBIND11_OVERLOAD(
    detected_object_sptr,
    det_obj_set,
    at,
    pos
  );
}

detected_object_sptr det_obj_set_trampoline::at(size_t pos)
{
  VITAL_PYBIND11_OVERLOAD(
    detected_object_sptr,
    det_obj_set,
    at,
    pos
  );
}


PYBIND11_MODULE(detected_object_set, m)
{
  /*
   *

    Developer:
        python -c "import kwiver.vital.types; help(kwiver.vital.types.DetectedObjectSet)"
        python -m xdoctest kwiver.vital.types DetectedObjectSet --xdoc-dynamic

   *
   */
  py::class_<det_obj_set, std::shared_ptr<det_obj_set>, det_obj_set_trampoline>(m, "DetectedObjectSet", R"(
      Collection holding a multiple detected objects

      Example:
          >>> from kwiver.vital.types import *
          >>> bbox = BoundingBox(0, 10, 100, 50)
          >>> dobj1 = DetectedObject(bbox, 0.2)
          >>> dobj2 = DetectedObject(bbox, 0.5)
          >>> dobj3 = DetectedObject(bbox, 0.4)
          >>> self = DetectedObjectSet()
          >>> self.add(dobj1)
          >>> self.add(dobj2)
          >>> self.add(dobj3)
          >>> self.add(dobj3)
          >>> assert len(self) == 4
          >>> assert list(self) == [dobj1, dobj2, dobj3, dobj3]
          >>> assert list(self) != [dobj1, dobj1, dobj1, dobj1]
          >>> print(self)
          <DetectedObjectSet(size=4)>
    )")
  .def(py::init<>())
  .def(py::init<std::vector<std::shared_ptr<kv::detected_object>>>())
  .def("add", [](det_obj_set &self, py::object object)
    {
      try
      {
        auto det_obj = object.cast<std::shared_ptr<kv::detected_object>>();
        return self.add(det_obj);
      }
      catch(...){};
      auto det_obj = object.cast<std::shared_ptr<det_obj_set>>();
      return self.add(det_obj);
    })
  .def("clone", &det_obj_set::clone)
  .def("at", (const detected_object_sptr (det_obj_set::*)(size_t) const) &det_obj_set::at)
  .def("at", (detected_object_sptr (det_obj_set::*)(size_t)) &det_obj_set::at)
  .def("empty", &det_obj_set::empty)
  .def("size", &det_obj_set::size)
  .def("__len__", &det_obj_set::size)
  .def("select", [](det_obj_set &self, double threshold, py::object class_name)
    {
      if(class_name.is(py::none()))
      {
        return self.select(threshold);
      }
      return self.select(class_name.cast<std::string>(), threshold);
    },
    py::arg("threshold")=kv::class_map::INVALID_SCORE,
    py::arg("class_name")=py::none())
  .def("__getitem__", [](det_obj_set &self, size_t idx)
    {
      return self.at(idx);
    })

  .def("__iter__", [](det_obj_set &self)
    {
      return py::make_iterator(self.begin(), self.end());
    },
    py::keep_alive<0, 1>())
  .def("__nice__", [](det_obj_set& self) -> std::string {
    auto locals = py::dict(py::arg("self")=self);
    py::exec(R"(
        retval = 'size={}'.format(len(self))
    )", py::globals(), locals);
    return locals["retval"].cast<std::string>();
    })
  .def("__repr__", [](py::object& self) -> std::string {
    auto locals = py::dict(py::arg("self")=self);
    py::exec(R"(
        classname = self.__class__.__name__
        devnice = self.__nice__()
        retval = '<%s(%s) at %s>' % (classname, devnice, hex(id(self)))
    )", py::globals(), locals);
    return locals["retval"].cast<std::string>();
    })
  .def("__str__", [](py::object& self) -> std::string {
    auto locals = py::dict(py::arg("self")=self);
    py::exec(R"(
        classname = self.__class__.__name__
        devnice = self.__nice__()
        retval = '<%s(%s)>' % (classname, devnice)
    )", py::globals(), locals);
    return locals["retval"].cast<std::string>();
    })
  ;
}
