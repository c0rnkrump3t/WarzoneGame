#ifndef COMP345_CONFIG_H
#define COMP345_CONFIG_H

#include <map>

using namespace std;

struct Config {
	// name of commands as enum instead of string
	// esaier to internally handle compared to a string
	enum CommandName {
		NONE,
		loadMap,
		validateMap,
		addPlayer,
		gameStart,
		issueOrder,
		endIssueOrders,
		execOrder,
		endExecOrders,
		win,
		replay,
		quit,
	};

	// Shared names of states
	enum State {
		start,
		mapLoaded,
		mapValidated,
		playersAdded,
		assignReinforcment,
		issueOrders,
		executeOrders,
		winState,
		END
	};
};

template <class T>
T getTransitions() {
	map<pair <Config::State, Config::CommandName>, Config::State> const Transitions{
		// GameProcessor handled commands
		{ {Config::State::start, Config::CommandName::loadMap},  Config::State::mapLoaded },
		{ {Config::State::mapLoaded, Config::CommandName::loadMap}, Config::State::mapLoaded },
		{ {Config::State::mapLoaded, Config::CommandName::validateMap}, Config::State::mapValidated },
		{ {Config::State::mapValidated, Config::CommandName::addPlayer }, Config::State::playersAdded },
		{ {Config::State::playersAdded, Config::CommandName::gameStart }, Config::State::assignReinforcment },
		{ {Config::State::playersAdded, Config::CommandName::addPlayer}, Config::State::playersAdded },
		{ {Config::State::winState, Config::CommandName::replay }, Config::State::start },
		{ {Config::State::winState, Config::CommandName::quit }, Config::State::END },

		// Gameplay commands ! Not input commands
		{ {Config::State::assignReinforcment, Config::CommandName::issueOrder}, Config::State::issueOrders },
		{ {Config::State::issueOrders, Config::CommandName::issueOrder}, Config::State::issueOrders },
		{ {Config::State::issueOrders, Config::CommandName::endIssueOrders },Config::State::executeOrders },
		{ {Config::State::executeOrders, Config::CommandName::execOrder}, Config::State::executeOrders },
		{ {Config::State::executeOrders,Config::CommandName::endExecOrders}, Config::State::assignReinforcment },
		{ {Config::State::executeOrders, Config::CommandName::win}, Config::State::winState },
	};
	return Transitions;
}

#endif // COMP345_CONFIG_H