/**
 * @file
 *
 * @brief Implementation of get/set and error plugins
 *
 * @copyright BSD License (see LICENSE.md or https://www.libelektra.org)
 *
 */


#ifndef TOOLS_PLUGINS_HPP
#define TOOLS_PLUGINS_HPP

#include <plugin.hpp>
#include <toolexcept.hpp>

#include <map>
#include <string>
#include <vector>

#include <kdb.hpp>

namespace kdb
{

namespace tools
{


struct Place
{
	int current;
	int max;

	Place () : current (-1), max (0)
	{
	}

	Place (int current_, int max_) : current (current_), max (max_)
	{
	}
};

/**
 * @brief A collection of plugins (either get, set or error)
 */
class Plugins
{
protected:
	std::vector<Plugin *> plugins;

	std::vector<std::string> needed;
	std::vector<std::string> recommended;
	std::vector<std::string> alreadyProvided;
	std::vector<std::string> alreadyConflict;

	int nrStoragePlugins;
	int nrResolverPlugins;

	int revPostGet;

	std::map<std::string, Place> placementInfo;

public:
	Plugins ();

	/** Add needed, provided and recommend information */
	void addInfo (Plugin & plugin);
	void addPlugin (Plugin & plugin, std::string const & which);

	/** Validate needed, and provided information.
	 * (Recommended ignored, @see getRecommendedMissing(),
	 * @see getNeededMissing() */
	bool validateProvided () const;
	std::vector<std::string> getNeededMissing () const;
	std::vector<std::string> getRecommendedMissing () const;

	bool checkPlacement (Plugin & plugin, std::string const & which);
	void checkStorage (Plugin & plugin);
	void checkResolver (Plugin & plugin);
	void checkOrdering (Plugin & plugin);
	void checkConflicts (Plugin & plugin);
};

/**
 * @brief Plugins to get configuration
 */
class GetPlugins : private Plugins
{
public:
	/**
	 * Returns true if GetPlugins are valid afterwards.
	 *
	 * Will throw an exception if plugin could not
	 * be added.
	 */
	void tryPlugin (Plugin & plugin);
	void addPlugin (Plugin & plugin);
	bool validated () const;

	void serialise (kdb::Key & baseKey, kdb::KeySet & ret);
};


/**
 * @brief Plugins to set configuration
 */
class SetPlugins : private Plugins
{
public:
	void tryPlugin (Plugin & plugin);
	void addPlugin (Plugin & plugin);
	bool validated () const;

	void serialise (kdb::Key & baseKey, kdb::KeySet & ret);
};

/**
 * @brief Plugins to handle errors during configuration access
 */
class ErrorPlugins : private Plugins
{
public:
	void status (std::ostream & os) const;

	void tryPlugin (Plugin & plugin);
	void addPlugin (Plugin & plugin);
	bool validated () const;

	void serialise (kdb::Key & baseKey, kdb::KeySet & ret);
};
}
}

#endif
