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
#ifndef _otbMorphologicalPyramidAnalyseFilter_h
#define _otbMorphologicalPyramidAnalyseFilter_h

#include "otbImageToImageListFilter.h"

namespace otb
{
/** \class MorphologicalPyramidAnalyseFilter
 * \brief Multiscale analysis filter using the morphological pyramid algorithm.
 *
 * Pyramidal decomposition is based on the following statement : once an
 * image has been smoothed with a linear filter, it does not contain
 * any more high-frequency details. Therefore, it can be down-sampled
 * without any loss of information, according to Shannon Theorem. By
 * iterating the same smoothing on the down-sampled image, a
 * multi-resolution decomposition of the scene is
 * computed. If the smoothing filter is a morphological filter, this
 * is no longer true, as the filter is not linear. However, by keeping
 * the details possibly lost in the down-sampling operation, such a
 * decomposition can be used. 
 *
 * The MorphologicalPyramidAnalyseFilter provide such a
 * decomposition. It's computation process is an iterative analyse
 * involving smoothing by the morphological filter, computing the
 * details lost in the smoothing, down-sampling the current image, and
 * computing the details lost in the down-sampling.
 *
 * \ingroup MultiScale
 * \sa MorphologicalPyramidSynthesisFilter,OpeningClosingMorphologicalFilter,
 * ClosingOpeningMorphologicalFilter
 */
template <class TInputImage, class TOutputImage, class TMorphoFilter>
class MorphologicalPyramidAnalyseFilter
  : public ImageToImageListFilter<TInputImage,TOutputImage>
{
public:
  /** Standard typedefs */
  typedef MorphologicalPyramidAnalyseFilter                    Self;
  typedef ImageToImageListFilter<TInputImage,TOutputImage>     Superclass;
  typedef itk::SmartPointer<Self>                              Pointer;
  typedef itk::SmartPointer<const Self>                        ConstPointer;
  /** Creation through object factory macro */
  itkNewMacro(Self);
  /** Type macro */
  itkTypeMacro(MorphologicalPyramidAnalyseFilter,MorphologicalPyramidAnalyseFilter);
  /** Input image dimension constant macro */
  itkStaticConstMacro(InputImageDimension, unsigned int,TInputImage::ImageDimension);
  /** Template parameters typedefs */
  typedef TMorphoFilter MorphoFilterType;
  typedef typename MorphoFilterType::KernelType KernelType;
  /** Output related typedefs */
  typedef typename Superclass::OutputImageType         OutputImageType;
  typedef typename Superclass::OutputImageListType     OutputImageListType;
  typedef typename OutputImageListType::Pointer        OutputImageListPointerType;
  typedef typename Superclass::OutputImagePointerType  OutputImagePointerType;
  /** Input related typedefs */
  typedef typename Superclass::InputImageType          InputImageType;          
  typedef typename Superclass::InputImageRegionType    InputImageRegionType;
  typedef typename InputImageType::Pointer             InputImagePointerType;
  typedef typename InputImageType::ConstPointer        InputImageConstPointerType;
  typedef typename InputImageType::SizeType            SizeType;                 
  typedef typename InputImageType::ValueType           ValueType;  
  typedef typename InputImageType::PixelType           PixelType;
  typedef typename InputImageType::SpacingType         SpacingType;
  /** Size and spacing vector typedefs */
  typedef std::vector<SpacingType>                     SpacingVectorType;
  typedef std::vector<SizeType>                        SizeVectorType;
  /** Accessors */
  itkSetMacro(NumberOfIterations, int);
  itkSetMacro(SubSampleScale, float);
  itkGetConstReferenceMacro(NumberOfIterations, int);
  itkGetConstReferenceMacro(SubSampleScale, float);
  /** Details images lists getters */
  itkGetConstReferenceMacro(SupFiltre,OutputImageListPointerType);
  itkGetConstReferenceMacro(InfFiltre,OutputImageListPointerType);
  itkGetConstReferenceMacro(SupDeci,OutputImageListPointerType);
  itkGetConstReferenceMacro(InfDeci,OutputImageListPointerType);
  /* /\** Spacings list getters *\/ */
/*   virtual  std::vector<typename TOutputImage::SpacingType> GetSpacing(); */
/*   /\** Size list getters *\/ */
/*   virtual  std::vector<typename TOutputImage::SizeType> GetSize(); */

protected:  
  /** Constructor */
  MorphologicalPyramidAnalyseFilter();
  /** Destructor */
  ~MorphologicalPyramidAnalyseFilter();
  virtual void GenerateOutputInformation(){}; // does nothing
  /** Main computation method */
  virtual void GenerateData();
  /** Printself method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;  
  /**
   * Resample an image according to specific size and spacing.
   * \param image The image to down sample.
   * \param size The new size of the image.
   * \param spacing The new spacing of the image.
   * \return The resampled image.
   */
  InputImagePointerType ResampleImage(InputImagePointerType image, SizeType size, SpacingType spacing);

 private:
  /** Number of iterations of the algorithm */
  int m_NumberOfIterations;
  /** Subsampling scale used */
  float m_SubSampleScale;
  /** Sup details from filtering operations */
  OutputImageListPointerType m_SupFiltre;
  /** Inf details from filtering operations */
  OutputImageListPointerType m_InfFiltre;
  /** Sup details from subsampling operations */
  OutputImageListPointerType m_SupDeci;
  /** Inf details from subsampling operations */
  OutputImageListPointerType m_InfDeci;
  /** Spacings vector */
  SpacingVectorType m_Spacing;
  /** Sizes vector */
  SizeVectorType m_Size;
};
}// End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMorphologicalPyramidAnalyseFilter.txx"
#endif

#endif
