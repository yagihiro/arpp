#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <arpp/arpp.h>

TEST_CASE("build is ok", "build") {
	std::map<std::string, std::string> options = {
	  {arpp::Connection::kOptionAdapter, "sqlite3"},
	  {arpp::Connection::kOptionDatabase, "db.sqlite3"},
	};

	arpp::Status status;
	auto c = arpp::Connection::connect(options, &status);
	REQUIRE(status.is_ok());
}
