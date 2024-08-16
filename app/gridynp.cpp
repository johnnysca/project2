#include <string>
#include <vector>
#include "gridynp.hpp"
#include <iostream>


unsigned computeMinHealth(std::vector<std::vector<std::string>> &grid, unsigned i, unsigned j, unsigned pFlag, unsigned dFlag, std::vector<std::vector<std::vector<std::vector<unsigned>>>> &memoGrid, unsigned N){
	// use uint flags as bools, 0 is F, 1 is T
	if(i >= N || j >= N) return INT_MAX;
	if(memoGrid[i][j][pFlag][dFlag] != INT_MAX) return memoGrid[i][j][pFlag][dFlag];
	if(i == N-1 && j == N-1){
		int val = stoi(grid[i][j]);
		memoGrid[i][j][pFlag][dFlag] = val > 0 ? 1 : std::abs(val)+1; // 1 hp is sufficient if hp is pos, need at least +1 extra if its negative
		return memoGrid[i][j][pFlag][dFlag];  
	}

	// grid will only contain P. D or digit
	if(grid[i][j] == "P"){
		pFlag = 1;
		int right = computeMinHealth(grid, i, j+1, pFlag, dFlag, memoGrid, N);
		int down = computeMinHealth(grid, i+1, j, pFlag, dFlag, memoGrid, N);
		memoGrid[i][j][pFlag][dFlag] = std::min(right, down);
		return memoGrid[i][j][pFlag][dFlag];
	}
	else if(grid[i][j] == "D"){
		dFlag = 1;
		int right = computeMinHealth(grid, i, j+1, pFlag, dFlag, memoGrid, N);
		int down = computeMinHealth(grid, i+1, j, pFlag, dFlag, memoGrid, N);
		memoGrid[i][j][pFlag][dFlag] = std::min(right, down);
		return memoGrid[i][j][pFlag][dFlag];
	}
	else{
		int val = stoi(grid[i][j]);

		if(pFlag && val < 0){ // previously saw a P so we can exclude this neg val
			pFlag = 0; // cancel out effect for next squares
			val = 0; // cell wont have effect on neighboring cells
		}
		else if(dFlag && val > 0){ // we can double this val
			dFlag = 0; // cancel out effect for next squares
			val *= 2;
		}
		int right = computeMinHealth(grid, i, j+1, pFlag, dFlag, memoGrid, N);
		int down = computeMinHealth(grid, i+1, j, pFlag, dFlag, memoGrid, N);
		int res = std::min(right, down); // take min from either going to right or down

		res -= val; // hp difference to reach current square
		memoGrid[i][j][pFlag][dFlag] = res > 0 ? res : 1; // cant have less than 1 hp at anytime
		return memoGrid[i][j][pFlag][dFlag];
	}
}

unsigned solve(std::vector<std::vector<std::string> > grid) {
	unsigned N = grid.size();
	std::vector<std::vector<std::vector<std::vector<unsigned>>>> memoGrid (N, std::vector<std::vector<std::vector<unsigned>>> (N, std::vector<std::vector<unsigned>> (2, std::vector<unsigned> (2, INT_MAX))));
	unsigned res = computeMinHealth(grid, 0,0,0,0, memoGrid, N);

	return res;
}
/*
	- both flags can be active at the same time 
	- BUT only one can be used at a time: handled by grid input


	{1, -2, P}
	{-3, D, -100}
	{P, 1, 4}
	^ 1, -2, P, -100, 4
	^ 1, -2, D, -100, 4  ====> path from -100 to 4 is a repeated subproblem, but P to -100 could be worse than D to -100. Have to try both 
	^ 1, -2, D, 1, 4....
*/