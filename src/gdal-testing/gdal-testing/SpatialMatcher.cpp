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

	auto left_spatialRef = left_layer->GetSpatialRef();
	auto right_spatialRef = right_layer->GetSpatialRef();

	OGRSpatialReference webMercator;
	webMercator.SetWellKnownGeogCS("EPSG:3857");
	for (auto&& left_feature : left_layer)
	{
		auto left_id = left_feature->GetFID();
		auto left_geometry = left_feature->GetGeometryRef();
		if (nullptr != left_geometry && !left_geometry->IsEmpty())
		{
			left_geometry->assignSpatialReference(left_spatialRef);
			left_geometry->transformTo(&webMercator);

			const double intersection_tolerance = 1.5;
			auto buffered_left_geometry = left_geometry->Buffer(intersection_tolerance);
			right_layer->SetSpatialFilter(buffered_left_geometry);
			GIntBig best_match = -1;
			double best_match_length = -1;
			OGRGeometry *best_intersection = nullptr;
			for (auto&& right_feature : right_layer)
			{
				auto right_geometry = right_feature->GetGeometryRef();
				right_geometry->transformTo(&webMercator);

				auto intersection = right_geometry->Intersection(buffered_left_geometry);
				if (-1 == best_match)
				{
					best_match = right_feature->GetFID();
					best_intersection = intersection;
				}
				else if (!intersection->IsEmpty())
				{
					switch (intersection->getGeometryType())
					{
					case wkbLineString:
						OGRLineString *line = dynamic_cast<OGRLineString*>(intersection);
						if (nullptr != line)
						{
							double line_length = line->get_Length();
							if (best_match_length < line_length)
							{
								best_match = right_feature->GetFID();
								best_intersection = intersection;
								best_match_length = line_length;
							}
						}
						break;
					}
				}
			}

			if (-1 != best_match)
			{
				matches.push_back(SpatialMatch(left_id, best_match));
			}
		}
	}
}
