#include <vector>
#include <tuple>
#include <iostream>
#include <map>
#define NORTH 1
#define EAST  2
#define SOUTH 4
#define WEST  8


typedef std::vector<std::vector<int>> ocean;
typedef std::vector<std::vector<int>> ocean_cache;
struct boundary {
	int i_min;
	int j_min;
	int i_max;
	int j_max;
};

struct index {
	int i;
	int j;
	bool inside(boundary& b) {
		return (i >= b.i_min && i <= b.i_max && j >= b.j_min && j <= b.j_max);
	}
	std::vector<index> neighbors(boundary& z) {
		std::vector<index> neigh;
		index 
		 a = index{i  , j-1}
		,b = index{i  , j+1}
		,c = index{i-1, j  }
		,d = index{i+1, j  };
		if (a.inside(z)) neigh.push_back(a);
		if (b.inside(z)) neigh.push_back(b);
		if (c.inside(z)) neigh.push_back(c);
		if (d.inside(z)) neigh.push_back(d);
		return neigh;
	}
	int height(ocean& o) {
		return o[this->i][this->j];
	}
};

bool operator==(index& a, const index& b) {
	return a.i ==b.i && a.j == b.j;
}

int traverse(index cur, ocean& grid, ocean_cache& c, boundary bounds, index par);

int main(int argc, char const *argv[])
{
	std::vector<std::vector<int>> grid{
		{1, 2, 1, 4, 5, 6},
		{2, 4, 1, 4, 2, 5},
		{3, 2, 1, 3, 3, 4},
		{4, 2, 1, 3, 2, 1},
		{1, 1, 1, 2, 2, 2},
		{4, 3, 1, 1, 1, 1}
	};

	std::vector<std::vector<int>> grid_values{
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0}
	};

	std::map<int, std::vector<index>> buckets;

	boundary grid_bounds{ 0, 0, (int)grid.size()-1, (int)grid[0].size()-1};

	for (int i = grid_bounds.i_min; i <= grid_bounds.i_max; ++i ) {
		for (int j = grid_bounds.j_min; j <= grid_bounds.j_max; ++j) {
			buckets[grid[i][j]].push_back(index{i,j});
		}
	}
	for (int i = 0; i < 10; ++i) {
		for (index ind : buckets[i])
			traverse(ind, grid, grid_values, grid_bounds, index{-1,-1});
	}

	return 0;
}

int innate_value(const index& cur, const boundary& bounds) {
	int value = 0;
	if (cur.i == bounds.i_min) value |= NORTH;
	if (cur.i == bounds.i_max) value |= SOUTH;
	if (cur.j == bounds.j_min) value |= WEST;
	if (cur.j == bounds.j_max) value |= EAST;
	return value;
}

int traverse(index cur, ocean& grid, ocean_cache& c, boundary bounds, index par) {
	if (!cur.inside(bounds)) return 0;
	if (c[cur.i][cur.j] != 0) return c[cur.i][cur.j];

	int value = innate_value(cur, bounds);

	for (index neighbor_index : cur.neighbors(bounds)) {
		if (!(neighbor_index == par) && cur.height(grid) >= neighbor_index.height(grid)) 
			value |= traverse(neighbor_index, grid, c, bounds, cur);
	}

	if (value == (NORTH | SOUTH | EAST | WEST) || par == index{-1,-1}) {
		c[cur.i][cur.j] = value;
	}

	if (value == (NORTH | SOUTH | EAST | WEST)) 
		std::cout << cur.i << " " << cur.j << " " << value << std::endl;
	
	return value;
}