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

int main(int argc, wchar_t* argv[])
{
	using namespace testing;
	InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}