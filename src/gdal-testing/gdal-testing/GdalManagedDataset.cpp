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
#include "GdalManagedDataset.h"


GdalManagedDataset::GdalManagedDataset(GDALDataset *dataset) : m_dataset(dataset)
{

}


bool GdalManagedDataset::is_valid()
{
	return nullptr != m_dataset;
}


OGRLayer* GdalManagedDataset::get_layer(const std::string &layer_name)
{
	if (nullptr == m_dataset)
	{
		return nullptr;
	}

	return m_dataset->GetLayerByName(layer_name.c_str());
}


std::shared_ptr<GdalManagedDataset> GdalManagedDataset::open_read(const std::string &filepath)
{
	auto raw_dataset = reinterpret_cast<GDALDataset*>(GDALOpenEx(filepath.c_str(), GDAL_OF_VECTOR, nullptr, nullptr, nullptr));
	return std::shared_ptr<GdalManagedDataset>(new GdalManagedDataset(raw_dataset));
}


GdalManagedDataset::~GdalManagedDataset()
{
	if (nullptr != m_dataset)
	{
		GDALClose(m_dataset);
		m_dataset = nullptr;
	}
}
