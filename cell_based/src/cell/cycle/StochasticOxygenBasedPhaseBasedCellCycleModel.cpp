/*

Copyright (c) 2005-2016, University of Oxford.
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

#include "StochasticOxygenBasedPhaseBasedCellCycleModel.hpp"
#include "ApoptoticCellProperty.hpp"
#include "CellPropertyRegistry.hpp"

StochasticOxygenBasedPhaseBasedCellCycleModel::StochasticOxygenBasedPhaseBasedCellCycleModel()
    : SimpleOxygenBasedPhaseBasedCellCycleModel()
{
}

StochasticOxygenBasedPhaseBasedCellCycleModel::StochasticOxygenBasedPhaseBasedCellCycleModel(const StochasticOxygenBasedPhaseBasedCellCycleModel& rModel)
   : SimpleOxygenBasedPhaseBasedCellCycleModel(rModel)
{
    /*
     * Set each member variable of the new cell-cycle model that inherits
     * its value from the parent.
     *
     * Note 1: some of the new cell-cycle model's member variables will already
     * have been correctly initialized in its constructor or parent classes.
     *
     * Note 2: one or more of the new cell-cycle model's member variables
     * may be set/overwritten as soon as InitialiseDaughterCell() is called on
     * the new cell-cycle model.
     *
     * Note 3: Only set the variables defined in this class. Variables defined
     * in parent classes will be defined there.
     *
     */

    // No new member variables.
}

void StochasticOxygenBasedPhaseBasedCellCycleModel::GenerateStochasticG2Duration()
{
    RandomNumberGenerator* p_gen = RandomNumberGenerator::Instance();

    double mean = AbstractPhaseBasedCellCycleModel::GetG2Duration();
    double standard_deviation = 1.0;

    mStochasticG2Duration = p_gen->NormalRandomDeviate(mean, standard_deviation);

    // Check that the normal random deviate has not returned a small or negative G2 duration
    if (mStochasticG2Duration < mMinimumGapDuration)
    {
        mStochasticG2Duration = mMinimumGapDuration;
    }
}

void StochasticOxygenBasedPhaseBasedCellCycleModel::InitialiseDaughterCell()
{
    SimpleOxygenBasedPhaseBasedCellCycleModel::InitialiseDaughterCell();
    GenerateStochasticG2Duration();
}

void StochasticOxygenBasedPhaseBasedCellCycleModel::Initialise()
{
    AbstractSimplePhaseBasedCellCycleModel::Initialise();
    GenerateStochasticG2Duration();
}

void StochasticOxygenBasedPhaseBasedCellCycleModel::ResetForDivision()
{
    SimpleOxygenBasedPhaseBasedCellCycleModel::ResetForDivision();
    GenerateStochasticG2Duration();
}

double StochasticOxygenBasedPhaseBasedCellCycleModel::GetG2Duration() const
{
    return mStochasticG2Duration;
}

AbstractCellCycleModel* StochasticOxygenBasedPhaseBasedCellCycleModel::CreateCellCycleModel()
{
    return new StochasticOxygenBasedPhaseBasedCellCycleModel(*this);
}

void StochasticOxygenBasedPhaseBasedCellCycleModel::OutputCellCycleModelParameters(out_stream& rParamsFile)
{
    // No new parameters to output

    // Call method on direct parent class
    SimpleOxygenBasedPhaseBasedCellCycleModel::OutputCellCycleModelParameters(rParamsFile);
}

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(StochasticOxygenBasedPhaseBasedCellCycleModel)