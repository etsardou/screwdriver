/***************************************************************************
 *   Copyright (C) 2014 by Tsardoulias Emmanouil                           *
 *   Author: Tsardoulias Emmanouil  <etsardou@gmail.com>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef SCREWDRIVER_GENETIC_HPP
#define SCREWDRIVER_GENETIC_HPP

#include <iostream>
#include <vector>

//  Screwdriver generic namespace
namespace Screwdriver
{
  
  /**
   * @struct Person
   * @brief An individual of the GA population 
   * */
  template <class T>
  struct Person
  {
    // The individual's genome
    std::vector<T> genome;
  };
  
  
  /**
   * @class Genetic
   * @brief Abstractly implements a genetic algorithm
   * */
  template <class T>
  class Genetic
  {
    private:
      
      // The population  
      std::vector<Person<T> > population;
      
    protected:
      
      Genetic(void){}
      
    public:
    
      /**
       * @brief Initializes the population
       * @param [in] pop_N : The population size
       * @param [in] genome_size : The genome size
       * @param [in] initial_value : The initial value for all the genome values
       * */
      void initialize_population(
        unsigned int pop_N, 
        unsigned int genome_size,
        T initial_value
      )
      {
        population = std::vector<Person<T> >(pop_N, Person<T>());
        
        for(typename std::vector<Person<T> >::iterator it = population.begin() ; 
          it != population.end() ; it++)
        {
          it->genome = std::vector<T>(genome_size, initial_value);
        }
      }
    
  };
}

#endif
