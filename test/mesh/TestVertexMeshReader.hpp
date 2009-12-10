/*

Copyright (C) University of Oxford, 2005-2009

University of Oxford means the Chancellor, Masters and Scholars of the
University of Oxford, having an administrative office at Wellington
Square, Oxford OX1 2JD, UK.

This file is part of Chaste.

Chaste is free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 2.1 of the License, or
(at your option) any later version.

Chaste is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details. The offer of Chaste under the terms of the
License is subject to the License being interpreted in accordance with
English Law and subject to any action against the University of Oxford
being under the jurisdiction of the English Courts.

You should have received a copy of the GNU Lesser General Public License
along with Chaste. If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef TESTVERTEXMESHREADER2D_HPP_
#define TESTVERTEXMESHREADER2D_HPP_

#include <cxxtest/TestSuite.h>

#include "VertexMeshReader.hpp"

/*
 * This typedef is just because we can't have lines such as
 * TS_ASSERT_THROWS_NOTHING(p_mesh_reader=new VertexMeshReader<2,2>(name));
 * because the macro thinks the comma separates two arguments
 */
typedef VertexMeshReader<2,2> READER_2D;

class TestVertexMeshReader : public CxxTest::TestSuite
{
public:

    /**
     * Check that input files are opened correctly.
     */
    void TestFilesOpen() throw(Exception)
    {
        VertexMeshReader<2,2> mesh_reader("notforrelease_cell_based/test/data/TestVertexMesh/vertex_mesh");
    }


    /**
     * Check that the nodes are read correctly. Checks that the output vector
     * for a given input file is the correct length and that if the input file
     * is corrupted (missing nodes) then an exception is thrown.
     */
    void TestNodesDataRead() throw(Exception)
    {
        VertexMeshReader<2,2> mesh_reader("notforrelease_cell_based/test/data/TestVertexMesh/vertex_mesh");

        TS_ASSERT_EQUALS(mesh_reader.GetNumNodes(), 7u);

        VertexMeshReader<2,2> mesh_reader2("notforrelease_cell_based/test/data/baddata/vertex_mesh_bad_nodes");

        // Reads node 0 from file
        TS_ASSERT_THROWS_NOTHING(mesh_reader2.GetNextNode());

        // Reads node 3 from file when expecting number 1
        TS_ASSERT_THROWS_THIS(mesh_reader2.GetNextNode(),"Data for node 1 missing");
    }


    /**
     * Check that the elements are read correctly. Checks that the output vector
     * for a given input file is the correct length and that if the input file
     * is corrupted (missing elements) then an exception is thrown.
     */
    void TestElementsDataRead() throw(Exception)
    {
        VertexMeshReader<2,2> mesh_reader("notforrelease_cell_based/test/data/TestVertexMesh/vertex_mesh");

        TS_ASSERT_EQUALS(mesh_reader.GetNumElements(), 2u);

        // Read element 0 from file
        ElementData data = mesh_reader.GetNextElementData();

        TS_ASSERT_EQUALS(data.NodeIndices.size(), 5u);
        TS_ASSERT_EQUALS(data.NodeIndices[0], 0u);
        TS_ASSERT_EQUALS(data.NodeIndices[1], 1u);
        TS_ASSERT_EQUALS(data.NodeIndices[2], 2u);
        TS_ASSERT_EQUALS(data.NodeIndices[3], 3u);
        TS_ASSERT_EQUALS(data.NodeIndices[4], 4u);

        // Read element 1 from file
        ElementData data2 = mesh_reader.GetNextElementData();

        TS_ASSERT_EQUALS(data2.NodeIndices.size(), 3u);
        TS_ASSERT_EQUALS(data2.NodeIndices[0], 2u);
        TS_ASSERT_EQUALS(data2.NodeIndices[1], 5u);
        TS_ASSERT_EQUALS(data2.NodeIndices[2], 6u);

        TS_ASSERT_EQUALS(mesh_reader.GetNumElementAttributes(), 0u);

        mesh_reader.Reset();
        for (unsigned i=1; i<mesh_reader.GetNumElements(); i++)
        {
            ElementData data = mesh_reader.GetNextElementData();
            TS_ASSERT_EQUALS(data.AttributeValue, 0u);
        }

        VertexMeshReader<2,2> mesh_reader2("notforrelease_cell_based/test/data/baddata/vertex_mesh_bad_elements");

        // Reads element 0 from file
        TS_ASSERT_THROWS_NOTHING(mesh_reader2.GetNextElementData());

        // Reads element 2 from file when expecting number 1
        TS_ASSERT_THROWS_THIS(mesh_reader2.GetNextElementData(), "Data for element 1 missing");
    }


    /**
     * Checks that nodes in the input data file are numbered sequentially.
     * (In the input file nodes must appear in increasing order since the node
     * number is only stored as the index of the vector in which the coordinates
     * are stored.)
     */
    void TestPermutedNodesFail() throw(Exception)
    {
        VertexMeshReader<2,2> mesh_reader("notforrelease_cell_based/test/data/baddata/vertex_mesh_permuted_nodes");
        TS_ASSERT_THROWS_THIS(for(unsigned i=0;i<mesh_reader.GetNumNodes();i++){mesh_reader.GetNextNode();}, "Data for node 3 missing")
    }


    /**
     * Check that GetNextNode() returns the coordinates of the correct node.
     * Compares the coordinates of the first two nodes with their known
     * values, checks that no errors are thrown for the remaining nodes and
     * that an error is thrown if we try to call the function too many times.
     */
    void TestGetNextNode() throw(Exception)
    {
        VertexMeshReader<2,2> mesh_reader("notforrelease_cell_based/test/data/TestVertexMesh/vertex_mesh");

        std::vector<double> first_node;
        first_node = mesh_reader.GetNextNode();

        TS_ASSERT_DELTA(first_node[0], 0.0, 1e-6);
        TS_ASSERT_DELTA(first_node[1], 0.0, 1e-6)

        std::vector<double> next_node;
        next_node = mesh_reader.GetNextNode();

        TS_ASSERT_DELTA(next_node[0], 1.0, 1e-6);
        TS_ASSERT_DELTA(next_node[1], 0.0, 1e-6);

        for (unsigned i=0; i<5; i++)
        {
            TS_ASSERT_THROWS_NOTHING(next_node = mesh_reader.GetNextNode());
        }

        TS_ASSERT_THROWS_THIS(next_node = mesh_reader.GetNextNode(),
                "Cannot get the next line from node or element file due to incomplete data");
    }


    /**
     * Check that GetNextElementData() works. Checks that no errors are thrown for
     * all of the elements and that an error is thrown if we try to call the
     * function too many times.
     */
    void TestGetNextElementData() throw(Exception)
    {
        VertexMeshReader<2,2> mesh_reader("notforrelease_cell_based/test/data/TestVertexMesh/vertex_mesh");

        std::vector<unsigned> next_element;
        for (unsigned i=0; i<mesh_reader.GetNumElements(); i++)
        {
            TS_ASSERT_THROWS_NOTHING(next_element = mesh_reader.GetNextElementData().NodeIndices);
        }

        TS_ASSERT_THROWS_THIS(next_element = mesh_reader.GetNextElementData().NodeIndices,
                "Cannot get the next line from node or element file due to incomplete data");
    }


    void TestReadingElementAttributes() throw(Exception)
    {
        VertexMeshReader<2,2> mesh_reader("notforrelease_cell_based/test/data/TestVertexMeshReader2d/vertex_mesh_with_element_attributes");

        TS_ASSERT_EQUALS(mesh_reader.GetNumElements(), 2u);

        TS_ASSERT_EQUALS(mesh_reader.GetNumElementAttributes(), 1u);

        ElementData next_element_info = mesh_reader.GetNextElementData();
        std::vector<unsigned> nodes = next_element_info.NodeIndices;
        TS_ASSERT_EQUALS(nodes.size(), 5u);
        TS_ASSERT_EQUALS(next_element_info.AttributeValue, 97u);

        next_element_info = mesh_reader.GetNextElementData();
        nodes = next_element_info.NodeIndices;
        TS_ASSERT_EQUALS(nodes.size(), 3u);
        TS_ASSERT_EQUALS(next_element_info.AttributeValue, 152u)

        /*
         * Coverage
         *
         * \todo The methods GetNextFaceData() and GetNumFaces() are not
         * fully implemented for VertexMeshReader, but must be overridden
         * as they are pure virtual in the base class. When they are
         * implemented, these lines need to be replaced by proper tests.
         *
         * See also #1001
         */
        ElementData face_data = mesh_reader.GetNextFaceData();
        TS_ASSERT_EQUALS(face_data.NodeIndices.empty(), true);
        TS_ASSERT_EQUALS(face_data.AttributeValue, 0u);

        TS_ASSERT_EQUALS(mesh_reader.GetNumFaces(), 0u);
        TS_ASSERT_EQUALS(mesh_reader.GetNumEdges(), 0u);
    }

    void TestOtherExceptions() throw(Exception)
    {
        TS_ASSERT_THROWS_THIS(READER_2D mesh_reader("notforrelease_cell_based/test/data/nonexistent_file"),
                "Could not open data file: notforrelease_cell_based/test/data/nonexistent_file.node");
        TS_ASSERT_THROWS_THIS(READER_2D mesh_reader("notforrelease_cell_based/test/data/baddata/vertex_mesh_without_element_file"),
                "Could not open data file: notforrelease_cell_based/test/data/baddata/vertex_mesh_without_element_file.cell");
    }

};
#endif /*TESTVERTEXMESHREADER2D_HPP_*/