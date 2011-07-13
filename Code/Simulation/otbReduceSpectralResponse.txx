/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbReduceSpectralResponse_txx
#define __otbReduceSpectralResponse_txx

#include "otbReduceSpectralResponse.h"

#include <algorithm>

namespace otb
{

template <class TSpectralResponse , class TRSR>
ReduceSpectralResponse <TSpectralResponse , TRSR>
::ReduceSpectralResponse()
{
    m_ReduceResponse = InputSpectralResponseType::New();
}

template <class TSpectralResponse , class TRSR>
bool
ReduceSpectralResponse<TSpectralResponse , TRSR>
::Clear()
{
   return ( m_InputSatRSR->Clear() & m_InputSpectralResponse->Clear() );
}


template <class TSpectralResponse , class TRSR>
inline typename ReduceSpectralResponse<TSpectralResponse , TRSR>::ValuePrecisionType
ReduceSpectralResponse<TSpectralResponse , TRSR>
::operator()(const unsigned int numBand)
{
   if ( numBand >= m_InputSatRSR->GetNbBands() )
   {
      itkExceptionMacro(<< "There is no band num " << numBand << " in the RSR vector!(Size of the current RSR vector is " << m_InputSatRSR->GetNbBands() << ")" );
   }
   else
   {
      ValuePrecisionType res = itk::NumericTraits<ValuePrecisionType>::ZeroValue();
      ValuePrecisionType response1;
      ValuePrecisionType response2;
      ValuePrecisionType inputRSR1;
      ValuePrecisionType inputRSR2;

      PrecisionType lambda1;
      PrecisionType lambda2;

      typename VectorPairType::const_iterator it;
      VectorPairType pairs = (m_InputSatRSR->GetRSR())[numBand]->GetResponse();
      it = pairs.begin();

      ValuePrecisionType totalArea = static_cast<ValuePrecisionType>(0);
      while ( it != pairs.end() -1 )
      {

      lambda1=(*it).first;
      lambda2=(*(it+1)).first;
      inputRSR1=(*it).second;
      inputRSR2=(*(it+1)).second;
      response1 = (*m_InputSpectralResponse)( lambda1 ) * inputRSR1;
      response2 = (*m_InputSpectralResponse)( lambda2 ) * inputRSR2;

      ValuePrecisionType rmin = std::min(response1, response2);
      ValuePrecisionType rmax = std::max(response1, response2);
      /*
                  rmax +\
                       | ---\
                       |     --| rmin
                       |       |
                       |       |
                       |       |
                       |       |
                       |-------+
                     lambda1   lambda2
      */
      
      //Compute the surface of the trapezoid
      ValuePrecisionType area = (lambda2-lambda1)*(rmax+rmin)/2.0;
      res += area;
      totalArea += (lambda2-lambda1);
      ++it;
      }
      return res/totalArea;
   }

}

template <class TSpectralResponse , class TRSR>
void
ReduceSpectralResponse<TSpectralResponse , TRSR>
::CalculateResponse()
{
   m_ReduceResponse->Clear();
   //Compute the reduce response for each band of the sensor
   for (unsigned int i=0; i<m_InputSatRSR->GetNbBands(); ++i)
   {
      PairType pair;
      //pair.first = center wavelength of the band
      pair.first=( (this->m_InputSatRSR->GetRSR())[i]->GetInterval().first + (this->m_InputSatRSR->GetRSR())[i]->GetInterval().second );
      pair.first=pair.first/2.0;
      pair.second=(*this)(i);
      m_ReduceResponse->GetResponse().push_back(pair);
   }

}


template <class TSpectralResponse , class TRSR>
void
ReduceSpectralResponse<TSpectralResponse , TRSR>
::LoadInputsFromFiles (const std::string & spectralResponseFile, const std::string & RSRFile, const unsigned int nbRSRBands, ValuePrecisionType coefNormSpectre, ValuePrecisionType coefNormRSR)
{
   //Instantiation
   m_InputSpectralResponse=InputSpectralResponseType::New();
   /** Load the spectral response file*/
   m_InputSpectralResponse->Load(spectralResponseFile, coefNormSpectre);

   m_InputSatRSR=InputRSRType::New();
   /** Set the satelite number of bands */
   m_InputSatRSR->SetNbBands(nbRSRBands);
   /** Load the satelite response file*/
   m_InputSatRSR->Load(RSRFile, coefNormRSR);
}


template <class TSpectralResponse , class TRSR>
void
ReduceSpectralResponse<TSpectralResponse , TRSR>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
   Superclass::PrintSelf(os, indent);
   os<<std::endl;
   os << "spectre " << m_InputSpectralResponse <<std::endl;
   os << "Sat RSR " << m_InputSatRSR <<std::endl;

   os<<std::endl;
   os <<indent << "[Center Wavelength (micrometers), Reflectance (percent)]" << std::endl;
   for(typename VectorPairType::const_iterator it = m_ReduceResponse->GetResponse().begin(); it != m_ReduceResponse->GetResponse().end(); ++it)
   {
     os <<indent << "Band Nb : "<< it - m_ReduceResponse->GetResponse().begin() << ": [" << (*it).first << ","<< (*it).second << "]" << std::endl;
   }
}

} // end namespace otb

#endif
