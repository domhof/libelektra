#ifndef MERGE_HPP
#define MERGE_HPP

#include <command.hpp>
#include <kdb.hpp>
#include <merging/threewaymerge.hpp>
#include <merging/automergestrategy.hpp>

class MergeCommand : public Command
{
	kdb::KDB kdb;

public:
	MergeCommand();
	~MergeCommand();

	virtual int execute (Cmdline const& cmdline);

	virtual std::string getShortOptions()
	{
		return "Htsvb";
	}

	virtual std::string getSynopsis()
	{
		return "[options] ourpath theirpath basepath [mergepath]";
	}

	virtual std::string getShortHelpText()
	{
		return "Three-way merge of KeySets.";
	}

	virtual std::string getLongHelpText()
	{
		return
			"Completes a three-way merge between keysets and saves the resulting keyset to mergepath\n\nmergepath .. path where the merged keyset should be saved\nourpath .. path to the keyset to serve as ours\ntheirpath .. path to the keyset to serve as theirs\nbasepath .. path to the base keyset\n\n-H --help                print help text\n-I --interactive         interactive mode to manually merge\n-V --version             print version info\n";
	}
};

#endif
