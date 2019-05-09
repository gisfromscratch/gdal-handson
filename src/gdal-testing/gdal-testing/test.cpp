/*
 * Copyright 2019 Jan Tschada
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "pch.h"

#include <iostream>
#include "GdalTestSuite.h"
#include "GdalManagedDataset.h"

/**
	Reads a GeoJSON file from the local hard drive.
*/
TEST_F(GdalTestSuite, ReadGeoJson) {
	auto filepath = GdalTestSuite::to_path("geojson.json");
	auto dataset = GdalManagedDataset::open_read(filepath);
	EXPECT_TRUE(dataset->is_valid()) << "Dataset is valid.";

	auto layer_names = dataset->get_layer_names();
	EXPECT_FALSE(layer_names.empty()) << "Layer names must not be empty.";
	for (auto &&layer_name : layer_names)
	{
		auto layer = dataset->get_layer(layer_name);
		EXPECT_NE(nullptr, layer) << "Layer must not be nullptr.";

		for (auto &&feature : layer)
		{
			auto geometry = feature->GetGeometryRef();
			EXPECT_FALSE(geometry->IsEmpty()) << "Geometry must not be empty.";
		}
	}
}


TEST_F(GdalTestSuite, FilterGeoJson) {
	OGRLinearRing ring;
	ring.addPoint(7.128, 50.721);
	ring.addPoint(7.128, 50.710);
	ring.addPoint(7.147, 50.710);
	ring.addPoint(7.147, 50.721);
	ring.addPoint(7.128, 50.721);
	OGRSpatialReference wgs84;
	wgs84.SetWellKnownGeogCS("WGS84");
	ring.assignSpatialReference(&wgs84);
	OGRPolygon area;
	area.assignSpatialReference(&wgs84);
	area.addRing(&ring);

	auto filepath = GdalTestSuite::to_path("geojson.json");
	auto dataset = GdalManagedDataset::open_read(filepath);
	EXPECT_TRUE(dataset->is_valid()) << "Dataset is valid.";

	auto layer_names = dataset->get_layer_names();
	EXPECT_FALSE(layer_names.empty()) << "Layer names must not be empty.";
	for (auto &&layer_name : layer_names)
	{
		auto layer = dataset->get_layer(layer_name);
		EXPECT_NE(nullptr, layer) << "Layer must not be nullptr.";

		layer->SetSpatialFilter(&area);
		std::list<GIntBig> ids;
		std::list<int> strecken_ids;
		for (auto &&feature : layer)
		{
			auto geometry = feature->GetGeometryRef();
			EXPECT_FALSE(geometry->IsEmpty()) << "Geometry must not be empty.";
			auto fid = feature->GetFID();
			ids.push_back(fid);
			auto strecke_id = feature->GetFieldAsInteger("strecke_id");
			strecken_ids.push_back(strecke_id);
		}
		EXPECT_FALSE(ids.empty()) << "At least one feature must be found.";
		EXPECT_FALSE(strecken_ids.end() == std::find(strecken_ids.begin(), strecken_ids.end(), 47)) << "Strecke 47 must be found.";
		EXPECT_FALSE(strecken_ids.end() == std::find(strecken_ids.begin(), strecken_ids.end(), 48)) << "Strecke 48 must be found.";
	}
}


int main(int argc, wchar_t* argv[])
{
	using namespace testing;
	InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}