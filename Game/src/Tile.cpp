#include "Tile.h"

static inline void check_and_add(Tile *current_tile, Tile *tmp, Node &nod,
								 std::vector<Tile *> &checked_node_tiles,
								 std::vector<Tile *> &tile_nodes_to_check) {
	if(tmp != current_tile && tmp->is_node) {
		nod.AddNeighbor(tmp);
		if(std::find(checked_node_tiles.begin(), checked_node_tiles.end(), tmp)
			== checked_node_tiles.end()) {
			if(std::find(tile_nodes_to_check.begin(), tile_nodes_to_check.end(), tmp)
				== tile_nodes_to_check.end()) {
				tile_nodes_to_check.push_back(tmp);
			}
		}
	}
}

std::vector<Node> create_nodes(std::vector<std::vector<Tile>> &map, Tile *node_tile) {
	std::vector<Tile *> checked_node_tiles;
	std::vector<Tile *> tile_nodes_to_check;
	std::vector<Node> nodes;

	tile_nodes_to_check.push_back(node_tile);
	while(!tile_nodes_to_check.empty()) {
		auto current_tile = tile_nodes_to_check[tile_nodes_to_check.size() - 1];
		tile_nodes_to_check.pop_back();

		Node nod(current_tile);

		Tile *tmp = current_tile;
		while(tmp->up == false) {
			if(tmp->row - 1 > 0) {
				tmp = &map[tmp->row - 1][tmp->column];
			}
			if(tmp->is_node) break;
		}
		check_and_add(current_tile, tmp, nod, checked_node_tiles, tile_nodes_to_check);

		tmp = current_tile;
		while(tmp->down == false) {
			if(tmp->row < map.size() - 1) {
				tmp = &map[tmp->row + 1][tmp->column];
			}
			if(tmp->is_node) break;
		}
		check_and_add(current_tile, tmp, nod, checked_node_tiles, tile_nodes_to_check);

		tmp = current_tile;
		while(tmp->right == false) {
			if(tmp->column < map[0].size() - 1) {
				tmp = &map[tmp->row][tmp->column + 1];
			}
			if(tmp->is_node) break;
		}
		check_and_add(current_tile, tmp, nod, checked_node_tiles, tile_nodes_to_check);

		tmp = current_tile;
		while(tmp->left == false) {
			if(tmp->column > 0) {
				tmp = &map[tmp->row][tmp->column - 1];
			}
			if(tmp->is_node) break;
		}
		check_and_add(current_tile, tmp, nod, checked_node_tiles, tile_nodes_to_check);

		checked_node_tiles.push_back(current_tile);
		nodes.push_back(nod);
	}

	return nodes;
}
