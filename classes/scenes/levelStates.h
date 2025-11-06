#pragma once

// BlackSmith Talked to in forest
// BlackSmith Talked to in town

struct LevelStates{
	bool SpokeToBSInForest = false;
	bool SpokeToBSInTown = false;
};

extern LevelStates levelStates;
