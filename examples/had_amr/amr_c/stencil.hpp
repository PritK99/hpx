//  Copyright (c) 2007-2010 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HPX_COMPONENTS_AMR_STENCIL_OCT_17_2008_0847AM)
#define HPX_COMPONENTS_AMR_STENCIL_OCT_17_2008_0847AM

#include "../amr/server/functional_component.hpp"
#include "stencil_data.hpp"
#include "../amr/unigrid_mesh.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace hpx { namespace components { namespace amr 
{
    /// This class implements the time step evolution functionality. It has to
    /// expose several functions: \a eval, \a alloc_data and \a free_data. 
    /// The function \a eval computes the value for the current time step based 
    /// on the values as computed by the previous time step. The functions 
    /// \a alloc_data is used to allocate the data needed to store one 
    /// datapoint, while the function \a free_data is used to free the memory
    /// allocated using alloc_data.
    class HPX_COMPONENT_EXPORT stencil 
      : public amr::server::functional_component
    {
    private:
        typedef amr::server::functional_component base_type;

    public:
        typedef stencil wrapped_type;
        typedef stencil wrapping_type;

        stencil();
        ~stencil() {}

        /// This is the function implementing the actual time step functionality
        /// It takes the values as calculated during the previous time step 
        /// and needs to return the current calculated value.
        ///
        /// The name of this function must be eval(), it must return whether
        /// the current time step is the last or past the last. The parameter
        /// \a result passes in the gid of the memory block where the result
        /// of the current time step has to be stored. The parameter \a gids
        /// is a vector of gids referencing the memory blocks of the results of
        /// previous time step.
        int eval(naming::id_type const& result, 
            std::vector<naming::id_type> const& gids, std::size_t row, std::size_t column,
            Parameter const& par);

        // this function creates a finer mesh from 3 input gids and evolves that
        // finer mesh for two steps.
        int finer_mesh(std::vector<naming::id_type> const& gids,
            access_memory_block<stencil_data>& resultval, 
            std::vector<access_memory_block<stencil_data> >& val,
            std::size_t vecvalsize,std::size_t size, 
            std::size_t compute_index, std::size_t row, std::size_t column, 
            Parameter const& par);
        
        /// The alloc function is supposed to create a new memory block instance 
        /// suitable for storing all data needed for a single time step. 
        /// Additionally it fills the memory with initial data for the data 
        /// item given by the parameter \a item (if item != -1).
        naming::id_type alloc_data(int item, int maxitems, int row,
            std::size_t level, had_double_type x, Parameter const& par);

        /// The init function initializes this stencil point
        void init(std::size_t, naming::id_type const&);

        int findpoint(access_memory_block<stencil_data> const& lookright,
                      access_memory_block<stencil_data> & resultval);

        /// floating point comparison (for coordinates)
        int floatcmp(had_double_type x1,had_double_type x2,had_double_type epsilon);

        int prep_initial_data(std::vector<naming::id_type> & initial_data,
            std::vector<naming::id_type> const& gids,
            std::size_t vecvalsize, std::size_t size,
            std::size_t row,std::size_t column,std::size_t numvals, Parameter const& par);

        int prep_restriction_data(std::vector<naming::id_type> & result_data,
            std::size_t numvals,std::size_t size,Parameter const& par);

    private:
        std::size_t numsteps_;
        naming::id_type log_;
    };

}}}

#endif
