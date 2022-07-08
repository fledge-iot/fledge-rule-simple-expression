#include <gtest/gtest.h>
#include <plugin_api.h>
#include <config_category.h>
#include <filter_plugin.h>
#include <filter.h>
#include <string.h>
#include <string>
#include <rapidjson/document.h>
#include <reading.h>
#include <reading_set.h>

using namespace std;
using namespace rapidjson;

extern "C" {
	PLUGIN_INFORMATION *plugin_info();
	void plugin_ingest(void *handle,
                   READINGSET *readingSet);
	PLUGIN_HANDLE plugin_init(ConfigCategory& config);
	bool plugin_eval(PLUGIN_HANDLE handle,
                 const string& assetValues);
};

TEST(EXPRESSION, NoTrigger)
{
	PLUGIN_INFORMATION *info = plugin_info();
	ConfigCategory config("expression", info->config);
	config.setItemsValueFromDefault();
	ASSERT_EQ(config.itemExists("expression"), true);
	config.setValue("expression", "a > b");
	ASSERT_EQ(config.itemExists("asset"), true);
	config.setValue("asset", "test");
	void *handle = plugin_init(config);

	string values = "{ \"test\" : { \"a\" : 10, \"b\" : 15 } }";

	bool result = plugin_eval(handle, values);
	ASSERT_EQ(result, false);
}

TEST(EXPRESSION, Trigger)
{
	PLUGIN_INFORMATION *info = plugin_info();
	ConfigCategory config("expression", info->config);
	config.setItemsValueFromDefault();
	ASSERT_EQ(config.itemExists("expression"), true);
	config.setValue("expression", "a > b");
	ASSERT_EQ(config.itemExists("asset"), true);
	config.setValue("asset", "test");
	void *handle = plugin_init(config);

	string values = "{ \"test\" : { \"a\" : 10, \"b\" : 5 } }";

	bool result = plugin_eval(handle, values);
	ASSERT_EQ(result, true);
}

TEST(EXPRESSION, FloatNoTrigger)
{
	PLUGIN_INFORMATION *info = plugin_info();
	ConfigCategory config("expression", info->config);
	config.setItemsValueFromDefault();
	ASSERT_EQ(config.itemExists("expression"), true);
	config.setValue("expression", "a > b");
	ASSERT_EQ(config.itemExists("asset"), true);
	config.setValue("asset", "test");
	void *handle = plugin_init(config);

	string values = "{ \"test\" : { \"a\" : 10.7, \"b\" : 15.2 } }";

	bool result = plugin_eval(handle, values);
	ASSERT_EQ(result, false);
}

TEST(EXPRESSION, FloatTrigger)
{
	PLUGIN_INFORMATION *info = plugin_info();
	ConfigCategory config("expression", info->config);
	config.setItemsValueFromDefault();
	ASSERT_EQ(config.itemExists("expression"), true);
	config.setValue("expression", "a > b");
	ASSERT_EQ(config.itemExists("asset"), true);
	config.setValue("asset", "test");
	void *handle = plugin_init(config);

	string values = "{ \"test\" : { \"a\" : 10.2, \"b\" : 5.1 } }";

	bool result = plugin_eval(handle, values);
	ASSERT_EQ(result, true);
}

TEST(EXPRESSION, DifferentAsset)
{
	PLUGIN_INFORMATION *info = plugin_info();
	ConfigCategory config("expression", info->config);
	config.setItemsValueFromDefault();
	ASSERT_EQ(config.itemExists("expression"), true);
	config.setValue("expression", "a > b");
	ASSERT_EQ(config.itemExists("asset"), true);
	config.setValue("asset", "unknown");
	void *handle = plugin_init(config);

	string values = "{ \"test\" : { \"a\" : 10, \"b\" : 5 } }";

	bool result = plugin_eval(handle, values);
	ASSERT_EQ(result, false);
}
