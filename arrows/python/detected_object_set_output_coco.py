# ckwg +29
# Copyright 2020 by Kitware, Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#  * Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
#  * Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
#  * Neither name of Kitware, Inc. nor the names of any contributors may be used
#    to endorse or promote products derived from this software without specific
#    prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import datetime
import json

from vital.algo import DetectedObjectSetOutput

class DetectedObjectSetOutputCoco(DetectedObjectSetOutput):
    """COCO-formatted output for DetectedObjectSets

    See DetectedObjectSetOutput for method information.

    """
    def __init__(self):
        DetectedObjectSetOutput.__init__(self)
        # List of dicts corresponding to elements of the output
        # "annotations" attribute, minus the "id" attribute
        self.detections = []
        # List of image paths
        self.images = []
        # Dict mapping category names to numerical IDs
        self.categories = {}
        # The first ID to be assigned to a category (and then counting
        # up from there)
        self.category_start_id = 1
        # The current file object or None
        self.file = None

    def get_configuration(self):
        cfg = super(DetectedObjectSetOutput, self).get_configuration()
        cfg.set_value("category_start_id", str(self.category_start_id))
        return cfg

    def set_configuration(self, cfg_in):
        cfg = self.get_configuration()
        cfg.merge_config(cfg_in)
        self.category_start_id = int(cfg.get_value("category_start_id"))

    def check_configuration(self, cfg):
        return True

    def open(self, file_name):
        self.file = open(file_name, 'w')

    def close(self):
        if self.file:
            self.file.close()

    def write_set(self, detected_object_set, file_name):
        for det in detected_object_set:
            bbox = det.bounding_box()
            d = dict(
                image_id=len(self.images),
                bbox=[
                    bbox.min_x(),
                    bbox.min_y(),
                    bbox.width(),
                    bbox.height(),
                ],
                score=det.confidence(),
            )
            if det.type() is not None:
                d['category_id'] = self.categories.setdefault(
                    det.type().get_most_likely_class(),
                    len(self.categories) + self.category_start_id,
                )
            self.detections.append(d)
        self.images.append(file_name)

    def complete(self):
        now = datetime.datetime.now(datetime.timezone.utc).astimezone()
        json.dump(dict(
            info=dict(
                year=now.year,
                description="Created by DetectedObjectSetOutputCoco",
                date_created=now.replace(microsecond=0).isoformat(' '),
            ),
            annotations=[dict(d, id=i)
                         for i, d in enumerate(self.detections)],
            categories=[dict(id=i, name=c) for c, i in self.categories.items()],
            images=[dict(id=i, file_name=im)
                    for i, im in enumerate(self.images)],
        ), self.file)
        self.file.flush()

def __vital_algorithm_register__():
    from vital.algo import algorithm_factory
    implementation_name = 'coco'
    if algorithm_factory.has_algorithm_impl_name(
            DetectedObjectSetOutputCoco.static_type_name(),
            implementation_name,
    ):
        return
    algorithm_factory.add_algorithm(
        implementation_name,
        "Write detections out in COCO-style JSON",
        DetectedObjectSetOutputCoco,
    )
    algorithm_factory.mark_algorithm_as_loaded(implementation_name)
