/*

Copyright (c) 2005-2012, University of Oxford.
All rights reserved.

University of Oxford means the Chancellor, Masters and Scholars of the
University of Oxford, having an administrative office at Wellington
Square, Oxford OX1 2JD, UK.

This file is part of Chaste.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
 * Neither the name of the University of Oxford nor the names of its
   contributors may be used to endorse or promote products derived from this
   software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/
#ifndef ABSTRACTPERIODICTWOBODYINTERACTIONFORCE_HPP_
#define ABSTRACTPERIODICTWOBODYINTERACTIONFORCE_HPP_

#include "AbstractTwoBodyInteractionForce.hpp"
#include "MeshBasedCellPopulation.hpp"
#include "NodeBasedCellPopulation.hpp"

/**
 * An abstract class for two-body force laws.
 */
template<unsigned DIM>
class AbstractPeriodicTwoBodyInteractionForce : public AbstractTwoBodyInteractionForce<DIM>
{
private:

    /** Needed for serialization. */
    friend class boost::serialization::access;
    /**
     * Archive the object and its member variables.
     *
     * @param archive the archive
     * @param version the current version of this class
     */
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractForce<DIM> >(*this);
        archive & mPeriodicDomainWidth;
    }

protected:

    /** Width of the periodic domain. */
    double mPeriodicDomainWidth;

    /** Depth of the periodic domain. */
    double mPeriodicDomainDepth;

    /** An extended mesh, used to implement periodicity. */
    MutableMesh<DIM,DIM>* mpExtendedMesh;

    /** A map from node indices in mpExtendedMesh to node indices in the cell population. */
    std::map<unsigned, unsigned> mExtendedMeshNodeIndexMap;

public:

    /**
     * Constructor.
     */
    AbstractPeriodicTwoBodyInteractionForce();

    /**
     * Destructor.
     */
    ~AbstractPeriodicTwoBodyInteractionForce();

    /**
     * Calculates the force between two nodes.
     *
     * Note that this assumes they are connected and is called by rCalculateVelocitiesOfEachNode()
     *
     * @param nodeAGlobalIndex index of one neighbouring node
     * @param nodeBGlobalIndex index of the other neighbouring node
     * @param rCellPopulation the cell population
     *
     * @return The force exerted on Node A by Node B.
     */
    virtual c_vector<double, DIM> CalculateForceBetweenNodes(unsigned nodeAGlobalIndex, unsigned nodeBGlobalIndex, AbstractCellPopulation<DIM>& rCellPopulation)=0;

    /**
     * Overridden AddForceContribution() method.
     *
     * @param rForces reference to vector of forces on nodes
     * @param rCellPopulation reference to the cell population
     */
    void AddForceContribution(std::vector<c_vector<double, DIM> >& rForces,
                              AbstractCellPopulation<DIM>& rCellPopulation);

    /**
     * Returns the width of the periodic domain.
     */
    double GetPeriodicDomainWidth();

    /**
     * Returns the width of the periodic domain.
     *
     * @param periodicDomainWidth the width of the periodic domain
     */
    void SetPeriodicDomainWidth(double periodicDomainWidth);

    /**
     * Returns the depth of the periodic domain.
     */
    double GetPeriodicDomainDepth();

    /**
     * Returns the depth of the periodic domain.
     *
     * @param periodicDomainDepth the width of the periodic domain
     */
    void SetPeriodicDomainDepth(double periodicDomainDepth);

    /**
     * Overridden OutputForceParameters() method.
     *
     * @param rParamsFile the file stream to which the parameters are output
     */
    virtual void OutputForceParameters(out_stream& rParamsFile);
};

#endif /* ABSTRACTPERIODICTWOBODYINTERACTIONFORCE_HPP_ */
