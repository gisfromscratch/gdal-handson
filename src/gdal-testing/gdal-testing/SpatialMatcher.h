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

#pragma once

class GdalManagedDataset;


/**
	Represents a match between two features using a spatial constraint.
*/
class SpatialMatch
{
public:
	SpatialMatch(GIntBig left, GIntBig right);
	~SpatialMatch();

private:
	GIntBig m_left;
	GIntBig m_right;
};


/**
	Represents a matcher finding the best matches between two datasets using a spatial constraint.
*/
class SpatialMatcher
{
public:
	SpatialMatcher(const std::unique_ptr<GdalManagedDataset> &left, const std::unique_ptr<GdalManagedDataset> &right);
	~SpatialMatcher();

	/**
		Finds the best matches between two datasets.
		@param left_layer_name the name of the layer from the left dataset.
		@param right_layer_name the name of the layer from the right dataset.
		@param matches output parameter using a container for the best matches.
	*/
	void find_best_matches(const std::string &left_layer_name, const std::string &right_layer_name, std::vector<SpatialMatch> &matches);

private:
	const std::unique_ptr<GdalManagedDataset> &m_left;
	const std::unique_ptr<GdalManagedDataset> &m_right;
};

