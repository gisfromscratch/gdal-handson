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

#include "pch.h"

class GdalManagedDataset
{
public:
	~GdalManagedDataset();

	/**
		Validates if this dataset is accessible and can be queried.
		@return true if this dataset is valid.
	*/
	bool is_valid();

	/**
		Gets the names of all layers.
		@return names of all layers managed by this dataset.
	*/
	std::list<std::string> get_layer_names();

	/**
		Gets a layer from this dataset using its name.
		@param layer_name the name of the layer.
		@return layer or null if the layer is not managed by this dataset.
	*/
	OGRLayer* get_layer(const std::string &layer_name);

	/**
		Opens the specified dataset using the file path.
		@param filepath the path to the dataset.
		@return dataset or null if the dataset cannot be opened.
	*/
	static std::unique_ptr<GdalManagedDataset> open_read(const std::string &filepath);

protected:
	GdalManagedDataset(GDALDataset *dataset);

private:
	GdalManagedDataset(const GdalManagedDataset&) = delete;
	GdalManagedDataset& operator=(const GdalManagedDataset&) = delete;

	GDALDataset *m_dataset;
};

