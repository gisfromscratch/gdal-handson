#include "pch.h"
#include "SpatialMatcher.h"
#include "GdalManagedDataset.h"


SpatialMatch::SpatialMatch(GIntBig left, GIntBig right) : m_left(left), m_right(right)
{
}


SpatialMatch::~SpatialMatch()
{
}


SpatialMatcher::SpatialMatcher(const std::unique_ptr<GdalManagedDataset> &left, const std::unique_ptr<GdalManagedDataset> &right)
	: m_left(left), m_right(right)
{
}


SpatialMatcher::~SpatialMatcher()
{
}


void SpatialMatcher::find_best_matches(const std::string &left_layer_name, const std::string &right_layer_name, std::vector<SpatialMatch> &matches)
{
	auto left_layer = m_left->get_layer(left_layer_name);
	if (nullptr == left_layer)
	{
		return;
	}

	auto right_layer = m_right->get_layer(right_layer_name);
	if (nullptr == right_layer)
	{
		return;
	}

	for (auto&& left_feature : left_layer)
	{
		auto left_geometry = left_feature->GetGeometryRef();
		if (nullptr != left_geometry && !left_geometry->IsEmpty())
		{
			right_layer->SetSpatialFilter(left_geometry);
			GIntBig best_match = -1;
			OGRGeometry *best_intersection = nullptr;
			for (auto&& right_feature : right_layer)
			{
				auto right_geometry = right_feature->GetGeometryRef();
				auto intersection = right_geometry->Intersection(left_geometry);
				if (-1 == best_match)
				{
					best_match = right_feature->GetFID();
					best_intersection = intersection;
				}
				else if (!intersection->IsEmpty())
				{
					std::cout << intersection->getGeometryType() << std::endl;
					switch (intersection->getGeometryType())
					{
					case wkbLineString:
						break;
					}
				}
			}
		}
	}
}
