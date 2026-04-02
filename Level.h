#ifndef LEVEL_H_
#define LEVEL_H_

#include "GameConstants.h"
#include "Coord.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>

class Level {
public:
    enum MazeEntry {
        empty, floor, lemming_factory,
        ice_monster, bonfire,
        trampoline, net, left_one_way_door, right_one_way_door, lemming_exit, pheromone, spring
    };

    enum LoadResult {
        load_success, load_fail_file_not_found, load_fail_bad_format
    };

    Level(std::string assetDir)
        : m_pathPrefix(assetDir) {
        m_maze.resize(LEVEL_HEIGHT, std::vector<MazeEntry>(LEVEL_WIDTH, empty));
        if (!m_pathPrefix.empty())
            m_pathPrefix += '/';
    }

    LoadResult loadLevel(std::string filename) {
        std::ifstream levelFile(m_pathPrefix + filename);
        if (!levelFile) {
            m_errorMsg = "File not found: " + m_pathPrefix + filename;
            return load_fail_file_not_found;
        }

        std::string line;
        int y = LEVEL_HEIGHT - 1;
        int lineNum = 0;
        bool has_exit = false;
		int num_factories = 0;

        while (lineNum < LEVEL_HEIGHT && std::getline(levelFile, line)) {
            lineNum++;

            if (line.size() != LEVEL_WIDTH) {
                m_errorMsg = "Line " + std::to_string(lineNum) + " has wrong length: " + 
                           std::to_string(line.size()) + " (expected " + std::to_string(LEVEL_WIDTH) + ")";
                return load_fail_bad_format;
            }

            for (int x = 0; x < LEVEL_WIDTH; x++) {
                MazeEntry me;
                switch (toupper(line[x])) {
                    case ' ': me = empty; break;
                    case '@': me = floor; break;
                    case 'L': me = lemming_factory; num_factories++; break;
                    case 'I': me = ice_monster; break;
                    case 'T': me = trampoline; break;
                    case 'B': me = bonfire; break;    
                    case 'X': me = lemming_exit; has_exit = true; break;
                    case 'N': me = net; break;
                    case '<': me = left_one_way_door; break;
                    case '>': me = right_one_way_door; break;
                    case 'P': me = pheromone; break;
                    case 'S': me = spring; break;
                    default: 
                        m_errorMsg = "Bad char '" + std::string(1, line[x]) + 
                                   "' at line " + std::to_string(lineNum) + ", column " + std::to_string(x + 1);
                        return load_fail_bad_format;
                }
                m_maze[y][x] = me;
            }
            y--;
        }

        if (lineNum < LEVEL_HEIGHT) {
            m_errorMsg = "Too few lines in file (expected " + std::to_string(LEVEL_HEIGHT) + 
                       " lines, got " + std::to_string(lineNum) + ")";
            return load_fail_bad_format;
        }

        // Check boundaries
        if (!checkBoundaries()) {
            m_errorMsg = "Boundary check failed: all edges must be walls (@)";
            return load_fail_bad_format;
        }

        if (num_factories != 1) {
            m_errorMsg = "Level must contain exactly one lemming factory (L)";
            return load_fail_bad_format;
        }
        
        if (!has_exit) {
            m_errorMsg = "No exit (X) found in level";
            return load_fail_bad_format;
        }

        /* Read the tools line */
        if (!std::getline(levelFile, m_tools)) {
            m_errorMsg = "Missing tools line after the grid";
            return load_fail_bad_format;
        } 

		std::string validTools("TNPS<>");
		for (char& c : m_tools)
		{
			char upperC = toupper(c);
			size_t pos = validTools.find(upperC);
			if (pos != std::string::npos)
				c = upperC;
			else
			{
            	m_errorMsg = "Bad char '" + std::string(1, c) + "' in tools line";
            	return load_fail_bad_format;
			}
		}

        m_errorMsg = "Success";
        return load_success;
    }
    
    std::string getErrorMessage() const {
        return m_errorMsg;
    }

    MazeEntry getContentsOf(Coord pos) const {
        if (pos.x >= 0 && pos.x < LEVEL_WIDTH && pos.y >= 0 && pos.y < LEVEL_HEIGHT)
            return m_maze[pos.y][pos.x];
        else
            return empty;
    }

    std::string getTools() const {
        return m_tools;
    }


private:
    std::vector<std::vector<MazeEntry>> m_maze;
    std::string m_pathPrefix;
    std::string m_errorMsg;
    std::string m_tools;

    static const int LEVEL_WIDTH = 20;
    static const int LEVEL_HEIGHT = 20;

    bool checkBoundaries() const {
        for (int x = 0; x < LEVEL_WIDTH; x++) {
            if (m_maze[0][x] != floor || m_maze[LEVEL_HEIGHT - 1][x] != floor)
                return false;
        }
        for (int y = 0; y < LEVEL_HEIGHT; y++) {
            if (m_maze[y][0] != floor || m_maze[y][LEVEL_WIDTH - 1] != floor)
                return false;
        }
        return true;
    }
};

#endif // LEVEL_H_
