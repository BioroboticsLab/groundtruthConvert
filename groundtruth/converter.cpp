/*
 * converter.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: hauke
 */

#include "converter.h"

#include <stdlib.h>
#include <stdio.h>

#include <pipeline/Decoder.h>
#include <pipeline/EllipseFitter.h>
#include <pipeline/GridFitter.h>

#include <pipeline/compat.h>
#include <pipeline/Localizer.h>
#include <pipeline/Preprocessor.h>


#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

#include <biotracker/serialization/SerializationData.h>

#include <pipeline/util/GroundTruthEvaluator.h>
#include "Grid3D.h"

#include <sstream>

namespace BC = BioTracker::Core;

std::string fmt(const std::shared_ptr<Grid3D> grid3d, int index) {
    std::stringstream ss;
    ss <<index << ",0,0,"
       << grid3d->getCenter().x << "," << grid3d->getCenter().y  << ","
       << grid3d->getXRotation() << ","<<  grid3d->getYRotation() << ","
       << grid3d->getZRotation() <<","<< grid3d->getPixelRadius()<< ",0,0,";


    const std::array<boost::logic::tribool, 12ul> arr = grid3d->getIdArray();
    for (int i=11; i>=0; i--) {
        ss << arr[i];
    }

    return ss.str();
}

void gtConverter::gtWorker::printTDatFile(std::string file) {
    BC::Serialization::Data data;
    {
        std::ifstream is(file.c_str());
        cereal::JSONInputArchive ar(is);

        // load serialized data into member .data
        ar(data);
    }

    int idx = 0;
    GroundTruthEvaluation::ResultsByFrame resultsByFrame;
    for (BC::TrackedObject const &object : data.getTrackedObjects()) {
        for (size_t frameNumber = 0; frameNumber <= object.getLastFrameNumber(); ++frameNumber) {
            const std::shared_ptr<Grid3D> grid3d = object.maybeGet<Grid3D>(frameNumber);

            if (!grid3d) {
                continue;
            }
            std::string ss = fmt(grid3d,idx++);
            std::cout << ss<<std::endl;

        }
    }

}


std::string gtConverter::gtWorker::TDatToCSV(std::string file, bool appendGridpoints) {
    BC::Serialization::Data data;
    {
        std::ifstream is(file.c_str());
        cereal::JSONInputArchive ar(is);

        // load serialized data into member .data
        ar(data);
    }

    std::stringstream ss;

    int idx = 0;
    GroundTruthEvaluation::ResultsByFrame resultsByFrame;
    for (BC::TrackedObject const &object : data.getTrackedObjects()) {
        for (size_t frameNumber = 0; frameNumber <= object.getLastFrameNumber(); ++frameNumber) {
            const std::shared_ptr<Grid3D> grid3d = object.maybeGet<Grid3D>(frameNumber);

            if (!grid3d) {
                continue;
            }
            std::string r = fmt(grid3d,idx++);
            ss << r;
            if (appendGridpoints) {

                std::vector<cv::Point> ptso = grid3d->getOuterRingPoints();
                std::vector<cv::Point> ptsm = grid3d->getMiddleRingPoints();
                std::vector<cv::Point> ptsi = grid3d->getInnerRingPoints();
                ss<<",O";
                for (unsigned int i=0; i<ptso.size(); i++) {
                    ss << "," << ptso[i].x << ","<<ptso[i].y;
                }
                ss<<",M";
                for (unsigned int i=0; i<ptsm.size(); i++) {
                    ss << "," << ptsm[i].x << ","<<ptsm[i].y;
                }
                ss<<",I";
                for (unsigned int i=0; i<ptsi.size(); i++) {
                    ss << "," << ptsi[i].x << ","<<ptsi[i].y;
                }
            }
            ss << std::endl;
        }
    }
    return ss.str();
}



