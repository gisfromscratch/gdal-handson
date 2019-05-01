#include "pch.h"
#include "GdalTestSuite.h"


void GdalTestSuite::SetUp()
{
	// Register all available GDAL drivers
	GDALAllRegister();
}


std::string GdalTestSuite::data_dir()
{
	auto full_path = boost::filesystem::system_complete("../../../data");
	return full_path.string();
}


std::string GdalTestSuite::to_path(const std::string &filename)
{
	boost::filesystem::path directory(data_dir());
	boost::filesystem::path full_path = directory / boost::filesystem::path(filename);
	return full_path.string();
}


void GdalTestSuite::TearDown()
{

}
