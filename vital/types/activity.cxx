/*ckwg +30
 * Copyright 2020 by Kitware, Inc.
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
 *  * Neither name of Kitware, Inc. nor the names of any contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */

#include <vital/types/activity.h>

#include <utility>

namespace kwiver {
namespace vital {

  activity::activity()
   :m_id{-1},
    m_label{UNDEFINED_ACTIVITY},
    m_type{nullptr},
    m_confidence{-1.0},
    m_participants{nullptr},
    m_start_frame{kwiver::vital::timestamp(-1, -1)},
    m_end_frame{kwiver::vital::timestamp(-1, -1)}
  {}

  activity::activity( activity_id_t id,
                      activity_label_t label,
                      double confidence,
                      activity_type_sptr classifications,
                      kwiver::vital::timestamp start,
                      kwiver::vital::timestamp end,
                      kwiver::vital::object_track_set_sptr participants )
   :m_id{id},
    m_label{label},
    m_type{classifications},
    m_confidence{confidence},
    m_participants{participants},
    m_start_frame{start},
    m_end_frame{end}
  {}

  activity_id_t
  activity::id() const
  {
    return m_id;
  }

  void activity::set_id( activity_id_t const id )
  {
    m_id = id;
  }

  activity_label_t activity::label() const
  {
    return m_label;
  }

  void activity::set_label( activity_label_t const label )
  {
    m_label = label;
  }

  activity_type_sptr activity::type() const
  {
    return m_type;
  }

  void activity::set_type( activity_type_sptr c )
  {
    m_type = c;
  }

  double activity::confidence() const
  {
    return m_confidence;
  }

  void activity::set_confidence( double confidence )
  {
    m_confidence = confidence;
  }

  kwiver::vital::timestamp activity::start() const
  {
    return m_start_frame;
  }

  void activity::set_start( kwiver::vital::timestamp start_frame )
  {
    m_start_frame = start_frame;
  }

  kwiver::vital::timestamp activity::end() const
  {
    return m_end_frame;
  }

  void activity::set_end( kwiver::vital::timestamp end_frame )
  {
    m_end_frame = end_frame;
  }

  std::pair<kwiver::vital::timestamp, kwiver::vital::timestamp> activity::duration() const
  {
    return std::make_pair(m_start_frame, m_end_frame);
  }

  kwiver::vital::object_track_set_sptr activity::participants() const
  {
    return m_participants;
  }

  void activity::set_participants( kwiver::vital::object_track_set_sptr participants )
  {
    m_participants = participants;
  }

} }
