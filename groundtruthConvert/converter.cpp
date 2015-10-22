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


void gtConverter::gtWorker::printTDatFile(std::string file){
	Serialization::Data data;
	{
		std::ifstream is(file.c_str());
		cereal::JSONInputArchive ar(is);

		// load serialized data into member .data
		ar(data);
	}

	GroundTruthEvaluation::ResultsByFrame resultsByFrame;
	for (TrackedObject const& object : data.getTrackedObjects()) {
		for (size_t frameNumber = 0; frameNumber <= object.getLastFrameNumber(); ++frameNumber) {
			std::vector<GroundTruthGridSPtr>& results = resultsByFrame[frameNumber];

			const std::shared_ptr<Grid3D> grid3d = object.maybeGet<Grid3D>(frameNumber);

			if (!grid3d) continue;
			std::cout <<
					grid3d->getCenter().x << "," << grid3d->getCenter().y << ","<< grid3d->getPixelRadius() << ","<<
					grid3d->getZRotation() << ","<<  grid3d->getYRotation() << ","<<
					grid3d->getXRotation()<<std::endl;
		}
	}

}


std::string gtConverter::gtWorker::TDatToCSV(std::string file){
	Serialization::Data data;
	{
		std::ifstream is(file.c_str());
		cereal::JSONInputArchive ar(is);

		// load serialized data into member .data
		ar(data);
	}

	std::stringstream ss;

	GroundTruthEvaluation::ResultsByFrame resultsByFrame;
	for (TrackedObject const& object : data.getTrackedObjects()) {
		for (size_t frameNumber = 0; frameNumber <= object.getLastFrameNumber(); ++frameNumber) {
			std::vector<GroundTruthGridSPtr>& results = resultsByFrame[frameNumber];

			const std::shared_ptr<Grid3D> grid3d = object.maybeGet<Grid3D>(frameNumber);

			if (!grid3d) continue;
			ss <<
					grid3d->getCenter().x << "," << grid3d->getCenter().y << ","<< grid3d->getPixelRadius() << ","<<
					grid3d->getZRotation() << ","<<  grid3d->getYRotation() << ","<<
					grid3d->getXRotation()<<std::endl;
		}
	}
	return ss.str();

}

