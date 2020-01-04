//
//  Tree.cpp
//  ForestTools
//
//  Created by DeAngelo Deshawn Wilson on 8/20/19.
//  Copyright © 2019 DeAngelo Deshawn Wilson. All rights reserved.
//

#include "Tree.h"
#include <algorithm>

namespace phylo {
    Tree::Tree() :
        adjacencyList(),
        counter()
    {
        //minimum size of viable tree is 4 leafs (quartet) (6 vertices + 5 edges entries)
        this->adjacencyList.reserve(11);
    }

    Tree::~Tree()
    {
    }

    //copy constructor
    Tree::Tree(const Tree& tree)
    {
        //copy all contents of tree_adjList to this adjList
        this->adjacencyList = tree.adjacencyList;
        this->counter = tree.counter;
    }
    /*
    Tree& Tree::operator=(const Tree& rhs)
    {
        this->adjacencyList = rhs.adjacencyList;
        this->counter = rhs.counter;
        return *this;
    }
    */
    Tree& Tree::operator=(Tree rhs)
    {
        swap(rhs);
        return *this;
    }

    void Tree::swap(Tree& rhs)
    {
        //swap this tree with rhs
        std::swap(rhs.adjacencyList, this->adjacencyList);
        std::swap(rhs.counter, this->counter);
    }

    /*
     ----------------------------
     NON-CONST MODIFIER FUNCTIONS
     ----------------------------
     */

     //PARSER: Newick expression -> phylo::Tree





     /*
      WHEN SHOULD THE VECTOR (adjList) be SORTED
      */
      //IF edge added before vertex, NOT SORTED into correct ordering


    void Tree::addVertex(const std::string& v)
    {
        const std::string emp;
        this->adjacencyList.push_back(adjPair(v, emp));

        //this->sortAdjacenyList();
    }

    //no duplicate edges handled
    void Tree::addEdge(const std::string& v, const std::string& e)
    {
        this->adjacencyList.push_back(adjPair(v, e));
        this->adjacencyList.push_back(adjPair(e, v));

        //this->sortAdjacenyList();
    }

    void Tree::deleteEdge(const std::string& v, const std::string& e)
    {
        deleteEdgeHelper(v, e);
        deleteEdgeHelper(e, v);
    }
    //finds (vertex,edge) pair if in adjList -> removes from list
    void Tree::deleteEdgeHelper(const std::string& v, const std::string& e)
    {
        //find edge if it exists
        auto pair = std::lower_bound(this->adjacencyList.begin(), this->adjacencyList.end(), adjPair(v, e), AdjPairCompare());
        //replace slow finds with LOwer_bound / binary_search (with vector sorted)
        //auto pair = std::find(this->adjacencyList.begin(), this->adjacencyList.end(), adjPair(v,e));
        if (pair != this->adjacencyList.end())
        {
            this->adjacencyList.erase(pair);
        }
        else
        {
            this->outputAdjacentList();
            printf("Tree::deleteEdge-- could not find edge\n");
        }
    }
    void Tree::deleteVertex(const std::string& v)
    {

        //Remove edge entries with w -> "v" vertex
        const std::vector<std::string> adjacents = this->getAdjacents(v);
        for (std::vector<std::string>::const_iterator it = adjacents.begin(); it != adjacents.end(); it++)
        {
            this->deleteEdge(*it, v);
        }


        //AdjPairKeyCompare --> ignores second element of pair (edge) when searching
        auto iterPair = std::equal_range(this->adjacencyList.begin(), this->adjacencyList.end(), v, AdjPairKeyCompare());
        /*
        if(std::distance(iterPair.first, iterPair.second) > 1)
        {
            printf("error deleting edges\n");
        }
        */
        //for each "v" vector entry in range -- remove from adjList
        for (std::vector<adjPair>::const_iterator it = iterPair.first; it != iterPair.second; it++)
        {
            this->adjacencyList.erase(it);
        }

    }

    void Tree::sortAdjacenyList()
    {
        //std::stable_sort(this->adjacencyList.begin(), this->adjacencyList.end(), AdjPairKeyCompare());
        std::stable_sort(this->adjacencyList.begin(), this->adjacencyList.end(), AdjPairCompare());
        //this->outputAdjacentList();
    }


    /*

     Const methods
        -accessors
     */

     //returns vector with names of all vertexes
    const std::vector<std::string> Tree::vertices() const
    {
        std::vector<std::string> v;
        for (std::vector<adjPair>::const_iterator it = this->adjacencyList.begin(); it != this->adjacencyList.end(); it++)
        {
            if ((*it).second.empty())
            {
                v.push_back((*it).first);
            }
        }
        /*
        for(int i = 0; i<this->adjacencyList.size(); i++)
        {
            adjPair tmp = this->adjacencyList[i];
            if(tmp.second.empty())
            {
                v.push_back(tmp.first);
            }
        }
        */
        return v;
    }

    //returns count of vertexes
    int Tree::v() const
    {
        int count = 0;
        for (std::vector<adjPair>::const_iterator it = this->adjacencyList.begin(); it != this->adjacencyList.end(); it++)
        {
            if ((*it).second.empty())
            {
                count += 1;
            }
        }
        return count;
        //return static_cast<int>(this->adjacencyList.size());
    }

    int Tree::e() const
    {

        return (static_cast<int>(this->adjacencyList.size()) - this->v()) / 2;
        /*
        for(int i = 0; i<this->adjacencyList.size(); i++)
        {
            //add size of current vertex's adjList
            edges += this->adjacencyList[i].second.size();
        }
        return edges/2;
        */
    }

    //gets vector of all leaves in "this" Tree
    const std::vector<std::string> Tree::leafNameList() const
    {
        std::vector<std::string> leaves;
        std::vector<std::string> visited;//visitied vertexes added to vector
        const std::vector<std::string> vertices = this->vertices();

        //for each vertex in tree
        for (std::vector<std::string>::const_iterator it = vertices.begin(); it != vertices.end(); it++)
        {
            //if verticies NOT found in visited (visited not sorted)
            if (visited.end() == std::find(visited.begin(), visited.end(), *it))
            {
                this->leafNameListHelper(*it, leaves, visited);
            }
        }

        std::stable_sort(leaves.begin(), leaves.end());
        return leaves;
    }
    //given vertex to find leaves reachable from
    const std::vector<std::string> Tree::leafNameList(const std::string& vertex) const
    {
        std::vector<std::string> leaves;
        std::vector<std::string> visited;//visitied vertexes added to vector

        //recurse on all vertices reachable from "vertex"
        this->leafNameListHelper(vertex, leaves, visited);

        std::stable_sort(leaves.begin(), leaves.end());
        return leaves;
    }

    void Tree::leafNameListHelper(const std::string& v, std::vector<std::string>& leaves, std::vector<std::string>& visited) const
    {
        //printf("LeafListHelper:: v: %s\n", v.c_str());

        //add v to vistited
        visited.push_back(v);
        if (this->isLeaf(v))
        {
            //printf("LeafListHelper:: leaf: %s\n", v.c_str());
            leaves.push_back(v);
        }
        std::vector<std::string> adjacents = this->getAdjacents(v);

        //loop all adjacent vertices
        for (std::vector<std::string>::const_iterator it = adjacents.begin(); it != adjacents.end(); it++)
        {
            //if current adjacent vertex not visited (visited not guranteed sorted --> std::find())
            if (visited.end() == std::find(visited.begin(), visited.end(), *it))
            {
                leafNameListHelper(*it, leaves, visited);
            }
        }

    }

    int Tree::getDegree(const std::string& vertex) const
    {
        auto adjacentsIters = std::equal_range(this->adjacencyList.begin(), this->adjacencyList.end(), vertex, AdjPairKeyCompare());
        //MINUS 1 for "blank" vertex entry
        //int d = static_cast<int>(std::distance(adjacentsIters.first, adjacentsIters.second))-1;
        //printf("degree: %d\n", d);
        return static_cast<int>(std::distance(adjacentsIters.first, adjacentsIters.second)) - 1;
    }

    const std::vector<std::string> Tree::getAdjacents(const std::string& vertex) const
    {
        auto adjacentsIters = std::equal_range(this->adjacencyList.begin(), this->adjacencyList.end(), vertex, AdjPairKeyCompare());
        std::vector<std::string> res;
        //skips first (vertex entry)
        for (std::vector<adjPair>::const_iterator it = adjacentsIters.first; it != adjacentsIters.second; it++)
        {
            //ignores "vertex"
            if ((*it).second.empty()) {/*next*/ }
            else
            {
                //printf("adjs: %s\n",(*it).second.c_str());
                res.push_back((*it).second);
            }
        }
        return res;
    }

    //TODO
    const std::pair<const std::string, const std::string> Tree::findSiblings() const
    {
        const std::vector<std::string> vertices = this->vertices();
        for (std::vector<std::string>::const_iterator it = vertices.begin(); it != vertices.end(); it++)
        {
            if (getDegree(*it) == 3)
            {
                std::vector<std::string> tmp;
                const std::vector<std::string> adjacents = this->getAdjacents(*it);
                for (std::vector<std::string>::const_iterator it = adjacents.begin(); it != adjacents.end(); it++)
                {
                    if (isLeaf(*it))
                    {
                        tmp.push_back(*it);
                    }
                }
                if (tmp.size() == 2) {
                    return std::pair<std::string, std::string>(tmp.at(0), tmp.at(1));
                }
            }
        }


        return std::pair<std::string, std::string>(std::string(), std::string());
    }

    bool Tree::isSiblings(const std::string& v, const std::string& w) const
    {
        if (!isLeaf(v) || !isLeaf(w))
        {
            return false;
        }
        const std::vector<std::string> vertices = this->vertices();
        for (std::vector<std::string>::const_iterator it = vertices.begin(); it != vertices.end(); it++)
        {
            if (isAdjacent(v, *it) && isAdjacent(w, *it))
            {
                return true;
            }
        }
        return false;

    }

    bool Tree::isLeaf(const std::string& v) const
    {
        return this->getDegree(v) == 1;
    }

    bool Tree::isAdjacent(const std::string& vertex, const std::string& edge) const
    {
        const std::vector<std::string> adjacents = this->getAdjacents(vertex);
        return std::binary_search(adjacents.begin(), adjacents.end(), edge);
        //return adjacents.end() != std::find(adjacents.begin(), adjacents.end(), edge);
    }

    /*
     REPRESENTATION FUNCTIONS
     */
     //TODO

    std::string Tree::to_newick() const
    {
        const std::vector<std::string> vertices = this->vertices();
        //find internal vertex
        std::string internal;
        std::vector<std::string> visitied;
        for (std::vector<std::string>::const_iterator it = vertices.begin(); it != vertices.end(); it++)
        {
            if (getDegree(*it) == 3)
            {
                internal = *it;
            }
        }
        std::string s;
        s = to_newickHelper(internal, s, visitied).append(";");

        return s;
    }
    std::string& Tree::to_newickHelper(const std::string& v, std::string& res, std::vector<std::string>& visited) const
    {
        visited.push_back(v);
        /*
        if leafNameList.include?(v) then
            s += v
            return s
            end
        */
        //printf("vertex: %s\n", v.c_str());
        if (this->isLeaf(v)) {
            //printf("leaf: %s\n", v.c_str());
            res.append(v);
            return res;
        }
        res.append("(");
        //int first = 1;
        bool first = true;
        const std::vector<std::string> adjacents = this->getAdjacents(v);
        for (std::vector<std::string>::const_iterator it = adjacents.begin(); it != adjacents.end(); it++)
        {
            //if adj_edge not in visited
            if (visited.end() == std::find(visited.begin(), visited.end(), *it))
            {
                if (first)
                {
                    first = false;
                }
                else { res.append(","); }
                //printf("vertex: %s\n", (*it).c_str());
                res = to_newickHelper(*it, res, visited);
            }
        }
        res.append(")");

        return res;
    }
    /*
    std::string Tree::to_s() const
    {
        return std::string();
    }
    */

    void Tree::outputAdjacentList() const
    {
        for (std::vector<adjPair>::const_iterator it = this->adjacencyList.begin(); it != this->adjacencyList.end(); it++)
        {
            //print out entries of adjList
            printf("(%s, %s)\n", (*it).first.c_str(), (*it).second.c_str());
        }

    }

}

