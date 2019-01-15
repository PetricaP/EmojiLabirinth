#pragma once

#include "Utility.h"
#include <vector>

class Node;

struct Tile {
	int32_t row;
	int32_t column;
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool is_node = false;
	Node *node = nullptr;
	float tileWidth;

	math::vec2f GetPosition() const {
		return {column * tileWidth + tileWidth / 2, -row * tileWidth + tileWidth / 2};
	}
};

class Node {
  private:
	Tile *m_Tile;
	std::vector<Tile *> m_Neighbors;

  public:
	Node(Tile *tile) : m_Tile(tile) {}

	void AddNeighbor(Tile *neighbor) {
		m_Neighbors.push_back(neighbor);
	}

	Tile *GetTile() { return m_Tile; }
	std::vector<Tile *> &GetNeighbors() { return m_Neighbors; }

	friend std::ostream &operator<<(std::ostream &os, const Node &node) {
		os << "{Tile: {" << node.m_Tile->row << " " << node.m_Tile->column << "}, Neighbors: { ";
		for(auto neighbor : node.m_Neighbors) {
			os << "{" << neighbor->row << " " << neighbor->column << "} ";
		}
		os << "}";
		return os;
	}
};

std::vector<Node> create_nodes(std::vector<std::vector<Tile>> &map, Tile *node_tile);

